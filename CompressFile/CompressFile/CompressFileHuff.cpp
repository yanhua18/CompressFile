#pragma once
#include"CompressFile.h"
#include<vector>
#include<assert.h>

//�ļ���ѹ��

CompreeFileHuff::CompreeFileHuff()//���캯���г�ʼ�����ַ�����
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
	//1��ͳ�ƽ��ÿ���ַ����ļ��г��ֵĴ���
	FILE* fIn = fopen(Path.c_str(), "rb");
	if (fIn == nullptr)//���ļ��Ƿ��
	{
		assert(false);
		return;
	}
	unsigned char * ReadBuffer = new unsigned char[1024];
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
	
	//2�����ַ����ֵĴ�������Huffman��
	HuffmanTree<CharInfo> ht;
	ht.CreatHuffmanTree(_charInfo, CharInfo(0));
	//3����ȡ�ַ��ı���
	HuffmanCode(ht.GetRoot());

	//4����ÿ���ַ��ı������¸�дԭ�ļ�
	FILE* fOut = fopen("end_compress.txt", "wb");//��������ѹ��������ļ�
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
	//���һ���ֽڿ��ܲ���8������λ��û�зŽ�ѹ���ļ���ȥ
	if (bitCount < 8)
	{
		ch <<= (8 - bitCount);
		fputc(ch, fOut);
	}
	delete[] ReadBuffer;
	fclose(fIn);
	fclose(fOut);
}

//��ѹ��
void CompreeFileHuff::UNComoressFile(const string& strPath)
{
	FILE* fIn = fopen(strPath.c_str(), "rb");
	assert(fIn);
	//�ļ��ĺ�׺
	string strFilePosFix;
	ReadLine(fIn, strFilePosFix);

	//�ַ���Ϣ��������
	string strCount;
	ReadLine(fIn, strCount);
	int lineCount = atoi(strCount.c_str());

	//�ַ���Ϣ
	for (int i = 0; i < lineCount; ++i)
	{
		string strChCount;
		ReadLine(fIn, strChCount);

		//�����ȡ�����ǻ���,��Ҫ��������
		if (strChCount.empty())
		{
			strChCount += '\n';
			ReadLine(fIn, strChCount);
		}
		_charInfo[(unsigned char)strChCount[0]]._charCount = atoi(strChCount.c_str() + 2);

	}
	
	//��ԭHuffman��
	HuffmanTree<CharInfo> t;
	t.CreatHuffmanTree(_charInfo, CharInfo(0));



	//��ѹ��
	FILE* fOut = fopen("UNcompress_src.txt", "wb");//���������ѹ���ļ�
	assert(fOut);

	char* pReadBuff = new char[1024];
	char ch = 0;
	HuffmanTreeNode<CharInfo>* pCur = t.GetRoot();
	size_t fileSize = pCur->_value._charCount;
	size_t unCount = 0;
	while (true)
	{
		size_t rdsize = fread(pReadBuff, 1, 1024, fIn);
		if (0 == rdsize)
			break;

		for (size_t i = 0; i < rdsize; ++i)
		{
			//ֻ�轫һ���ֽ��е�8������λ���е����Ĵ���
			ch = pReadBuff[i];
			for (int pos = 0; pos < 8; pos++)
			{
				if (ch & 0x80)
				{
					pCur = pCur->_pRight;
				}
				else
				{
					pCur = pCur->_pLeft;
				}

				ch <<= 1;

				if (pCur->_pLeft == nullptr && pCur->_pRight == nullptr)
				{
					unCount++;
					fputc(pCur->_value._ch, fOut);
					if (unCount == fileSize)
						break;
					pCur = t.GetRoot();
				}
			}
		}
	}
	delete[] pReadBuff;
	fclose(fIn);
	fclose(fOut);
}

void CompreeFileHuff::ReadLine(FILE* fIn, string& strInfo)
{
	assert(fIn);

	while (!feof(fIn))
	{
		char ch = fgetc(fIn);
		if (ch == '\n')
			break;

		strInfo += ch;
	}

}


string CompreeFileHuff::GetFilePostFix(const string& fileName)
{
	return fileName.substr(fileName.rfind('.'));
}

//����ѹ����Ϣд��ѹ���ļ���
void CompreeFileHuff::WriteHead(FILE* fOut, const string& fileName)
{
	assert(fOut);
	//1,,��¼�ļ���׺
	string strHead;
	strHead += GetFilePostFix(fileName);
	strHead += '\n';
	//2,��¼�ַ�������Ϣ
	size_t lineCount = 0;
	string strCount;//�ַ�������
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
	//3����¼����
	_itoa(lineCount, szValue, 10);
	strHead += szValue;
	strHead += '\n';

	strHead += strCount;

	fwrite(strHead.c_str(), 1, strHead.size(), fOut);

}


//��ȡ�ַ��ı���
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


