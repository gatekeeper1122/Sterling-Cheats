#include "stdafx.h"

void(*R_AddCmdDrawText)(char *text, int maxChars, int font, float x, float y, float xScale, float yScale, float rotation, color& color, int style);
void(*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, color& color, int material);
int(*R_TextWidth)(const char* text, int maxChars, int font);
int(*R_TextHeight)(int font);

int(*Material_RegisterHandle)(char* name, int imageTrack);

void (*CG_DrawRotatedPicPhysical)(int scrPlace, float x, float y, float width, float height, float angle, color& color, int material);


DWORD(*DB_FindXAssetHeader)(DWORD Type, PCHAR Name, DWORD r5);