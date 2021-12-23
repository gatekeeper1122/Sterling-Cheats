#pragma once

#ifndef _HV_PEEKPOKE_H
#define _HV_PEEKPOKE_H

typedef unsigned __int64 QWORD;

static int dwExpansionID = 0x48565050;

enum PEEK_POKE_TYPE {
	PEEK_BYTE = 0,
	PEEK_WORD = 1,
	PEEK_DWORD = 2,
	PEEK_QWORD = 3,
	PEEK_BYTES = 4,
	POKE_BYTE = 5,
	POKE_WORD = 6,
	POKE_DWORD = 7,
	POKE_QWORD = 8,
	POKE_BYTES = 9,
	PEEK_SPR = 10
};

typedef struct _INSTALL_HEADER {
	DWORD dwSignature;				// 0x0 'HXPR' or 'HXPC', we've patched rtail hv to HXPR
	DWORD hashType;			// 0x4 0 = RotSumSha 1 = HmacSha 1x = cpukey crypt 0x = no cpukeycrypt
	DWORD fullSize;			// 0x8 includes header
	BYTE hash[0x14];		// 0xC CryptSha of [0x130 - (fullSize-0x130)]
	BYTE aesFeed[0x10];		// 0x20 when AES is used
	BYTE pkcs1data[0x100];	// @0x30-0x130, when verified gainst mfgPubKey rotsumsha or hmacsha of header is produced
							// code descriptor
	DWORD blockSig;			// 0x130 lets go for 'HVPP'
	DWORD keyStatus;		// 0x134 value can be gotten from 0x8e038610 keyStatus/hvRestrictedStatus mirrors, this check is patched out normally on retail
	DWORD ver1;				// 0x138 hv build ver (ie 17489) must be >= to this val, or this val must be 0
	DWORD ver2;				// 0x13c hv build ver (ie 17489) must be >= to this val, or this val must be 0
	DWORD unused1;			// 0x140
	DWORD unused2;			// 0x144
	QWORD restrictedPrivs;	// 0x148 value can be gotten from 0x8e038630, kvRestrictedPrivs, this check is patched out normally on retail
	DWORD unk4;				// 0x150 - just set to 0
	DWORD unk5;				// 0x154 - just set to 0
	DWORD srcOffset;		// 0x158 offset of code with header chunk (0x160)
	DWORD codeChunkSize;	// 0x15c size of code with the following header
							// code with header
	DWORD entryOffset;		// 0x160 offset - this value from here! = 0x10 entryOffset&0xF must be 0!
	DWORD headerSize;		// 0x164 size of this header (0x10) headerSize&0xF must be 0!, headerSize+codeSize should equal codeChunkSize
	DWORD codeSize;			// 0x168 size of code following header codeSize&0xF must be 0!
	DWORD dummy;			// 0x16A padding
	DWORD code[1];			// 0x170 just a convenient way to make an array for code insertion
} INSTALL_HEADER, * PINSTALL_HEADER;

class Hypervisor {
public:
	static NTSTATUS Initialize();

	static BYTE PeekBYTE(QWORD Address);
	static WORD PeekWORD(QWORD Address);
	static DWORD PeekDWORD(QWORD Address);
	static QWORD PeekQWORD(QWORD Address);
	static HRESULT PeekBytes(QWORD Address, PVOID Buffer, DWORD Size);

	static HRESULT PokeBYTE(QWORD Address, BYTE Value);
	static HRESULT PokeWORD(QWORD Address, WORD Value);
	static HRESULT PokeDWORD(QWORD Address, DWORD Value);
	static HRESULT PokeQWORD(QWORD Address, QWORD Value);
	static HRESULT PokeBytes(QWORD Address, const void* Buffer, DWORD Size);
	static QWORD GetFuseLine(BYTE fuseIndex);

	static BYTE* GetHypervisorCPU();
	static BYTE* GetFuseCPU();
	static BYTE* GetConsoleType();
};

#endif // _HV_PEEKPOKE_H