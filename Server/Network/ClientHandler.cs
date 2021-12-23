using System;
using System.Net;
using System.Threading;
using System.Net.Sockets;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Server {
    class ClientHandler {
        private TcpListener ListenerHandle;
        private Thread ListenerThread;

        public ClientHandler() {
            ListenerHandle = new TcpListener(IPAddress.Any, Global.iPort);
        }

        public void Start() {
            ListenerThread = new Thread(new ThreadStart(() => {
                ListenerHandle.Start();

                while (true) {
                    Thread.Sleep(100);
                    if (ListenerHandle.Pending()) new Thread(new ParameterizedThreadStart(Handler)).Start(ListenerHandle.AcceptTcpClient());
                }
            }));
            ListenerThread.Start();
        }

        private void Handler(object client) {
            TcpClient tcpClient = (TcpClient)client;
            NetworkStream netStream = tcpClient.GetStream();
            string IP = tcpClient.Client.RemoteEndPoint.ToString().Split(new char[] { ':' })[0];

            EndianWriter serverWriter = new EndianWriter(netStream, EndianStyle.BigEndian);

            if (!netStream.CanRead) {
                Console.WriteLine("Socket closed for {0} - CanRead was false!", IP);
                tcpClient.Close();
                return;
            }

            try {
                List<Log.PrintQueue> logId = Log.GetQueue();
                Header header = new Header();

                byte[] neededHeaderData = new byte[0x8];
                if (netStream.Read(neededHeaderData, 0, 0x8) != 0x8) {
                    Log.Add(logId, ConsoleColor.DarkYellow, "Flag", "Failed to read header start. Banning IP for 1 hour", IP);
                    Log.Print(logId);

                    serverWriter.Write(0x13371337);
                    serverWriter.Write((byte)0x2);
                    serverWriter.Close();
                    tcpClient.Close();
                    return;
                }

                EndianReader baseHeaderParse = new EndianReader(new MemoryStream(neededHeaderData), EndianStyle.BigEndian);

                header.Command = (Packets)(baseHeaderParse.ReadInt32());

                if (!Enum.IsDefined(typeof(Packets), header.Command)) {
                    Log.Add(logId, ConsoleColor.DarkYellow, "Flag", "Client sent an invalid packet (" + header.Command + "). Banning IP for 1 hour", IP);
                    Log.Print(logId);

                    serverWriter.Write(0x13371337);
                    serverWriter.Write((byte)0x3);
                    serverWriter.Close();
                    tcpClient.Close();
                    return;
                }

                header.iSize = baseHeaderParse.ReadInt32();

                if (header.iSize > Global.iMaximumRequestSize) {
                    Log.Add(logId, ConsoleColor.DarkYellow, "Flag", "Client sent a header size bigger than the max (" + header.iSize + "). Banning IP for 1 hour", IP);
                    Log.Print(logId);


                    serverWriter.Write(0x13371337);
                    serverWriter.Write((byte)0x4);
                    serverWriter.Close();
                    tcpClient.Close();
                    return;
                }

                byte[] data = new byte[header.iSize - 8];
                if (netStream.Read(data, 0, header.iSize - 8) != header.iSize - 8) {
                    Log.Add(logId, ConsoleColor.DarkYellow, "Flag", "Failed to read header (" + header.iSize + "). Banning IP for 1 hour", IP);
                    Log.Print(logId);

                    serverWriter.Write(0x13371337);
                    serverWriter.Write((byte)0x5);
                    serverWriter.Close();
                    tcpClient.Close();
                    return;
                }

                byte[] untouchedData = new byte[header.iSize - 8];
                Buffer.BlockCopy(data, 0, untouchedData, 0, header.iSize - 8);

                EndianReader dataReader = new EndianReader(new MemoryStream(data), EndianStyle.BigEndian);

                header.szCPU = dataReader.ReadBytes(0x10);
                header.szHypervisorCPU = dataReader.ReadBytes(0x10);

                if (!header.szCPU.SequenceEqual(header.szHypervisorCPU)) {
                    Log.Add(logId, ConsoleColor.DarkYellow, "Flag", "Client sent a mismatching cpu set (spoofed)", IP);
                    Log.Print(logId);

                    serverWriter.Write(0x13371337);
                    serverWriter.Write((byte)0x6);
                    serverWriter.Close();
                    tcpClient.Close();
                    return;
                }

                File.WriteAllBytes($"Dump-{header.iSize}", data);
                Console.WriteLine($"CPU: {Utils.BytesToString(header.szCPU)}");
                Console.WriteLine($"HV: {Utils.BytesToString(header.szHypervisorCPU)}");

                if (header.Command == Packets.PACKET_CONNECT) {
                    // handle connection status
                    PacketConnect.Handle(dataReader, serverWriter, header, logId, IP);
                } else {
                    switch (header.Command) {
                        case Packets.PACKET_DOWNLOAD_PLUGIN:
                            PacketDownloadPlugin.Handle(dataReader, serverWriter, header, logId, IP);
                            break;
                    }
                }

            } catch(Exception ex) {
                serverWriter.Close();
                Console.WriteLine("Error: {0} - {1}", ex.Message, ex.StackTrace);
            }
            tcpClient.Close();
        }
    }
}
