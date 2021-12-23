#pragma once

struct ServerXexInfo {
	int iID;
	DWORD dwLastVersion;
	DWORD dwTitle;
	DWORD dwTitleTimestamp;
};

class Global {
public:
	HANDLE hMyModule;
	PLDR_DATA_TABLE_ENTRY pDataTableEntry;
	PLDR_DATA_TABLE_ENTRY LatestTitle;
	bool bLoadedProperly;
	bool bConnectedToServerInit;

	/*Title plugin info*/
	vector<ServerXexInfo> PluginData;
};

extern Global Sterling;

