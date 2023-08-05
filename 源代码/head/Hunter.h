

class Hunter
{
public:
	//记录猎人的位置
	float x;
	float y;
	//猎人思考的时间
	int thinking;
	int run_thinking;
	//猎人移动的速度
	int move_speed;
	int run_speed;
	//判断猎人是否存活
	bool is_death;

public:
	Hunter();
	~Hunter();

	//让猎人Blinky移动到下一个路径点
	void BMoveToNextPoint(int nextX, int nextY, std::wstring& map, int map_width, int map_height, bool* is_info_fragment);
	//让猎人Tinky移动到下一个路径点
	void TMoveToNextPoint(int nextX, int nextY, std::wstring& map, int map_width, int map_height, bool* is_info_fragment);

	//就算猎人与玩家之间的距离
	float CalculateDistance(float playerX, float playerY);
};