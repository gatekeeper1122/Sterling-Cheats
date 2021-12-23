#ifndef __MENU_H__
#define __MENU_H__

enum Submenu_s {
	CLOSED,
	MAIN_MENU,
	AIMBOT_MENU,
	VISUAL_MENU,
	CLIENT_LIST,
	SETTINGS_MENU
};

struct UIItems_s {
	int currentOption;
	int maxOptions;
	int optionCount;
	int submenu;
	int submenuLevel;
	int lastSubmenu[20];
	int lastOption[20];
	bool optionPress;
	bool leftPress;
	bool rightPress;
};

class Menu {
private:
	char* option;
	char* font;

	char temp[50];
public:

	void ResetVars();
	int getOption();
	Menu(char* optionText, char* fontName = Functions::GetFont());
	void AddOption(char* optionName, char* fontName);
	inline Menu Option();
	inline Menu Toggle(bool* toChange);
	inline Menu ToggleBox(bool* toChange);

	static void HandleInput();
	static void HandleMenu();
	static void UpdateMenu();
};

extern UIItems_s UIItems;

#endif // __MENU_H__