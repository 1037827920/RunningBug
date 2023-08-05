#pragma once

namespace Game
{
	class Menu
	{
	private:
		std::string strat_filename;
		std::string help_filename;
		std::string exit_filename;
		std::string return_menu_filename;
		std::string over_exit_filename;

		char* screen;
		char* help_screen;

	public:
		Menu();
		~Menu();

		//更新且渲染菜单
		void UpdateAndRenderMenu();
		//显示开始游戏的菜单
		void ShowStartMenu();
		//显示帮助的菜单
		void ShowHelpMenu();
		//显示退出的菜单
		void ShowExitMenu();
		//帮助菜单的具体实现
		void HelpMenuComplement();

		//更新且渲染结束菜单
		void UpdateAndRenderGameOverMenu();
		//显示返回菜单
		void ShowReturnMenu();
		//显示退出菜单
		void ShowExitProgram();
	};
}