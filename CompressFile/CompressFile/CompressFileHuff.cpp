#include"CompressFile.h"
#include"HuffmanTree.hpp"
#include<assert.h>

//文件的压缩

void CompreeFileHuff::CompressFileHuff()//构造函数中初始化好字符数组
{
	_charInfo.resize(256);
	for (int i = 0; i < 256; i++)
	{
		_charInfo[i]._ch = i;
		_charInfo[i]._charCount = 0;
	}
}
//1，统计结果每个字符在文件中出现的次数
void CompreeFileHuff::CompressFile(const string& Path)
{
	FILE* fIn = fopen(Path.c_str(), "r");
	if (fIn == nullptr)//看文件是否打开
	{
		assert(false);
		return;
	}
	char* ReadBuffer = new char[1024];
	while (true)
	{
		int rdsize = fread(ReadBuffer, 1, 1024, fIn);//返回值是读到了多少个字节
		if (rdsize == 0)
		{
			break;
		}
		for (int i = 0; i < rdsize; i++)
		{
			_charInfo[ReadBuffer[i]]._charCount++;
		}
	}

	//创建Huffman树
	HuffmanTree<CharInfo> ht;
	ht.CreatHuffmanTree(_charInfo, CharInfo(0));
	//获取字符的编码
	HuffmanCode(ht.GetRoot());


	delete[] ReadBuffer;
	fclose(fIn);
}


//用每个字符的编码重新改写原文件
void CompreeFileHuff::HuffmanCode(HuffmanTreeNode<CharInfo>* proot)
{
	if (proot == nullptr)
	{
		return;
	}
	HuffmanCode(proot->_pLeft);
	HuffmanCode(proot->_pRight);
	if (proot->_pLeft == nullptr&&proot->_pRight == nullptr)
	{
		HuffmanTreeNode<CharInfo>* pcur = proot;
		HuffmanTreeNode<CharInfo>*pparent = proot->_pParent;
		string& strCode = _charInfo[pcur->_value._ch]._strCode;
		while (pparent)
		{
			if (pparent->_pLeft == pcur)
			{
				strCode += '0';
			}
			else
			{
				strCode += '1';
			}
			pcur = pparent;
			pparent = pcur->_pParent;
		}
		reverse(strCode.begin(), strCode.end());
	}
}
