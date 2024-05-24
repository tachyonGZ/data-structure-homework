#include <iostream>
#include <fstream>
#include <algorithm>

struct HTNode{
	int weight;
	size_t lchild, rchild, parent;
};

using HuffmanTree = HTNode *;

void select_huffman_tree(HuffmanTree& ht, const size_t i, size_t& l, size_t& r)
{
	// 寻找根节点子集中的第一个
	l = 0;
	for (size_t j = 1; j < i; j++)
	{
		if (0 == ht[j].parent)
		{
			l = j;
			break;
		}
	}

	if (0 == l)
	{
		return;
	}

	// 寻找根节点子集中的第二个
	r = 0;
	for (size_t j = l + 1; j < i; j++)
	{
		if (0 == ht[j].parent)
		{
			r = j;
			break;
		}
	}

	if (0 == r)
	{
		return;
	}

	// 确保l的权值比r小
	if (ht[l].weight > ht[r].weight)
	{
		std::swap(l, r);
	}

	// 确保l和r的权值是子集中最小的
	for (size_t j = r + 1; j < i; j++)
	{
		if (ht[j].parent != 0)
		{
			continue;
		}

		if (ht[j].weight < ht[l].weight)
		{
			r = l; l = j;
		}
		else if (ht[j].weight < ht[r].weight)
		{
			r = j;
		}
	}
}

// 构造哈夫曼树
void make_huffman_tree(HuffmanTree &ht, int weightTable[], size_t setSize)
{
	size_t m = setSize * 2 - 1;
	ht = new HTNode[m + 1];

	for (size_t i = 0; i <= m; i++)
	{
		ht[i].lchild = 0;
		ht[i].rchild = 0;
		ht[i].parent = 0;

		if (i <= setSize)
		{
			ht[i].weight = weightTable[i];
		}
		else
		{
			ht[i].weight = 0;
		}
	}

	size_t l, r;
	for (size_t i = setSize + 1; i <= m; i++)
	{
		select_huffman_tree(ht, i, l, r);
		ht[i].lchild = l;
		ht[i].rchild = r;
		ht[i].weight = ht[l].weight + ht[r].weight;
		ht[l].parent = i;
		ht[r].parent = i;
	}
}

void release_huffman_tree(HuffmanTree ht)
{
	if (nullptr == ht)
	{
		return;
	}

	delete []ht;
}

int depth_huffman_tree(const HuffmanTree ht, const size_t setSize, const size_t index)
{
	if (0 == index)
	{
		return 0;
	}

	int m = depth_huffman_tree(ht, setSize, ht[index].lchild);
	int n = depth_huffman_tree(ht, setSize, ht[index].rchild);

	return (m > n) ? (m + 1) : (n + 1);
}

// 计算哈夫曼树的深度
inline int depth_huffman_tree(const HuffmanTree ht, const size_t setSize)
{
	return depth_huffman_tree(ht, setSize, setSize * 2 - 1);
}

// 打印哈夫曼树
void print_huffman_tree(const HuffmanTree ht, const size_t htN)
{
	int depth = depth_huffman_tree(ht, htN);
	size_t width = (2 << depth) - 3; // width = 2^(n+1)-3
	size_t height = (2 << (depth - 1)) - 1; // height = 2^n -1
}

using HuffmanCode = char **;


// 构造哈夫曼编码表
void make_huffman_code(HuffmanCode &hc, const HuffmanTree &ht, const size_t setSize)
{
	hc = new char *[setSize + 1];


	size_t len = setSize - 1;
	char *pTemp = new char[len];

	for (size_t i = 1; i <= setSize; i++)
	{
		size_t str = len;


		// 二叉树自底向上构造哈夫曼编码
		for (size_t j = i, prep = j; ht[j].parent != 0; prep = j)
		{
			j = ht[j].parent;

			if (ht[j].lchild == prep)
			{
				pTemp[--str] = '0';
			}
			else if (ht[j].rchild == prep)
			{
				pTemp[--str] = '1';
			}
		}

		hc[i] = new char[len - str];
		std::memcpy(hc[i], pTemp + str, len - str);
	}

	delete []pTemp;
}

void release_huffman_code(HuffmanCode &hc,const size_t setSize)
{
	for (size_t i = 0; i <= setSize; i++)
	{
		if (hc[i])
		{
			delete hc[i];
		}
	}

	delete[] hc;
}

