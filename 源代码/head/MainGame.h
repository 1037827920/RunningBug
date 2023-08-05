#pragma once

namespace Game
{
	class MainGameClass
	{
	public:
		//生成玩家
		Player player;
		//生成猎人
		Hunter Blinky;
		Hunter Tinky;
		//记录猎人的数量
		int num_hunter;
		//创建对应猎人路径数组
		std::vector<Node*>Blinky_path;
		std::vector<Node*>Tinky_path;
		//地图的宽度和高度
		int map_width;
		int map_height;
		//显示的屏幕
		wchar_t* screen;
		//地图
		std::wstring map;
		//判断地图是否被打开
		bool map_is_open;
		//迷宫的行列
		int ROW;
		int COL;
		//保留控制台原来字符的颜色 每一个字符都要
		WORD* attribute_array;
		//记录玩家目前收集的信息碎片数量
		int num_info_fragment;
		//这个数组记录此处是否有信息碎片
		bool* is_info_fragment;
		//记录玩家的朝向
		int player_direction;
		//记录玩家是否可以地图
		int can_open_map;
		int collect_info;

	public:
		//构造和析构函数
		MainGameClass();
		~MainGameClass();
		//更新并渲染游戏进程
		void UpdateAndRenderMainGame();



		//游戏开始前的初始化
		void GameInit();
		//生成迷宫  使用递归回溯算法
		void GenerateMaze(int row, int col);
		//给地图赋值
		void SetMap();
		//生成猎人Blincy
		void GenerateBlincy();
		//生成猎人Tinky
		void GenerateTinky();
		//随机生成玩家的位置
		void GeneratePlayerPos();



		//游戏进行中的总函数
		void GamePlaying();
		//游戏玩家视角转动和前后移动
		void PlayerTrunAndMove();
		//模拟玩家视角发出的120条(由屏幕宽度决定)射线的移动以及碰撞 并对screen进行赋值
		void PlayerRaySimulation();
		//绘制
		void DrawGamePlaying();
		//按下Tab键显示地图
		void ShowMap();
		//按下p键暂停
		void PauseGame();
		//检查是否有墙壁
		bool CheckWall(int x, int y);

		//设置猎人Blinky的AI
		void SetAIBlinky();
		//设置猎人Tinky的AI
		void SetAITinky();
		//猎人如果在玩家附近，播放心跳声
		void PlayJumpSound();
		//如果猎人与玩家碰撞，游戏结束
		void JudgeGameOver();

		//模拟玩家收集信息碎片
		void CollectInfoFragment();

		//游戏结束要做的一些清理工作
		void Clear();


		//游戏暂停
		void GamePause();
		//退出游戏
		void ExitGame();
	};

}