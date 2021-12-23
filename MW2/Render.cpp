#include "stdafx.h"

void Render::DrawText(char* text, float x, float y, char* fontName, float fontScale, color color) {
	int font = DB_FindXAssetHeader(0x16, fontName, 1);

	R_AddCmdDrawText(text, 0x7FFFFFFF, font, x, y, fontScale, fontScale, 0, color, 0);
}

void Render::DrawShader(float x, float y, float width, float height, char* materials, color color) {
	R_AddCmdDrawStretchPic(x, y, width, height, 1.0f, 1.0f, 1.0f, 1.0f, color, DB_FindXAssetHeader(0x05, materials, 1));
}

void Render::DrawShaderWithOutline(float x, float y, float w, float h, color backgroundColor, color outlineColor, float lineWidth) {
	DrawShader(x, y, lineWidth, h, "white", outlineColor);
	DrawShader(x + lineWidth, y, w - lineWidth, lineWidth, "white", outlineColor);
	DrawShader(x + lineWidth, y + h - lineWidth, w - (lineWidth * 2), lineWidth, "white", outlineColor);
	DrawShader(x + w - lineWidth, y + lineWidth, lineWidth, h - lineWidth, "white", outlineColor);

	DrawShader(x + lineWidth, y + lineWidth, w - (lineWidth * 2), h - (lineWidth * 2), "white", backgroundColor);
}

void Render::DrawCenteredText(char* text, float x, float y, float width, char* fontName, float fontScale, color color) {
	int font = DB_FindXAssetHeader(0x16, fontName, 1);
	int textLength = ((R_TextWidth(text, 0x7FFFFFFF, font) / 2) * fontScale);

	R_AddCmdDrawText(text, 0x7FFFFFFF, font, (x + (width / 2) - textLength), y, fontScale, fontScale, 0, color, 0);
}

void Render::DrawLeftSidedText(char* text, float x, float y, float width, char* fontName, float fontScale, color color) {
	int font = DB_FindXAssetHeader(0x16, fontName, 1);
	int textLength = ((R_TextWidth(text, 0x7FFFFFFF, font) / 2) * fontScale);

	R_AddCmdDrawText(text, 0x7FFFFFFF, font, ((x + width) - textLength - 20), y, fontScale, fontScale, 0, color, 0);
}

void Render::CheckBox(float centerX, float centerY, color checkColor, color outlineBoxColor, bool isToggled) {
	DrawShader(centerX, centerY, 12, 1, "white", outlineBoxColor);
	DrawShader(centerX, centerY + 12, 12, 1, "white", outlineBoxColor);
	DrawShader(centerX, centerY, 1, 12, "white", outlineBoxColor);
	DrawShader(centerX + 12, centerY, 1, 13, "white", outlineBoxColor);

	if (isToggled)
		DrawShader(centerX + 1, centerY + 1, 11, 11, "white", checkColor);
}

void Render::DrawLine(vec2_t start, vec2_t end, color color, float size)
{
	vec2_t  delta = start - end;
	vec_t angle = atan2(delta.y, delta.x) * (180 / 3.141592654f);
	vec_t length = delta.Length();
	vec2_t  coords(end.x + ((delta.x - length) / 2), end.y + (delta.y / 2));
	//CG_DrawRotatedPicPhysical(MP_Scr_Placment, coords.x, coords.y, length, size, angle, color, Material_RegisterHandle("white", 7));
}

void Render::DrawHearth(float x, float y, float w, float h, color outline, float* fill) {
	{
		DrawLine(vec2_t(x + (w / 100 * 0), y + (h / 100 * 45)), vec2_t(x + (w / 100 * 10), y + (h / 100 * 18)), outline, 2);
		DrawLine(vec2_t(x + (w / 100 * 100), y + (h / 100 * 45)), vec2_t(x + (w / 100 * 90), y + (h / 100 * 18)), outline, 2);
		DrawLine(vec2_t(x + (w / 100 * 10), y + (h / 100 * 18)), vec2_t(x + (w / 100 * 30), y + (h / 100 * 9)), outline, 2);
		DrawLine(vec2_t(x + (w / 100 * 90), y + (h / 100 * 18)), vec2_t(x + (w / 100 * 70), y + (h / 100 * 9)), outline, 2);
		DrawLine(vec2_t(x + (w / 100 * 30), y + (h / 100 * 9)), vec2_t(x + (w / 100 * 50), y + (h / 100 * 20)), outline, 2);
		DrawLine(vec2_t(x + (w / 100 * 70), y + (h / 100 * 9)), vec2_t(x + (w / 100 * 50), y + (h / 100 * 20)), outline, 2);
		DrawLine(vec2_t(x + (w / 100 * 0), y + (h / 100 * 45)), vec2_t(x + (w / 100 * 50), y + (h / 100 * 93)), outline, 2);
		DrawLine(vec2_t(x + (w / 100 * 100), y + (h / 100 * 45)), vec2_t(x + (w / 100 * 50), y + (h / 100 * 93)), outline, 2);
	}
}