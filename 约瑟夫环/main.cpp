#include <iostream>

// ����Ľڵ�
typedef struct sNode{
	sNode *pNext;
	unsigned int value;
	unsigned int index;
} Node;

// ����
typedef struct{
	Node *rear;
	size_t size;
} List;

// ��ʼ������
void list_init(List& list);

// ��ѭ�������β�����½��
int list_push_back(List& l, unsigned int m, unsigned int index);

// ɾ�������е�ָ�����
void list_delete(List& l, Node* pPrev);

// ��ȡ����ĳߴ�
size_t list_size(List& list);

// ��ǰ������㣨ǰ����
Node* node_forward(Node* pPrev);

// ��ȡ����ֵ
unsigned int node_value(Node* pPrev);

// ��ȡ�������
unsigned int node_index(Node* pPrev);

// ��ʼ������
void list_init(List &list)
{
	list.rear = nullptr;
	list.size = 0;
}

// ������β�˲����½ڵ�
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

// ɾ�������е�һ���ڵ�
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

// ǰ�Ƶ�ǰ�ڵ�
Node *node_forward(Node *pPrev)
{
	return pPrev->pNext;
}

// ��ȡ�ڵ��ֵ
unsigned int node_value(Node *pPrev)
{
	return pPrev->pNext->value;
}

// ��ȡ�ڵ㱻����ʱ�����
unsigned int node_index(Node *pPrev)
{
	return pPrev->pNext->index;
}

// ��ȡ����ߴ�
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
		// pPrevNode��Զָ��ǰ����ǰһ�����
		for (unsigned int i = 1; i != m; i++)
		{
			// ǰ��
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