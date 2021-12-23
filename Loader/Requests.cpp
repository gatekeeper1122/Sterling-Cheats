#include "stdafx.h"

int Requests::iPluginSize;

void Requests::PopulateHeader(Request::Header* header, Packets packet, int size) {
	header->iSize = size;
	header->Command = packet;

	memcpy(header->szCPU, Hypervisor::GetFuseCPU(), 0x10);
	memcpy(header->szHypervisorCPU, Hypervisor::GetHypervisorCPU(), 0x10);
}

void Requests::InitThread() {
	while (true) {
		if (Requests::PacketConnect()) {
			Sterling.bConnectedToServerInit = true;
			break;
		}

		LOG_PRINT("Sterling - Failed to connect. Check your internet connection!");
		Sleep(10000);
	}

	ExitThread(0);
}

BOOL Requests::PacketConnect() {
	DbgPrint("Running PacketConnect...\n");

	XNADDR xbXnaddr;
	DWORD dwAttempts = 0;

	// wait for the console to get a IP address
	while (XNetGetTitleXnAddr(&xbXnaddr) == XNET_GET_XNADDR_PENDING || XNetGetEthernetLinkStatus() == 0) {
		if (dwAttempts > 50)
			break;

		Sleep(250);
		dwAttempts++;
	}

	if (XNetGetTitleXnAddr(&xbXnaddr) == XNET_GET_XNADDR_PENDING || XNetGetEthernetLinkStatus() == 0) {
		return FALSE;
	}

	Network::Initialize();

	Request::ServerPacketConnect* packetConnect = (Request::ServerPacketConnect*)XEncryptedAlloc(sizeof(Request::ServerPacketConnect));
	PopulateHeader(packetConnect, PACKET_CONNECT, sizeof(Request::ServerPacketConnect));
	Response::ServerPacketConnect* packetConnectResponse = (Response::ServerPacketConnect*)XEncryptedAlloc(sizeof(Response::ServerPacketConnect));

	auto cleanup = [&] {
		XEncryptedFree(packetConnect);
		XEncryptedFree(packetConnectResponse);
	};

	int receivedBytes = 0;
	bool success; const char* error;

	Network network;

	network.Create(&success, &error);
	if (success) {
		network.Connect(SERVER_MAIN, &success, &error);
		if (success) {
			network.Send(packetConnect, sizeof(Request::ServerPacketConnect));
			network.Receive(packetConnect, packetConnectResponse, sizeof(Response::ServerPacketConnect), &receivedBytes, &success, &error);
			if (success) {
				if (receivedBytes == sizeof(Response::ServerPacketConnect)) {
					if (packetConnectResponse->bConnected) {
						cleanup();
						return TRUE;
					}
				}
			}
		}
	}

	cleanup();

	return FALSE;
}

BOOL Requests::PacketDownloadPlugin(DWORD TitleID, DWORD* pAddress, DWORD* pSize) {
	DbgPrint("Running PacketDownloadPluginSize...");

	Network::Initialize();

	Request::ServerPacketDownloadPlugin* packetDownloadPlugin = (Request::ServerPacketDownloadPlugin*)XEncryptedAlloc(sizeof(Request::ServerPacketDownloadPlugin));
	PopulateHeader(packetDownloadPlugin, PACKET_DOWNLOAD_PLUGIN, sizeof(Request::ServerPacketDownloadPlugin));
	Response::ServerPacketDownloadPlugin_1* packetDownloadPluginResponse = (Response::ServerPacketDownloadPlugin_1*)XEncryptedAlloc(sizeof(Response::ServerPacketDownloadPlugin_1));

	auto cleanup = [&] {
		XEncryptedFree(packetDownloadPlugin);
		XEncryptedFree(packetDownloadPluginResponse);
	};

	packetDownloadPlugin->bOnlySize = true;
	packetDownloadPlugin->iTitleid = TitleID;

	int receivedBytes = 0;
	bool success; const char* error;

	Network network;

	network.Create(&success, &error);
	if (success) {
		network.Connect(SERVER_MAIN, &success, &error);
		if (success) {
			network.Send(packetDownloadPlugin, sizeof(Request::ServerPacketDownloadPlugin));
			network.Receive(packetDownloadPlugin, packetDownloadPluginResponse, sizeof(Response::ServerPacketDownloadPlugin_1), &receivedBytes, &success, &error);
			if (success) {
				if (receivedBytes >= sizeof(Response::ServerPacketDownloadPlugin_1)) {
					switch (packetDownloadPluginResponse->Status) {
					case Response::DOWNLOAD_PLUGIN_STATUS_SUCCESS:
						iPluginSize = packetDownloadPluginResponse->iSize;
						cleanup();
						return PacketDownloadPluginBytes(TitleID, pAddress, pSize);

					case Response::DOWNLOAD_PLUGIN_STATUS_ERROR:
						error = "xbLive - Failed to download plugin!";
						break;
					}
				}
				else {
					error = Utils::va("Sterling - Expected %i bytes but got %i.", sizeof(Response::ServerPacketDownloadPlugin_1), receivedBytes);
				}
			}
		}
	}

	if (error) Utils::Notify(error);

	cleanup();

	return FALSE;
}

