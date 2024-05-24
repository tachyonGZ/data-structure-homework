#include <iostream>

static unsigned int g_compareCnt;
static unsigned int g_moveCnt;

void set_compare_count(unsigned int cnt)
{
	g_compareCnt = cnt;
}

unsigned int get_compare_count()
{
	return g_compareCnt;
}

void set_move_count(unsigned int cnt)
{
	g_moveCnt = cnt;
}

unsigned int get_move_count()
{
	return g_moveCnt;
}

void swap(int &a, int &b)
{
	g_moveCnt += 3;
	int temp = a;
	a = b;
	b = temp;
}

void move(int &dst, const int src)
{
	g_moveCnt += 1;
	dst = src;
}

int compare(const int a, const int b)
{
	g_compareCnt += 1;
	return a - b;
}

void insert_sort(int a[], size_t length)
{
	for (int i = 0; i < length; i++)
	{
		for (int j = i; j > 0; j--)
		{
			//if (a[j - 1] > a[j])
			if (compare(a[j - 1], a[j]) > 0)
			{
				swap(a[j - 1], a[j]);
			}
		}
	}
}

void bubble_sort(int a[], size_t length)
{
	for (size_t i = 1; i < length; i++)
	{
		for (size_t j = i; j < length; j++)
		{
			//if (a[j - 1] > a[j])
			if (compare(a[j - 1], a[j]) > 0)
			{
				swap(a[j - 1], a[j]);
			}
		}
	}
}

void quick_sort(int a[], int begin, int end)
{
	if (begin >= end)
	{
		return;
	}

	int i = begin;
	int j = end;
	const int key = a[i];

	while (i < j)
	{
		//for (; i < j && a[j] >= key; j--);
		for (; i < j && compare(a[j], key) >= 0; j--);
		if (i < j) a[i] = a[j];

		//for (; i < j && a[i] <= key; i++);
		for (; i < j && compare(a[i], key) <= 0; i++);
		if (i < j) a[j] = a[i];
	}

	a[i] = key;

	quick_sort(a, i + 1, end);
	quick_sort(a, begin, i - 1);
}

void quick_sort(int a[], size_t length)
{
	quick_sort(a, 0, length - 1);
}

void select_sort(int a[], size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		size_t min = i;
		for (size_t j = i; j < length; j++)
		{
			//if (a[min] > a[j])
			if (compare(a[min], a[j]) > 0)
			{
				min = j;
			}
		}

		swap(a[i], a[min]);
	}
}

void merge_sort(int a[], int temp[], size_t begin, size_t end)
{
	if (begin >= end)
	{
		return;
	}

	size_t middle = begin + (end - begin) / 2;

	merge_sort(a, temp, begin, middle);
	merge_sort(a, temp, middle + 1, end);
	
	size_t i = begin;
	size_t j = middle + 1;

	size_t k = begin;
	while (i <= middle && j <= end)
	//while (compare(i, middle) <= 0 && compare(j, end) <= 0)
	{
		//temp[k++] = (a[i] < a[j])?(a[i++]):(a[j++]);
		move(temp[k++], (compare(a[i], a[j]) < 0) ? (a[i++]) : (a[j++]));
	}

	while (i <= middle)
	//while (compare(i, middle) <= 0)
	{
		//temp[k++] = a[i++];
		move(temp[k++], a[i++]);
	}

	while (j <= end)
	//while (compare(j, end) <= 0)
	{
		//temp[k++] = a[j++];
		move(temp[k++], a[j++]);
	}
}

void merge_sort(int a[], size_t length)
{
	int *pTemp = new int[length];
	merge_sort(a, pTemp, 0, length - 1);

	for (size_t i = 0; i < length; i++)
	{
		a[i] = pTemp[i];
	}

	delete[] pTemp;
}

#ifdef _DEBUG
#define T_LENGTH 5
#else
#define T_LENGTH 100
#endif

typedef void SORT(int a[], size_t length);

struct sortDescPair
{
	SORT *fnSort;
	const char *szDesc;
};

sortDescPair pairTable[] = {
	{ insert_sort, "²åÈëÅÅÐò" },
	{ bubble_sort, "Ã°ÅÝÅÅÐò" },
	{ quick_sort, "¿ìËÙÅÅÐò" },
	{ select_sort, "Ñ¡ÔñÅÅÐò"},
	{ merge_sort, "¹é²¢ÅÅÐò" },
};

void sort_by_input()
{
	int a[T_LENGTH];

	for (size_t i = 0; i < T_LENGTH; i++)
	{
		std::cin >> a[i];
	}

	int b[T_LENGTH];

	for (size_t i = 0; i < 5; i++)
	{
		set_compare_count(0);
		set_move_count(0);

		std::copy(std::cbegin(a), std::cend(a), std::begin(b));
		pairTable[i].fnSort(a, T_LENGTH);
		std::cout << get_compare_count() << " " << get_move_count() << " ";
	}

	std::cout << std::endl;
}

int main(int argc, char **argv)
{
	unsigned int n = 0;
	std::cin >> n;

	for (unsigned int i = 0; i < n; i++)
	{
		sort_by_input();
	}

	return 0;
}