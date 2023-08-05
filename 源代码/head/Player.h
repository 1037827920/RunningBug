#pragma once 

class Player
{
public:
	//记录玩家位置
	float x;
	float y;
	//记录玩家的视野   中间的角度
	float angle;
	//玩家视野大小
	float field_of_view;
	//玩家视野发出的射线能走的最大距离
	float depth;
	//打穿墙壁的能力是否恢复
	int is_punch;

public:
	Player();
	~Player();

};