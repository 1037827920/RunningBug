#include"MyConsoleEnegine.h"
#include"Game.h"


void PlayMusic()
{
	while (1)
	{
		PlaySound(NULL, NULL, SND_PURGE);
		if (Game::is_less_10)
		{
			PlaySound(L"Media/music/is_less_10_sound.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
			std::this_thread::sleep_for(std::chrono::seconds(3));
		}
		if (Game::is_less_5)
		{
			PlaySound(L"Media/music/is_less_5_sound.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
			std::this_thread::sleep_for(std::chrono::seconds(3));
		}
		if (Game::is_play_get_info)
		{
			Game::is_play_get_info = false;
			PlaySound(L"Media/music/get_info.wav", NULL, SND_FILENAME | SND_ASYNC);
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		if (Game::is_player_die)
		{
			PlaySound(L"Media/music/die.wav", NULL, SND_FILENAME | SND_ASYNC);
			std::this_thread::sleep_for(std::chrono::seconds(2));
			Game::is_player_die = false;
		}
	}
}

int main()
{
	std::thread t(PlayMusic);


	Game::InitGlobal();
	//游戏主循环
	while (1)
	{
		//设置窗口名称
		Game::UpdateWindowTitle();
		//游戏状态机驱动程序r
		Game::GameStateSelect();
	}

	//等待音乐播放线程结束
	t.join();

	return 0;
}