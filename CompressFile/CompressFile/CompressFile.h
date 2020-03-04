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
	string _strCode;//�ַ��ı���
	

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
	void CompressFileHuff();//���캯���г�ʼ�����ַ�����
	void CompressFile(const string& strPath);//ѹ���ļ�
	void UNComoressFile(const string& strPath);
	void HuffmanCode(HuffmanTreeNode<CharInfo>* proot);
private:
	vector<CharInfo> _charInfo;
};
