#include "stdafx.h"

UIItems_s UIItems;

float centerX, centerY, MenuWidth = 280.0f;

void Menu::ResetVars() {
	UIItems.optionPress = false;
	UIItems.leftPress = false;
	UIItems.rightPress = false;
}

int Menu::getOption() {
	if (UIItems.optionPress)
		return UIItems.currentOption;
	else
		return 0;
}

Menu::Menu(char* optionText, char* fontName) {
	this->option = optionText;
	this->font = fontName;
}

void Menu::AddOption(char* optionName, char* fontName) {
	UIItems.optionCount++;

	if (UIItems.currentOption <= UIItems.maxOptions && UIItems.optionCount <= UIItems.maxOptions) {
		if (UIItems.currentOption == UIItems.optionCount) {
			Render::DrawShader(centerX + 100, ((centerY - 73) + (UIItems.optionCount * 25)), MenuWidth, 18, "white", colorRed);
		}

		Render::DrawText(optionName, (centerX + 110), ((centerY - 55) + (UIItems.optionCount * 25)), fontName, 0.75f, colorWhite);
	}
	else if ((UIItems.optionCount > (UIItems.currentOption - UIItems.maxOptions)) && UIItems.optionCount <= UIItems.currentOption) {
		if (UIItems.currentOption == UIItems.optionCount) {
			Render::DrawShader(centerX + 100, ((centerY - 73) + ((UIItems.optionCount - (UIItems.currentOption - UIItems.maxOptions)) * 25)), MenuWidth, 18, "white", colorRed);

		}

		Render::DrawText(optionName, (centerX + 110), ((centerY - 55) + ((UIItems.optionCount - (UIItems.currentOption - UIItems.maxOptions)) * 25)), fontName, 0.75f, colorWhite);
	}
}

inline Menu Menu::Option() {
	AddOption(this->option, this->font);
	return *this;
}

inline Menu Menu::Toggle(bool* toChange) {
	AddOption(this->option, this->font);

	Render::DrawLeftSidedText(*toChange ? "ON" : "OFF", centerX + 100, (centerY - 50 + (UIItems.optionCount * 25) - 5), MenuWidth, this->font, 0.8f, UIItems.optionCount == UIItems.currentOption ? colorWhite : colorRed);

	if (UIItems.currentOption == UIItems.optionCount && UIItems.optionPress) {
		*toChange = !*toChange;
	}

	return *this;
}

inline Menu Menu::ToggleBox(bool* toChange) {
	AddOption(this->option, this->font);

	if (UIItems.currentOption <= UIItems.maxOptions && UIItems.optionCount <= UIItems.maxOptions)
		Render::CheckBox((centerX + 100 + 260.0f), ((centerY - 71) + (UIItems.optionCount * 25)), colorWhite, colorWhite, *toChange);
	else if ((UIItems.optionCount > (UIItems.currentOption - UIItems.maxOptions)) && UIItems.optionCount <= UIItems.currentOption)
		Render::CheckBox((centerX + 100 + 260.0f), ((centerY - 71) + ((UIItems.optionCount - (UIItems.currentOption - UIItems.maxOptions)) * 25)), colorWhite, colorWhite, *toChange);

	if (UIItems.currentOption == UIItems.optionCount && UIItems.optionPress) {
		*toChange = !*toChange;
	}
	return *this;
}

