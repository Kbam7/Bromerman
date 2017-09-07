#pragma once

#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/label.h>
#include <nanogui/textbox.h>
#include "MainGame.h"


class   MainMenu
{
private:
	nanogui::Screen     *_screen;
	nanogui::Window     *_startMenu;
	nanogui::Window     *_startGameMenu;
	nanogui::Window     *_pauseGameMenu;
	nanogui::Window     *_exitWindow;
	MainGame            *_mainGame;


// create everything we need
	void    _createStartMenu(float width, float height);
	void    _createStartGameMenu(float width, float height);
	void    _createPauseGameMenu(float width, float height);
//	void    _createSettingsMenu(float width, float height);
//	void    _createBackground(float width, float height);
	void    _createExitWindow(float width, float height);

public:
	MainMenu() = default;
	MainMenu(float width, float height, MainGame *mainGame, bool fullScreen = false, bool resizable = false);
	MainMenu(const MainMenu & rhs);
	MainMenu&   operator=(const MainMenu & rhs);
	~MainMenu() = default;

	bool    initMenu(float width, float height, MainGame *mainGame, bool fullScreen = false, bool resizable = false);
	bool    startMenu();
	bool    startGameMenu();
	bool    buildMenuWindows(float width, float height);

	static MainMenu    *activeMenu;
	static void    updateMenu(MainGame *game, std::vector<void *> params);
	static void    exitButtonCallBack();
	/// creating callbacks for menu
	static bool     mouseCallback(int button, int action, int mod);
	static bool     cursorPositionCallback(int xpos, int ypos);
	static bool     keyCallback(int key, int scancode, int action, int mods);

	GLFWwindow  *getGlfwWindow(void);
}; 