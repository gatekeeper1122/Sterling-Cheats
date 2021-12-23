#include "stdafx.h"

bool Tools::CheckGame(DWORD TitleID) {
	return (XamGetCurrentTitleId() == TitleID);
}

void Tools::Notify(const char* text) {
	wchar_t* buf = (wchar_t*)calloc(1024, 1);
	wsprintfW(buf, L"%S", text);
	XNotifyQueueUI(XNOTIFYUI_TYPE_PREFERRED_REVIEW, 0, 2, buf, 0);
	free(buf);
}

color::color() {
	this->r = 0;
	this->g = 0;
	this->b = 0;
	this->a = 0;
}

color::color(float red, float green, float blue, float alpha) {
	this->r = red / 255.0f;
	this->g = green / 255.0f;
	this->b = blue / 255.0f;
	this->a = alpha / 255.0f;
}

color colorWhite = color(255, 255, 255, 255);
color colorBlack = color(0, 0, 0, 255);
color colorRed = color(255, 0, 0, 255);
color colorGreen = color(0, 255, 0, 255);
color colorBlue = color(0, 0, 255, 255);
color colorGrey = color(63.75, 63.75, 63.75, 235);
color colorGrey2 = color(50, 50, 50, 235);
color colorLightBlue = color(66, 135, 245, 255);