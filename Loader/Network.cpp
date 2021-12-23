#include "stdafx.h"

HRESULT Network::Initialize() {
	XNetStartupParams xnsp;
	xnsp.cfgSizeOfStruct = sizeof(XNetStartupParams);
	xnsp.cfgFlags = XNET_STARTUP_BYPASS_SECURITY;

	if (NetDll_XNetStartup(XNCALLER_SYSAPP, &xnsp) != S_OK) {
		LOG_PRINT("Failed to start XNet");
		return S_FAIL;
	}

	WSADATA wsaData;
	if (NetDll_WSAStartupEx(XNCALLER_SYSAPP, MAKEWORD(0x2, 0x2), &wsaData, 0x2) != S_OK) {
		LOG_PRINT("Failed to start WSA");
		return S_FAIL;
	}

	return S_OK;
}

Network& Network::Create(bool* pbSuccessful, const char** ppError) {
	bool success = true;
	for (int i = 0; i < 10; i++) {
		SocketHandle = NetDll_socket(XNCALLER_SYSAPP, AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (SocketHandle == SOCKET_ERROR) {
			success = false;
			LOG_PRINT("Failed to create socket, attempt %i to reconnect. Status: %X", i + 1, GetLastError());
		}
		else {
			success = true;
			break;
		}

		Sleep(100);
	}

	if (!success) {
		if (pbSuccessful) *pbSuccessful = false;
		if (ppError) *ppError = "Failed to create socket to server after 10 tries.";
		return *this;
	}

	if (pbSuccessful) *pbSuccessful = true;

	return *this;
}

Network& Network::Connect(eServer server, bool* pbSuccessful, const char** ppError) {
	BOOL sockOpt = TRUE;
	NetDll_setsockopt(XNCALLER_SYSAPP, SocketHandle, SOL_SOCKET, 0x5801, (const char*)&sockOpt, 4);

	DWORD sendRecvSize = 0x100000;
	NetDll_setsockopt(XNCALLER_SYSAPP, SocketHandle, SOL_SOCKET, SO_SNDBUF, (const char*)&sendRecvSize, 0x04);
	NetDll_setsockopt(XNCALLER_SYSAPP, SocketHandle, SOL_SOCKET, SO_RCVBUF, (const char*)&sendRecvSize, 0x04);

	DWORD timeout = 10 * 1000;
	NetDll_setsockopt(XNCALLER_SYSAPP, SocketHandle, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

	ServerHandle.sin_family = AF_INET;

	ServerHandle.sin_addr.S_un.S_un_b.s_b1 = 192;
	ServerHandle.sin_addr.S_un.S_un_b.s_b2 = 168;
	ServerHandle.sin_addr.S_un.S_un_b.s_b3 = 178;
	ServerHandle.sin_addr.S_un.S_un_b.s_b4 = 58;
	ServerHandle.sin_port = htons(17559);

	bool success = true;
	for (int i = 0; i < 10; i++) {
		if (NetDll_connect(XNCALLER_SYSAPP, SocketHandle, (struct sockaddr*)&ServerHandle, sizeof(sockaddr_in)) == SOCKET_ERROR) {
			success = false;
			LOG_PRINT("Failed to connect to socket, attempt %i to reconnect. Status: %X", i + 1, GetLastError());
		}
		else {
			success = true;
			break;
		}

		Sleep(1000);
	}

	if (!success) {
		NetDll_closesocket(XNCALLER_SYSAPP, SocketHandle);
		if (pbSuccessful) *pbSuccessful = false;
		if (ppError) *ppError = "Failed to connect to server after 10 tries.";
		return *this;
	}

	if (pbSuccessful) *pbSuccessful = true;

	return *this;
}

Network& Network::SetSocketOption(int name, const char* value, int size) {
	NetDll_setsockopt(XNCALLER_SYSAPP, SocketHandle, SOL_SOCKET, name, value, size);
	return *this;
}

Network& Network::Send(void* pBuffer, int size) {
	NetDll_send(XNCALLER_SYSAPP, SocketHandle, (char*)pBuffer, size, 0);
	return *this;
}

Network& Network::Receive(Request::Header* header, void* pRecvBuffer, int size, int* pReadBytes, bool* pbSuccessful, const char** ppError) {
	char* receivedBuffer = new char[size];

	DWORD remaining = size;
	DWORD received = 0;

	while (remaining > 0) {
		DWORD recvsize = 0x1000 < remaining ? 0x1000 : remaining;
		DWORD receive = NetDll_recv(XNCALLER_SYSAPP, SocketHandle, (char*)receivedBuffer + received, recvsize, 0);

		if (receive == SOCKET_ERROR) {
			if (pbSuccessful) *pbSuccessful = false;
			if (ppError) *ppError = "Failed receiving payload from server, please tell an owner.";
			NetDll_closesocket(XNCALLER_SYSAPP, SocketHandle);
			return *this;
		}

		if (receive == 0) break;

		remaining -= receive;
		received += receive;
	}

	NetDll_closesocket(XNCALLER_SYSAPP, SocketHandle);

	if (pReadBytes) *pReadBytes = received;

	delete[] receivedBuffer;
	return *this;
}