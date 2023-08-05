#include"Game.h"

using namespace Game;

Menu::Menu()
{
	this->strat_filename = "Media/ASCII/menu_start.txt";
	this->help_filename = "Media/ASCII/menu_help.txt";
	this->exit_filename = "Media/ASCII/menu_exit.txt";
	this->return_menu_filename = "Media/ASCII/game_over_menu.txt";
	this->over_exit_filename = "Media/ASCII/game_over_exit.txt";
	screen = new char[screen_height * screen_width];
	help_screen = new char[screen_width * screen_height];
}
Menu::~Menu()
{
	delete[] screen;
	delete[] help_screen;
}

//更新且渲染开始菜单菜单
void Menu::UpdateAndRenderMenu()
{
	static int menu_state = GameState::StartMenu::GS_StartSelect;

	switch (menu_state)
	{
	case GameState::StartMenu::GS_StartSelect:
	{
		//按下回车键进入游戏
		if (IS_KEY_DOWN(VK_RETURN))
		{
			//进入游戏前停止播放菜单音乐
			mciSendString(L"stop Media/music/menu_bgm.mp3", 0, 0, 0);
			mciSendString(L"open Media/music/game_bgm.mp3", 0, 0, 0);
			mciSendString(L"play Media/music/game_bgm.mp3 repeat", 0, 0, 0);
			MCIDEVICEID wDeviceID = NULL;
			is_play_menu_music = false;
			game_state = GameState::GS_MainGame;
			main_game_state = GameState::MainGame::GS_GameInit;
			Sleep(200);
		}
		if (IS_KEY_DOWN(0x53))
		{
			menu_state = GameState::StartMenu::GS_HelpSelect;
			Sleep(200);
		}
		ShowStartMenu();
	}break;
	case GameState::StartMenu::GS_HelpSelect:
	{
		//按下回车键进入帮助
		if (IS_KEY_DOWN(VK_RETURN))
		{
			menu_state = GameState::StartMenu::GS_HelpMenu;
			Sleep(200);
		}
		if (IS_KEY_DOWN(0x57))
		{
			menu_state = GameState::StartMenu::GS_StartSelect;
			Sleep(200);
		}
		if (IS_KEY_DOWN(0x53))
		{
			menu_state = GameState::StartMenu::GS_ExitSelect;
			Sleep(200);
		}
		ShowHelpMenu();
	}break;
	case GameState::StartMenu::GS_ExitSelect:
	{
		//按下回车键退出游戏
		if (IS_KEY_DOWN(VK_RETURN))
		{
			exit(0);
		}
		if (IS_KEY_DOWN(0x57))
		{
			menu_state = GameState::StartMenu::GS_HelpSelect;
			Sleep(200);
		}
		ShowExitMenu();
	}break;
	case GameState::StartMenu::GS_HelpMenu:
	{
		HelpMenuComplement();
		if (IS_KEY_DOWN(0x1B))
			menu_state = GameState::StartMenu::GS_HelpSelect;
	}break;
	}
}

//显示开始游戏的菜单
void Menu::ShowStartMenu()
{
	std::ifstream infile(strat_filename, std::ios::in);
	if (!infile.is_open())
	{
		MessageBox(0, L"读取开始游戏菜单失败", 0, 0);
		return;
	}
	int index = 0;
	std::string str;
	while (getline(infile, str))
	{
		for (int i = 0; i < str.size(); i++)
		{
			screen[index++] = str[i];
		}
	}
	screen[screen_height * screen_width - 1] = '\0';
	DWORD dw_byte_wirtten = 0;
	WriteConsoleOutputCharacterA(*renderer.h_p_output, screen, screen_width * screen_height, { 0,0 }, &dw_byte_wirtten);
	infile.close();
}

//显示帮助的菜单
void Menu::ShowHelpMenu()
{
	std::ifstream infile(help_filename, std::ios::in);
	if (!infile.is_open())
	{
		MessageBox(0, L"读取帮助菜单失败", 0, 0);
		return;
	}
	int index = 0;
	std::string str;
	while (getline(infile, str))
	{
		for (int i = 0; i < str.size(); i++)
		{
			screen[index++] = str[i];
		}
	}
	screen[screen_height * screen_width - 1] = '\0';
	DWORD dw_byte_wirtten = 0;
	WriteConsoleOutputCharacterA(*renderer.h_p_output, screen, screen_width * screen_height, { 0,0 }, &dw_byte_wirtten);
	infile.close();
}

