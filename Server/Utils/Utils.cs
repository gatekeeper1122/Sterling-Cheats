using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Server {
    class Utils {
        public static string BytesToString(byte[] Buffer) {
            string str = "";
            for (int i = 0; i < Buffer.Length; i++) str = str + Buffer[i].ToString("X2");
            return str;
        }
        public static void SendPacket(EndianWriter serverWriter, byte[] dgram) {
            try {
                serverWriter.Write(dgram);
                serverWriter.Close();
            }
            catch (Exception ex) {
                Console.WriteLine("SendPacket Error: {0}", ex.Message);
            }
        }
    }
}
