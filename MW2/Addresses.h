#ifndef __ADDRESSES_H__
#define __ADDRESSES_H__

struct addresses_s {
	DWORD R_AddCmdDrawText;
	DWORD R_AddCmdDrawStretchPic;
	DWORD R_TextWidth;
	DWORD R_TextHeight;
	DWORD R_RegisterFont;

	DWORD Material_RegisterHandle;

	DWORD CG_DrawRotatedPicPhysical;

	DWORD FindDvar;
	DWORD Dvar_GetBool;

	DWORD Cbuf_AddText;

	DWORD DB_FindXAssetHeader;

	DWORD ScreenWidth;
	DWORD ScreenHeight;

	DWORD SND_EndFrame;
};

class Addresses {
public:
	static void LoadAddresses();
};

extern addresses_s addrs;

#endif // __ADDRESSES_H__