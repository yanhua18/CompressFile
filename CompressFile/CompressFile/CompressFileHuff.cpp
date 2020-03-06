#pragma once
#include"CompressFile.h"
#include<vector>
#include<assert.h>

//文件的压缩

CompreeFileHuff::CompreeFileHuff()//构造函数中初始化好字符数组
{
	_charInfo.resize(256);
	for (int i = 0; i < 256; i++)
	{
		_charInfo[i]._ch = i;
		_charInfo[i]._charCount = 0;
	}
}

void CompreeFileHuff::CompressFile(const string& Path)
{
	//1，统计结果每个字符在文件中出现的次数
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

	//2，以字符出现的次数创建Huffman树
	HuffmanTree<CharInfo> ht;
	ht.CreatHuffmanTree(_charInfo, CharInfo(0));
	//3，获取字符的编码
	HuffmanCode(ht.GetRoot());

	//4，用每个字符的编码重新改写原文件
	FILE* fOut = fopen("6.txt", "w");//用来保存压缩结果的文件
	assert(fOut);

	WriteHead(fOut, Path);
	fseek(fIn, 0, SEEK_SET);
	char ch = 0;
	int bitCount = 0;
	while (true)
	{
		int rdsize = fread(ReadBuffer, 1, 1024, fIn);
		if (rdsize == 0)
		{
			break;
		}
		for (int i = 0; i < rdsize; i++)
		{
			string strCode = _charInfo[ReadBuffer[i]]._strCode;
			for (size_t j = 0; j < strCode.size(); ++j)
			{
				ch <<= 1;
				if (strCode[j] == '1')
				{
					ch |= 1;
				}
				bitCount++;
				if (8 == bitCount)
				{
					fputc(ch, fOut);
					bitCount = 0;
					ch = 0;
				}
			}
		}
	}
	//最后一个字节可能不够8个比特位，没有放进压缩文件中去
	if (bitCount < 8)
	{
		ch <<= (8 - bitCount);
		fputc(ch, fOut);
	}
	delete[] ReadBuffer;
	fclose(fIn);
	fclose(fOut);
}

//解压缩
void CompreeFileHuff::UNComoressFile(const string& strPath)
{
	FILE* fIn = fopen(strPath.c_str(), "r");
	assert(fIn);
	//文件的后缀
	string strFilePosFix;
	ReadLine(fIn, strFilePosFix);

	//字符信息的总行数
	string strCount;
	ReadLine(fIn, strCount);
	int lineCount = atoi(strCount.c_str());

	//字符信息
	for (int i = 0; i < lineCount; ++i)
	{
		string strChCount;
		ReadLine(fIn, strChCount);
		_charInfo[strChCount[0]]._charCount = atoi(strChCount.c_str() + 2);

	}
	
	//还原Huffman树
	HuffmanTree<CharInfo> t;
	t.CreatHuffmanTree(_charInfo, CharInfo(0));
	//解压缩
}




string CompreeFileHuff::GetFilePostFix(const string& fileName)
{
	return fileName.substr(fileName.rfind('.'));
}

//将解压缩信息写入压缩文件中
void CompreeFileHuff::WriteHead(FILE* fOut, const string& fileName)
{
	assert(fOut);
	//1,,记录文件后缀
	string strHead;
	strHead += GetFilePostFix(fileName);
	strHead += '\n';
	//2,记录字符次数信息
	size_t lineCount = 0;
	string strCount;//字符次数段
	char szValue[32] = { 0 };
	for (int i = 0; i < 256; i++)
	{
		if (_charInfo[i]._charCount)
		{
			lineCount++;
			strCount += _charInfo[i]._ch;
			strCount += ':';
			_itoa(_charInfo[i]._charCount, szValue, 10);
			strCount += szValue;
			strCount += '\n';
		}
	}
	//3，记录行数
	_itoa(lineCount, szValue, 10);
	strHead += szValue;
	strHead += '\n';

	strHead += strCount;

	fwrite(strHead.c_str(), 1, strHead.size(), fOut);

}


//获取字符的编码
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
