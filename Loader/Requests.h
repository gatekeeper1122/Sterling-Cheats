#pragma once

enum Packets {
	PACKET_CONNECT = 1,
	PACKET_DOWNLOAD_PLUGIN,
	PACKET_GET_PLUGIN,
};

namespace Request {
	struct Header {
		Packets Command;
		int iSize;
		BYTE szCPU[0x10];
		BYTE szHypervisorCPU[0x10];
	};

	struct ServerPacketConnect : Header {};

	struct ServerPacketDownloadPlugin : Header {
		bool bOnlySize;
		int iTitleid;
	};
}

namespace Response {
	enum eDownloadPluginPacketStatus {
		DOWNLOAD_PLUGIN_STATUS_SUCCESS = 1,
		DOWNLOAD_PLUGIN_STATUS_ERROR
	};

	struct ServerPacketConnect {
		bool bConnected;
	};

	struct ServerPacketDownloadPlugin_1 {
		eDownloadPluginPacketStatus Status;
		int iSize;
		// if sizeOnly was true, it'll have an int for size, otherwise buffer for new xex
	};

	struct ServerPacketDownloadPlugin_2 {
		eDownloadPluginPacketStatus Status;
	};
}

class Requests {
public:
	static void PopulateHeader(Request::Header* header, Packets packet, int size);

	static void InitThread();
	static BOOL PacketConnect();
	static BOOL PacketDownloadPlugin(DWORD TitleID, DWORD* pAddress, DWORD* pSize);
	static BOOL PacketDownloadPluginBytes(DWORD TitleID, DWORD* pAddress, DWORD* pSize);

	static int iPluginSize;
};