BOOL Requests::PacketDownloadPluginBytes(DWORD TitleID, DWORD* pAddress, DWORD* pSize) {
	LOG_DEV("Running PacketDownloadPluginBytes...");

	Network::Initialize();

	Request::ServerPacketDownloadPlugin* packetDownloadPlugin = (Request::ServerPacketDownloadPlugin*)XEncryptedAlloc(sizeof(Request::ServerPacketDownloadPlugin));
	PopulateHeader(packetDownloadPlugin, PACKET_DOWNLOAD_PLUGIN, sizeof(Request::ServerPacketDownloadPlugin));
	Response::ServerPacketDownloadPlugin_2* packetDownloadPluginResponse = (Response::ServerPacketDownloadPlugin_2*)XEncryptedAlloc(sizeof(Response::ServerPacketDownloadPlugin_2) + iPluginSize);

	auto cleanup = [&] {
		XEncryptedFree(packetDownloadPlugin);
		XEncryptedFree(packetDownloadPluginResponse);
	};

	packetDownloadPlugin->bOnlySize = false;
	packetDownloadPlugin->iTitleid = TitleID;

	int receivedBytes = 0;
	bool success; const char* error;

	Network network;

	network.Create(&success, &error);
	if (success) {
		network.Connect(SERVER_MAIN, &success, &error);
		if (success) {
			network.Send(packetDownloadPlugin, sizeof(Request::ServerPacketDownloadPlugin));
			network.Receive(packetDownloadPlugin, packetDownloadPluginResponse, sizeof(Response::ServerPacketDownloadPlugin_2) + iPluginSize, &receivedBytes, &success, &error);
			if (success) {
				if (receivedBytes == sizeof(Response::ServerPacketDownloadPlugin_2) + iPluginSize) {
					switch (packetDownloadPluginResponse->Status) {
					case Response::DOWNLOAD_PLUGIN_STATUS_SUCCESS: {
						if (iPluginSize > 100) {
							if (pSize) *pSize = iPluginSize;
							BYTE* xexData = (BYTE*)malloc(iPluginSize);
							memcpy(xexData, (BYTE*)((DWORD)packetDownloadPluginResponse + sizeof(Response::ServerPacketDownloadPlugin_2)), iPluginSize);

							if (pAddress) *pAddress = (DWORD)xexData;
						}
						else {
							error = "Sterling - Failed to load engine, plugin was too small!";
							break;
						}

						cleanup();
						return TRUE;
					}

					case Response::DOWNLOAD_PLUGIN_STATUS_ERROR:
						error = "Sterling - Failed to download plugin!";
						break;
					}
				}
				else {
					error = Utils::va("Sterling - Expected %i bytes but got %i.", sizeof(Response::ServerPacketDownloadPlugin_2), receivedBytes);
				}
			}
		}
	}

	if (error) Utils::Notify(error);

	cleanup();

	return FALSE;
}