#include"CompressFile.h"
#include"HuffmanTree.hpp"
#include<assert.h>

//�ļ���ѹ��

void CompreeFileHuff::CompressFileHuff()//���캯���г�ʼ�����ַ�����
{
	_charInfo.resize(256);
	for (int i = 0; i < 256; i++)
	{
		_charInfo[i]._ch = i;
		_charInfo[i]._charCount = 0;
	}
}
//1��ͳ�ƽ��ÿ���ַ����ļ��г��ֵĴ���
void CompreeFileHuff::CompressFile(const string& Path)
{
	FILE* fIn = fopen(Path.c_str(), "r");
	if (fIn == nullptr)//���ļ��Ƿ��
	{
		assert(false);
		return;
	}
	char* ReadBuffer = new char[1024];
	while (true)
	{
		int rdsize = fread(ReadBuffer, 1, 1024, fIn);//����ֵ�Ƕ����˶��ٸ��ֽ�
		if (rdsize == 0)
		{
			break;
		}
		for (int i = 0; i < rdsize; i++)
		{
			_charInfo[ReadBuffer[i]]._charCount++;
		}
	}

	//����Huffman��
	HuffmanTree<CharInfo> ht;
	ht.CreatHuffmanTree(_charInfo, CharInfo(0));
	//��ȡ�ַ��ı���
	HuffmanCode(ht.GetRoot());


	delete[] ReadBuffer;
	fclose(fIn);
}


//��ÿ���ַ��ı������¸�дԭ�ļ�
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