bool buttonFinished;
void Menu::HandleInput() {
	XINPUT_STATE state;
	XInputGetState(0, &state);
	short button = state.Gamepad.wButtons;

	if (!buttonFinished)
	{
		if (button & XINPUT_GAMEPAD_DPAD_LEFT && state.Gamepad.bLeftTrigger >= 100) {
			if (UIItems.submenu == CLOSED) {
				UIItems.lastSubmenu[UIItems.submenuLevel] = UIItems.submenu;
				UIItems.lastOption[UIItems.submenuLevel] = UIItems.currentOption;
				UIItems.currentOption = 1;
				UIItems.submenu = MAIN_MENU;
				UIItems.submenuLevel++;
				UIItems.submenuLevel = 0;
				UIItems.currentOption = 1;
			}
			else {
				UIItems.submenu = CLOSED;
			}

			buttonFinished = true;
		}

		if (button != 0 && UIItems.submenu != CLOSED) {

			if (button & XINPUT_GAMEPAD_LEFT_SHOULDER) {
				if (UIItems.submenu == MAIN_MENU || UIItems.submenu == AIMBOT_MENU || UIItems.submenu == VISUAL_MENU || UIItems.submenu == CLIENT_LIST || UIItems.submenu == SETTINGS_MENU) {
					UIItems.submenu == MAIN_MENU ? UIItems.submenu = SETTINGS_MENU : UIItems.submenu--;
					UIItems.currentOption = 1;
				}
			}

			if (button & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
				if (UIItems.submenu == MAIN_MENU || UIItems.submenu == AIMBOT_MENU || UIItems.submenu == VISUAL_MENU || UIItems.submenu == CLIENT_LIST || UIItems.submenu == SETTINGS_MENU) {
					UIItems.submenu == SETTINGS_MENU ? UIItems.submenu = MAIN_MENU : UIItems.submenu++;
					UIItems.currentOption = 1;
				}
			}

			if (button & XINPUT_GAMEPAD_A) {
				UIItems.optionPress = true;
			}
			else if (button & XINPUT_GAMEPAD_DPAD_UP) {
				UIItems.currentOption--;
				UIItems.currentOption = UIItems.currentOption < 1 ? UIItems.optionCount : UIItems.currentOption;
			}
			else if (button & XINPUT_GAMEPAD_DPAD_DOWN) {
				UIItems.currentOption++;
				UIItems.currentOption = UIItems.currentOption > UIItems.optionCount ? 1 : UIItems.currentOption;
			}
			else if (button & XINPUT_GAMEPAD_DPAD_LEFT) {
				UIItems.leftPress = true;
			}
			else if (button & XINPUT_GAMEPAD_DPAD_RIGHT) {
				UIItems.rightPress = true;
			}

			buttonFinished = true;
		}
	}

	if (button == 0 && buttonFinished)
		buttonFinished = false;
}

char* SubMenus[5] = { "Main", "Aimbot", "Visual", "Client List", "Settings" };

bool nosway = false;

void Menu::HandleMenu() {
	centerX = ((*(int*)(addrs.ScreenWidth) / 2) - (MenuWidth / 2));
	centerY = ((*(int*)(addrs.ScreenHeight) / 2) - (((UIItems.maxOptions * 25) + 22) / 2));

	if (UIItems.submenu > CLOSED) {
		/* Background with Border */
		Render::DrawShaderWithOutline(centerX + 100, centerY - 100, 280.0f, 500, colorGrey, colorRed, 2);

		Render::DrawShader(centerX + 100, centerY - 50, 280.0f, 2, "white", colorRed);

		Render::DrawCenteredText("Sterling Host", centerX + 100, centerY - 60, 280.0f, Functions::GetFont(), 1.0f, colorRed);
	}

	UIItems.optionCount = 0;

	switch (UIItems.submenu)  {
		case MAIN_MENU:
			Menu("No Sway").ToggleBox(&nosway);
			Menu("No Recoil").Toggle(&nosway);
			Menu("No Sway").ToggleBox(&nosway);
			Menu("No Recoil").Toggle(&nosway);
			Menu("No Sway").ToggleBox(&nosway);
			Menu("No Recoil").Toggle(&nosway);
			Menu("No Sway").ToggleBox(&nosway);
			Menu("No Recoil").Toggle(&nosway);
			Menu("No Sway").ToggleBox(&nosway);
			Menu("No Recoil").Toggle(&nosway);
			break;

		case AIMBOT_MENU:
			break;

		case VISUAL_MENU:
			break;

		case CLIENT_LIST:
			break;

		case SETTINGS_MENU:
			break;
	}
	Menu(NULL).ResetVars();
}

void Menu::UpdateMenu() {
	UIItems.maxOptions = 10;

	HandleInput();
	HandleMenu();
}