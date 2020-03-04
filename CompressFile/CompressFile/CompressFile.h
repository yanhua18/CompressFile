#pragma once
#include"HuffmanTree.hpp"
#include<string>

struct CharInfo
{
	CharInfo(int charCount = 0)
		:_charCount(charCount)
	{}
	unsigned char _ch;//字符
	int _charCount;//字符出现的次数
	string _strCode;//字符的编码
	

	CharInfo operator+(const CharInfo& c)const
	{
		return CharInfo(_charCount + c._charCount);
	}
	CharInfo operator!=(const CharInfo& c)const
	{
		return _charCount != c._charCount;
	}
	CharInfo operator==(const CharInfo& c)const
	{
		return _charCount == c._charCount;
	}
	CharInfo operator>(const CharInfo& c)const
	{
		return _charCount > c._charCount;
	}
};

class CompreeFileHuff
{
public:
	void CompressFileHuff();//构造函数中初始化好字符数组
	void CompressFile(const string& strPath);//压缩文件
	void UNComoressFile(const string& strPath);
	void HuffmanCode(HuffmanTreeNode<CharInfo>* proot);
private:
	vector<CharInfo> _charInfo;
};
