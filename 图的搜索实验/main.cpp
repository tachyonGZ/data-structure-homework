#include <iostream>

struct PointInfo
{
	size_t x;
	size_t y;
};

struct Queue
{
	size_t maxSize;
	size_t head;
	size_t rear;
	PointInfo *p;
};

// 初始化队列
void init_queue(Queue &q, size_t elementNum)
{
	q.maxSize = elementNum + 1;
	q.head = 0;
	q.rear = 0;
	q.p = new PointInfo[q.maxSize];
}

// 反初始化队列
void uninit_queue(Queue &q)
{
	delete[] q.p;
}

// 判断队列是否为空
bool queue_empty(Queue &q)
{
	return q.head == q.rear;
}

// 获取队首元素
const PointInfo &queue_front(Queue &q)
{
	return q.p[q.head];
}

// 从队首中弹出元素
void queue_pop(Queue &q)
{
	q.head++;
}

// 向队尾中压入元素
void queue_push(Queue &q, const PointInfo &info)
{
	if ((q.rear + 1) % q.maxSize == q.head)
	{
		std::cerr << "queue overflow!";
		return;
	}

	q.p[q.rear] = info;

	q.rear = (q.rear + 1) % q.maxSize;
}

size_t g_row;
size_t g_col;

typedef int *Maze;

Maze g_maze;

// 获取迷宫二维数组元素的引用
int &maze_ref(size_t x, size_t y)
{
	return g_maze[(x * g_col) + y];
}

// 初始化迷宫二维数组
void maze_init()
{
	g_maze = new int[g_row * g_col];
}

// 反初始化迷宫二维数组
void maze_uninit()
{
	delete[] g_maze;
}

typedef PointInfo **Path;

Path g_path;

PointInfo *&path_ref(size_t x, size_t y)
{
	return g_path[(x * g_col) + y];
}

void path_init()
{
	g_path = new PointInfo *[g_row * g_col];

	for (size_t i = 0; i < g_row; i++)
	{
		for (size_t j = 0; j < g_col; j++)
		{
			path_ref(i, j) = nullptr;
		}
	}
}

void path_uninit()
{
	for (size_t i = 0; i < g_row; i++)
	{
		for (size_t j = 0; j < g_col; j++)
		{
			if (path_ref(i, j))
			{
				delete path_ref(i, j);
			}
		}
	}

	delete[] g_path;
}

PointInfo g_start;
PointInfo g_end;

size_t g_vertexNum;

int g_d[4][2] = {
	{0, 1},
	{1, 0},
	{0, -1},
	{-1, 0},
};

void find_path_by_bfs()
{
	Queue q;
	init_queue(q, g_vertexNum);

	queue_push(q, g_start);

	// directed edge
	// v ----> w

	struct PointInfo w;

	while (!queue_empty(q))
	{
		const PointInfo &v = queue_front(q);

		if (v.x == g_end.x && v.y == g_end.y)
		{
			// find a way from start to end
			break;
		}

		for (size_t i = 0; i < 4; i++)
		{
			w.x = v.x + g_d[i][0];
			w.y = v.y + g_d[i][1];

			// ensure w is a 0-vertex
			// 0 mean that vertex has not been traveled
			if (0 != maze_ref(w.x, w.y))
			{
				continue;
			}

			// 2 mean that vertex hash been traveled
			maze_ref(w.x, w.y) = 2;

			path_ref(w.x, w.y) = new PointInfo;
			path_ref(w.x, w.y)->x = v.x;
			path_ref(w.x, w.y)->y = v.y;

			queue_push(q, w);

		}

		queue_pop(q);
	}

	uninit_queue(q);
}


void read_start_and_end()
{
	std::cout << "请输入开始点和结束点\n";
	std::cin >> g_start.x >> g_start.y;
	std::cin >> g_end.x >> g_end.y;
}

void read_row_and_col()
{
	std::cout << "请输入行列\n";
	std::cin >> g_row >> g_col;
	g_row += 2;
	g_col += 2;
	g_vertexNum = g_row * g_col;
}

void construct_maze_by_read_line()
{
	std::cout << "请按行输入迷宫\n";

	// set edge of maze to 1-vertex
	for (size_t i = 0; i < g_row; i++)
	{
		maze_ref(i, 0) = 1;
		maze_ref(i, g_col - 1) = 1;
	}

	for (size_t i = 0; i < g_col; i++)
	{
		maze_ref(0, i) = 1;
		maze_ref(g_row - 1, i) = 1;
	}

	// construct maze by read line
	for (size_t i = 1; i < g_row - 1; i++)
	{
		for (size_t j = 1; j < g_col - 1; j++)
		{
			std::cin >> maze_ref(i, j);
		}
	}
}

void print_path(size_t x, size_t y)
{
	if (x != g_start.x || y != g_start.y)
	{
		print_path(path_ref(x, y)->x, path_ref(x, y)->y);
	}

	std::cout << "(" << x << "," << y << ") ";
}

void print_path()
{
	print_path(g_end.x, g_end.y);
}

int main(int argc, char *argv[])
{
	read_row_and_col();
	maze_init();
	path_init();

	construct_maze_by_read_line();

	read_start_and_end();

	find_path_by_bfs();

	print_path();

	path_uninit();
	maze_uninit();
	return 0;
}