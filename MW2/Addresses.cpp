#include "stdafx.h"

addresses_s addrs;

void Addresses::LoadAddresses() {
	addrs.R_AddCmdDrawText = 0x82350278;
	addrs.R_AddCmdDrawStretchPic = 0x8234F9B8;
	addrs.R_TextWidth = 0x8234DD20;
	addrs.R_TextHeight = 0x8234DE10;
	addrs.R_RegisterFont = 0x82146458;

	addrs.Material_RegisterHandle = 0x8234E4F0;
	
	addrs.CG_DrawRotatedPicPhysical = 0x820F7900;

	addrs.FindDvar = 0x8229EE40;
	addrs.Dvar_GetBool = 0x8229EF58;

	addrs.Cbuf_AddText = 0x82224A28;

	addrs.DB_FindXAssetHeader = 0x8219BBC8;

	addrs.ScreenWidth = 0x83647880;
	addrs.ScreenHeight = 0x83647884;

	addrs.SND_EndFrame = 0x82351748;

	R_AddCmdDrawText = (void(*)(char *, int, int, float, float, float, float, float, color&, int))addrs.R_AddCmdDrawText;
	R_AddCmdDrawStretchPic = (void(*)(float x, float y, float width, float height, float xScale, float yScale, float xay, float yay, color& color, int material))addrs.R_AddCmdDrawStretchPic;
	R_TextWidth = (int(*)(const char*, int, int))addrs.R_TextWidth;
	R_TextHeight = (int(*)(int))addrs.R_TextHeight;

	Material_RegisterHandle = (int(*)(char*, int))addrs.Material_RegisterHandle;

	CG_DrawRotatedPicPhysical = (void(*)(int, float, float, float, float, float, color&, int))addrs.CG_DrawRotatedPicPhysical;

	DB_FindXAssetHeader = (DWORD(*)(DWORD, PCHAR, DWORD))addrs.DB_FindXAssetHeader;
}