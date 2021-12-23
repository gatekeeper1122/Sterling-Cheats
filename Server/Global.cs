namespace Server {
    class Global {
        public static int iPort = 17559;
        public static int iMaximumRequestSize = 0x1000;
    }

    public enum Packets {
        PACKET_CONNECT = 1,
        PACKET_DOWNLOAD_PLUGIN,
        PACKET_GET_PLUGINS,
    }

    public struct Header {
        public Packets Command;
        public int iSize;

        public byte[] szCPU; // 0x10
        public byte[] szHypervisorCPU; // 0x10
    }
}
