#include <iostream>

// 链表的节点
typedef struct sNode{
	sNode *pNext;
	unsigned int value;
	unsigned int index;
} Node;

// 链表
typedef struct{
	Node *rear;
	size_t size;
} List;

// 初始化链表
void list_init(List& list);

// 向循环链表结尾插入新结点
int list_push_back(List& l, unsigned int m, unsigned int index);

// 删除链表中的指定结点
void list_delete(List& l, Node* pPrev);

// 获取链表的尺寸
size_t list_size(List& list);

// 向前遍历结点（前进）
Node* node_forward(Node* pPrev);

// 获取结点的值
unsigned int node_value(Node* pPrev);

// 获取结点的序号
unsigned int node_index(Node* pPrev);

// 初始化链表
void list_init(List &list)
{
	list.rear = nullptr;
	list.size = 0;
}

// 项链表尾端插入新节点
int list_push_back(List &l, unsigned int m, unsigned int index)
{
	Node *p = new Node;
	if (nullptr == p)
	{
		return -1;
	}

	p->value = m;
	p->index = index;

	if (0 == l.size)
	{
		p->pNext = p;
	}
	else
	{
		p->pNext = l.rear->pNext;
		l.rear->pNext = p;
	}

	l.rear = p;

	l.size++;
	return 1;
}

// 删除链表中的一个节点
void list_delete(List &l, Node *pPrev)
{
	Node *p = pPrev->pNext;

	if (l.size == 1)
	{
		l.rear = nullptr;
	}
	else
	{
		pPrev->pNext = p->pNext;
		if (p == l.rear)
		{
			l.rear = pPrev;
		}
	}
	delete p;

	l.size--;
}

// 前移当前节点
Node *node_forward(Node *pPrev)
{
	return pPrev->pNext;
}

// 获取节点的值
unsigned int node_value(Node *pPrev)
{
	return pPrev->pNext->value;
}

// 获取节点被插入时的序号
unsigned int node_index(Node *pPrev)
{
	return pPrev->pNext->index;
}

// 获取链表尺寸
size_t list_size(List &list)
{
	return list.size;
}

void make_joseph_list(List &l, unsigned int n)
{
	unsigned int indexCount = 1;
	for (unsigned int i = 0; i < n; i++)
	{
		unsigned int m = 0;
		std::cin >> m;
		list_push_back(l, m, indexCount++);
	}
}

void process_joseph_list(List &l, unsigned int m)
{

	Node* pPrev = l.rear;

	while (list_size(l) != 0)
	{
		// pPrevNode永远指向当前结点的前一个结点
		for (unsigned int i = 1; i != m; i++)
		{
			// 前进
			pPrev = node_forward(pPrev);
		}

		m = node_value(pPrev);

		printf("%d ", node_index(pPrev));

		list_delete(l, pPrev);

	}

	printf("\n");
}

int main(int argc, char **argv)
{
	List list;
	list_init(list);

	unsigned int m = 0;
	unsigned int n = 0;
	
	std::cin >> m >> n;

	make_joseph_list(list, n);
	process_joseph_list(list, m);
	return 0;
}