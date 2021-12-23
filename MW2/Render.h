#ifndef __RENDER_H__
#define __RENDER_H__

class Render {
public:
	static void DrawText(char* text, float x, float y, char* fontName, float fontScale, color color);
	static void DrawShader(float x, float y, float width, float height, char* materials, color color);
	static void DrawShaderWithOutline(float x, float y, float w, float h, color backgroundColor, color outlineColor, float lineWidth);
	static void DrawCenteredText(char* text, float x, float y, float width, char* fontName, float fontScale, color color);
	static void DrawLeftSidedText(char* text, float x, float y, float width, char* fontName, float fontScale, color color);
	static void CheckBox(float centerX, float centerY, color checkColor, color outlineBoxColor, bool isToggled);
	static void DrawLine(vec2_t start, vec2_t end, color color, float size);
	static void DrawHearth(float x, float y, float w, float h, color outline, float* fill);
};

#endif // __RENDER_H__