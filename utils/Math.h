#pragma once

struct Node
{
	int x, y;//坐标
	int f, g, h;//f代表总价值   g代表从出发点到目前点的价值    h是目前点到目的点的预估价值，这是用曼哈顿距离预估价值
	Node* parent;//父节点

	Node(int x, int y)
	{
		this->x = x;
		this->y = y;
		parent = nullptr;
	}

	void CalculateH(Node* other)
	{
		int dx = abs(x - other->x);
		int dy = abs(y - other->y);
		this->h = dx + dy;
	}

	void CalculateF()
	{
		this->f = this->g + this->h;
	}
};

class AStar
{
private:
	std::vector<std::vector<int>> grid;
	int row, col;

public:
	AStar(std::vector<std::vector<int>>& grid);

	//算法实现  返回一个路径数组
	std::vector<Node*> FindPath(Node* start, Node* end);
};