//显示退出的菜单
void Menu::ShowExitMenu()
{
	std::ifstream infile(exit_filename, std::ios::in);
	if (!infile.is_open())
	{
		MessageBox(0, L"读取退出游戏菜单失败", 0, 0);
		return;
	}
	int index = 0;
	std::string str;
	while (getline(infile, str))
	{
		for (int i = 0; i < str.size(); i++)
		{
			screen[index++] = str[i];
		}
	}
	screen[screen_height * screen_width - 1] = '\0';
	DWORD dw_byte_wirtten = 0;
	WriteConsoleOutputCharacterA(*renderer.h_p_output, screen, screen_width * screen_height, { 0,0 }, &dw_byte_wirtten);
	infile.close();
}

//帮助菜单的具体实现
void Menu::HelpMenuComplement()
{
	//const char* s0 = u8"你是一名诞生出自我意识的BUG，你的使命是破环程序的正常运行，使软件不能运作                                                                                      ";
	//const char* s1 = u8"现在，一名软件工程师已经在程序中定位了你的存在，TA设计了一个迷宫将你困住，并派出AI猎人追杀你，这能忍                                                     ";
	//const char* s2 = u8"你的目标是躲避猎人的追杀，你存活的越久，这个软件就愈发容易崩溃!!!                      ";
	std::ifstream infile("Media/help.txt", std::ios::in);
	std::string str;
	int index = 0;
	while (getline(infile, str))
	{
		for (int i = 0; i < str.size(); i++)
		{
			help_screen[index++] = str[i];
		}
	}

	help_screen[screen_height * screen_width] = '\0';

	DWORD dw_byte_wirtten = 0;
	WriteConsoleOutputCharacterA(*renderer.h_p_output, help_screen, screen_width * screen_height, { 0,0 }, &dw_byte_wirtten);
}

//更新且渲染结束菜单
void Menu::UpdateAndRenderGameOverMenu()
{
	static int over_menu_state = GameState::GameOverMenu::GS_GameOverMenu;
	switch (over_menu_state)
	{
	case GameState::GameOverMenu::GS_GameOverMenu:
	{
		//返回菜单
		if (IS_KEY_DOWN(VK_RETURN))
		{
			game_state = GameState::GS_Menu;
			Sleep(200);
		}
		if (IS_KEY_DOWN(0x53))
		{
			over_menu_state = GameState::GameOverMenu::GS_GameOverExit;
			Sleep(200);
		}
		ShowReturnMenu();
	}break;
	case GameState::GameOverMenu::GS_GameOverExit:
	{
		//退出游戏
		if (IS_KEY_DOWN(VK_RETURN))
		{
			exit(0);
		}
		if (IS_KEY_DOWN(0x57))
		{
			over_menu_state = GameState::GameOverMenu::GS_GameOverMenu;
			Sleep(200);
		}
		ShowExitProgram();
	}break;
	}
}

//显示返回菜单
void Menu::ShowReturnMenu()
{
	std::ifstream infile(return_menu_filename, std::ios::in);
	if (!infile.is_open())
	{
		MessageBox(0, L"读取返回菜单失败", 0, 0);
		return;
	}
	int index = 0;
	std::string str;
	while (getline(infile, str))
	{
		for (int i = 0; i < str.size(); i++)
		{
			screen[index++] = str[i];
		}
	}
	screen[screen_height * screen_width - 1] = '\0';
	DWORD dw_byte_wirtten = 0;
	WriteConsoleOutputAttribute(*renderer.h_p_output, main_game.attribute_array, screen_width * screen_height, { 0,0 }, &dw_byte_wirtten);
	WriteConsoleOutputCharacterA(*renderer.h_p_output, screen, screen_width * screen_height, { 0,0 }, &dw_byte_wirtten);
	infile.close();
}

//显示退出菜单
void Menu::ShowExitProgram()
{
	std::ifstream infile(over_exit_filename, std::ios::in);
	if (!infile.is_open())
	{
		MessageBox(0, L"读取退出菜单失败", 0, 0);
		return;
	}
	int index = 0;
	std::string str;
	while (getline(infile, str))
	{
		for (int i = 0; i < str.size(); i++)
		{
			screen[index++] = str[i];
		}
	}
	screen[screen_height * screen_width - 1] = '\0';
	DWORD dw_byte_wirtten = 0;
	WriteConsoleOutputAttribute(*renderer.h_p_output, main_game.attribute_array, screen_width * screen_height, { 0,0 }, &dw_byte_wirtten);
	WriteConsoleOutputCharacterA(*renderer.h_p_output, screen, screen_width * screen_height, { 0,0 }, &dw_byte_wirtten);
	infile.close();
}