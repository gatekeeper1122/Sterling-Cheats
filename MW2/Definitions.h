#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

extern void(*R_AddCmdDrawText)(char *text, int maxChars, int font, float x, float y, float xScale, float yScale, float rotation, color& color, int style);
extern void(*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, color& color, int material);
extern int(*R_TextWidth)(const char* text, int maxChars, int font);
extern int(*R_TextHeight)(int font);

extern int(*Material_RegisterHandle)(char* name, int imageTrack);

extern void (*CG_DrawRotatedPicPhysical)(int scrPlace, float x, float y, float width, float height, float angle, color& color, int material);

extern DWORD(*DB_FindXAssetHeader)(DWORD Type, PCHAR Name, DWORD r5);

#endif // __DEFINITIONS_H__