#include "../inc/MainMenu.h"
#include "../inc/MainGame.h"
// stevens code

MainMenu*   MainMenu::activeMenu = nullptr;

MainMenu::MainMenu(float width, float height, MainGame *mainGame, bool fullScreen, bool resizable)
{
	initMenu(width, height, mainGame, fullScreen, resizable);
}

MainMenu::MainMenu(const MainMenu &rhs)
{
	*this = rhs;
}

MainMenu& MainMenu::operator=(const MainMenu &rhs)
{
	if (this != &rhs)
	{
		_screen = rhs._screen;
		_startGameMenu = rhs._startGameMenu;
		_startMenu = rhs._startMenu;
		_pauseGameMenu = rhs._pauseGameMenu;
		_mainGame = rhs._mainGame;
	}
	return *this;
}

bool MainMenu::initMenu(float width, float height, MainGame *mainGame, bool fullScreen, bool resizable)
{
	_mainGame = mainGame;
	MainMenu::activeMenu = this;

	_screen = new nanogui::Screen({width, height}, "Bomberman", resizable, fullScreen,
		8, 8, 24, 8, 4, 4, 1);

    _screen->setVisible(true);
	std::vector<void *> params;
	params.push_back(this);
	//MainGame::functions.insert(std::pair<const char *, Func>("menuUpdate", {Menu::updateMenu, params})); 
	return true;
}

bool MainMenu::buildMenuWindows(float width, float height)
{
	_createStartMenu(width, height);
	_createStartGameMenu(width, height);
	_createPauseGameMenu(width, height);
//	_createBackground(width, height); useing opengl to create background
    _createExitWindow(width, height);
    //_createVivtoryWindow(width, height);
	return true;
}

void MainMenu::_createStartMenu(float width, float height)
{
	(void)width;
	(void)height;
	_startMenu = new nanogui::Window(_screen, "Menu");
	_startMenu->setLayout(new nanogui::GroupLayout());
	_startMenu->setWidth(250);
	_startMenu->setHeight(250);
	_startMenu->setVisible(true);
	_startMenu->center();
	/// start game button
	nanogui::Button *startButton = new nanogui::Button(_startMenu, "Start Game");
	startButton->setPosition({50, 40});
    startButton->setSize({100, 50});
    // what the button will do
	startButton->setCallback([]{
		activeMenu->_mainGame->setGameState(GAMESTATE::PLAY);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//		activeMenu->_scene = new Scene(activeMenu->_mainGame, activeMenu->_mainGame->getMap("map1"), 5);
	});
	/// options button
	nanogui::Button *optionsButton = new nanogui::Button(_startMenu, "Options");
	optionsButton->setPosition({50, 110});
	optionsButton->setSize({150, 50});
	/// exit button
	nanogui::Button *exitButton = new nanogui::Button(_startMenu, "Exit");
	exitButton->setPosition({50, 180});
	exitButton->setSize({150, 50});
	exitButton->setCallback([]{ MainMenu::exitButtonCallBack();});
}

void MainMenu::_createStartGameMenu(float width, float height)
{
	(void)width;
	(void)height;
	_startGameMenu = new nanogui::Window(_screen, "Start Game");
	_startGameMenu->setVisible(false);
}

void MainMenu::_createPauseGameMenu(float width, float height)
{
	(void)width;
	(void)height;
	_pauseGameMenu = new nanogui::Window(_screen, "Pause");
	_pauseGameMenu->setVisible(false);
}

void MainMenu::_createExitWindow(float width, float height)
{
	(void)width;
	(void)height;
	_exitWindow = new nanogui::Window(_screen, "");
	_exitWindow->setLayout(new nanogui::GroupLayout());
	_exitWindow->setWidth(400);
	_exitWindow->setHeight(150);
	_exitWindow->setVisible(false);
	_exitWindow->center();
	/// text label
	nanogui::Label *label = new nanogui::Label(_exitWindow, "Do you want to exit game?", "sans", 30);
	label->setPosition({70, 20});
	label->setSize({300, 30});
	/// yes button
	nanogui::Button *yes = new nanogui::Button(_exitWindow, "Yes");
	yes->setSize({70, 30});
	yes->setPosition({80, 90});
	yes->setCallback([]{
		glfwSetWindowShouldClose(activeMenu->_screen->glfwWindow(), (int)true);
	});
	/// no button
	nanogui::Button *no = new nanogui::Button(_exitWindow, "No");
	no->setSize({70, 30});
	no->setPosition({250, 90});
	no->setCallback([]{
		activeMenu->_startMenu->setVisible(true);
		activeMenu->_exitWindow->setVisible(false);
	});
}

/* how stephen made the background
void MainMenu::_createBackground(float width, float height)
{
	_menuBg = new Zion::SquareSprite(*_mainGame->getShader("basic"), 0, 0, 8, 5);
	_menuBg->addTextureFromFile("resource/images/menu_bg.jpg");
	
	_menuTitle = new Zion::SquareSprite(*_mainGame->getShader("basic"), 0, 0, 3, 1.5);
	_menuTitle->addTextureFromFile("resource/images/title1.png");
}
*/


void MainMenu::updateMenu(MainGame *game, std::vector<void *> params)
{
	(void)game;
	auto *menu = (MainMenu*)params[0];
	
	GAMESTATE state = menu->_mainGame->getGameState();
	
	if (state == GAMESTATE::MENU)
	{
	//	menu->_menuBg->render(glm::translate(glm::mat4(), {0, 0, -1}));
	//	menu->_menuTitle->render(glm::translate(glm::mat4(), {0, 1, 0}));
		menu->_screen->drawWidgets();
	}
//	if (state == GAMESTATE::PAUSE)
//	{
//		menu->_menuBg->render(glm::translate(glm::mat4(), {0, 0, -1}));
//	}
}

bool MainMenu::mouseCallback(int button, int action, int mod)
{
	if (activeMenu != nullptr)
	activeMenu->_screen->mouseButtonCallbackEvent(button, action, mod);
	return true;
}

bool MainMenu::cursorPositionCallback(int xpos, int ypos)
{
	if (activeMenu != nullptr)
	activeMenu->_screen->cursorPosCallbackEvent(xpos, ypos);
	return true;
}

bool MainMenu::keyCallback(int key, int scancode, int action, int mods)
{
	if (activeMenu != nullptr)
	activeMenu->_screen->keyCallbackEvent(key, scancode, action, mods);
	return true;
}

void MainMenu::exitButtonCallBack()
{
	activeMenu->_startMenu->setVisible(false);
	activeMenu->_exitWindow->setVisible(true);
}

GLFWwindow* MainMenu::getGlfwWindow()
{
	return _screen->glfwWindow();
}