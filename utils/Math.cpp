#include"MyConsoleEnegine.h"


AStar::AStar(std::vector<std::vector<int>>& grid)
{
	this->grid = grid;
	this->row = grid.size();
	this->col = grid[0].size();
}

//算法实现  返回一个路径数组
std::vector<Node*> AStar::FindPath(Node* start, Node* end)
{
	//路径数组
	std::vector<Node*> path;
	//开始列表
	std::priority_queue<Node*, std::vector<Node*>, std::function<bool(Node*, Node*)>>open_list([](Node* a, Node* b) {return a->f > b->f; });
	//结束列表
	std::vector<Node*>closed_list;

	start->g = 0;
	start->CalculateH(end);
	start->CalculateF();
	//讲起点压入开始列表
	open_list.push(start);

	int dx[] = { 0,0,-1,1 };//上 下 左 右
	int dy[] = { -1,1,0,0 };

	//开始循环开始列表
	while (!open_list.empty())
	{
		Node* current = open_list.top();
		open_list.pop();

		if (current->x == end->x && current->y == end->y)
		{
			while (current != nullptr)
			{
				path.push_back(current);
				current = current->parent;
			}
			std::reverse(path.begin(), path.end());
			return path;
		}

		//在开始列表遍历一遍后压入结束列表
		closed_list.push_back(current);
		//开始遍历目前点的邻居点
		for (int i = 0; i < 4; i++)
		{
			int x = current->x + dx[i];
			int y = current->y + dy[i];

			if (x < 0 || x >= col || y < 0 || y >= row || grid[y][x] == 1) continue;

			Node* neighbor = new Node(x, y);
			neighbor->parent = current;

			int new_g = current->g + abs(dx[i]) + abs(dy[i]);

			//检测邻居点是否在关闭列表内，如果在，则不必再检测这个点
			bool in_closed_list = false;
			for (Node* node : closed_list)
			{
				if (neighbor->x == node->x && neighbor->y == node->y)
				{
					in_closed_list = true;
					break;
				}
			}
			if (in_closed_list) continue;

			neighbor->g = new_g;
			neighbor->CalculateH(end);
			neighbor->CalculateF();
			open_list.push(neighbor);
		}
	}
	//没有路径
	return std::vector<Node*>();
}
