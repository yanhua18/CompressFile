#pragma once
#include"HuffmanTree.hpp"
#include<string>

struct CharInfo
{
	CharInfo(int charCount = 0)
		:_charCount(charCount)
	{}
	unsigned char _ch;//�ַ�
	int _charCount;//�ַ����ֵĴ���
	string _strCode;//�ַ��ı��룬��ȡ����󱣴�����
	

	CharInfo operator+(const CharInfo& c)const
	{
		return CharInfo(_charCount + c._charCount);
	}
	bool operator!=(const CharInfo& c)const
	{
		return _charCount != c._charCount;
	}
	bool operator==(const CharInfo& c)const
	{
		return _charCount == c._charCount;
	}
	bool operator>(const CharInfo& c)const
	{
		return _charCount > c._charCount;
	}
};

class CompreeFileHuff
{
public:
	void CompressFileHuff();//���캯���г�ʼ�����ַ�����
	void CompressFile(const string& strPath);//ѹ���ļ�
	void UNComoressFile(const string& strPath);//��ѹ��
	void HuffmanCode(HuffmanTreeNode<CharInfo>* proot);//��ȡHuffman����



private:
	vector<CharInfo> _charInfo;
};