// 编码
void encode(std::ofstream &ofs, std::ifstream &ifs, const HuffmanCode& hc, const char ATable[], const size_t setSize)
{
	while (true)
	{
		char a;
		ifs >> a;
		if (ifs.eof())
		{
			break;
		}

		size_t i = 0;
		for (size_t j = 1; j <= setSize; j++)
		{
			if (ATable[j] == a)
			{
				i = j;
				break;
			}
		}

		if (0 == i)
		{
			//error
			//exit(-1);
			break;
		}

		for (size_t j = 0; hc[i][j] != '0'; j++)
		{
			ofs << hc[i][j];
		}
	}
}

// 解码
void decode(std::ofstream& ofs, std::ifstream& ifs, const HuffmanTree ht, const char ATable[], const size_t setSize)
{
	const size_t m = setSize * 2 - 1;
	size_t index = m;
	while (true)
	{
		char a;
		ifs >> a;
		if (ifs.eof())
		{
			break;
		}

		switch(a)
		{
		case '0':
			index = ht[index].lchild;
			break;
		case '1':
			index = ht[index].rchild;
			break;
		default:
			// error
			break;
		}

		// 遍历到根节点，输出根节点对应的原码，并将路径重置到根节点
		if (0 == ht[index].lchild && 0 == ht[index].rchild)
		{
			ofs << ATable[index];
			index = m;
		}
	}
}

// 打印代码文件
void print_codefile(std::ofstream& ofs, std::ifstream& ifs)
{
	int i = 1;
	while(true)
	{
		char temp;
		ifs >> temp;
		if(ifs.eof())
		{
			break;
		}

		ofs << temp;
		std::cout << temp;

		if (i % 50 == 0)
		{
			ofs << '\n';
			std::cout << std::endl;
		}

		i++;
	}
}

int main(int argc, char **argv)
{
	bool bExit = false;

	HuffmanTree ht = nullptr;

	// 用于标记哈夫曼树是否已经构造
	bool bHt = false;

	// 字符集大小
	size_t setSize = 0;

	HuffmanCode hc;

	// 用于标记哈夫曼编码表是否已经构造
	bool bHc = false;

	char *ATable = nullptr;

	int *weightTable = nullptr;

	while (!bExit)
	{
		char choose;
		std::cout << "]请选择你要进行的操作" << std::endl 
			<< ">";
		std::cin >> choose;

		//FILE *fI = nullptr, *fO = nullptr;
		std::ifstream ifs;
		std::ofstream ofs;
		switch (choose)
		{
		case 'I':
			std::cout << "]请输入字符集大小\n>";
			std::cin >> setSize;

			ATable = new char[setSize + 1];
			

			std::cout << "]请依次输入" << setSize << "个字符" << std::endl;
			for (size_t i = 1; i <= setSize; i++)
			{
				std::cout << '>';
				std::cin >> ATable[i];
			}

			weightTable = new int[setSize + 1];

			std::cout << "]请依次输入" << setSize << "个字符对应的权值" << std::endl;
			for (size_t i = 1; i <= setSize; i++)
			{
				std::cout << '>';
				std::cin >> weightTable[i];
			}

			make_huffman_tree(ht, weightTable, setSize);
			bHt = true;
			std::cout << "]哈夫曼树构造完成" << std::endl;

			make_huffman_code(hc, ht, setSize);
			bHc = true;
			std::cout << "]哈夫曼编码表构造完成" << std::endl;
			break;

		case 'C':
			ifs.open("textfile.txt", std::ios::in);
			ofs.open("codefile.txt", std::ios::out);
			encode(ofs, ifs, hc, ATable, setSize);
			ofs.close();
			ifs.close();
			std::cout << "]编码结果已存入codefile.txt中" << std::endl;
			break;

		case 'D':
			if (!bHt)
			{
				break;
			}
			ifs.open("codefile.txt", std::ios::in);
			ofs.open("textfile.txt", std::ios::out);
			decode(ofs, ifs, ht, ATable, setSize);
			ofs.close();
			ifs.close();
			std::cout << "]译码结果已存入textfile.txt中" << std::endl;
			break;

		case 'P':
			std::cout << "]codefile.txt内容如下:" << std::endl;
			ifs.open("codefile.txt", std::ios::in);
			ofs.open("codeprint.txt", std::ios::out);
			print_codefile(ofs, ifs);
			ofs.close();
			ifs.close();
			std::cout << "]打印信息已被保存在codeprint.txt中" << std::endl;
			break;

		case 'T':
			break;
		
		case 'E':
			bExit = true;
			break;
		}
	}

	if (weightTable)
	{
		delete[] weightTable;
	}

	if (ATable)
	{
		delete[] ATable;
	}

	release_huffman_code(hc, setSize);

	release_huffman_tree(ht);
	return 0;
}