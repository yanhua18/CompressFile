#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include<queue>


template <class w>
struct HuffmanTreeNode
{
	HuffmanTreeNode(const w& value = w())//w������������ͣ����൱��0
		:_pLeft(nullptr)
		, _pRight(nullptr)
		, _pParent(nullptr)
		, _value(value)
	{}
	
	HuffmanTreeNode<w>* _pLeft;
	HuffmanTreeNode<w>* _pRight;
	HuffmanTreeNode<w>* _pParent;
	w _value;//�ڵ��Ȩֵ
};

template<class w>
struct Compare
{
	typedef HuffmanTreeNode<w>* pNode;
	bool operator()(const pNode pLeft,const pNode pRight)//����()
	{
		return pLeft->_value > pRight->_value;
	}
};

template <class w>
class HuffmanTree
{
private:
	HuffmanTreeNode<w>* _pRoot;
public:
	HuffmanTree()
		:_pRoot(nullptr)
	{}

	~HuffmanTree()
	{
		DesTroyTree(_pRoot);//�������Huffman�Ĺ���
	}
#
	void CreatHuffmanTree(const vector<w>& v,const w& invalid)
	{//����ɭ��
		if (v.empty())
		{
			return;
		}
		//�������ȼ�����Ĭ�ϵ��Ǵ�ѣ���Ҫ�Լ������Ƚϵķ���
		//                �ڵ������         �ײ�ʵ�ֵ�����          Ĭ�ϵıȽϹ��򣨰�less�ķ�ʽ��
		priority_queue<HuffmanTreeNode<w>*, vector<HuffmanTreeNode<w>*>, Compare<w>> minheap;
		for (size_t i = 0; i < v.size(); i++)
		{
			if (v[i] != invalid)
			{
				minheap.push(new HuffmanTreeNode<w>(v[i]));
			}
		}
		//��ɭ���Ƿ񳬹�������
		while (minheap.size()>1)
		{
			HuffmanTreeNode<w>* pLeft = minheap.top();
			minheap.pop();
			HuffmanTreeNode<w>* pRight = minheap.top();
			minheap.pop();
			HuffmanTreeNode<w>* pParent = new HuffmanTreeNode<w>(pLeft->_value + pRight->_value);
			pParent->_pLeft = pLeft;
			pLeft->_pParent = pParent;
			pParent->_pRight = pRight;
			pRight->_pParent = pParent;
			minheap.push(pParent);
		}
		//��ɭ��ֻ��һ�����ˣ����������Huffman��
		_pRoot = minheap.top();
	}
	HuffmanTreeNode<w>* GetRoot()
	{
		return _pRoot;
	}

private:
	void DesTroyTree(HuffmanTreeNode<w>* &root)//����Ҫ�����ã���Ϊ��Ҫ�ı�root��ָ��
	{
		if (root == nullptr)//�����Ƿ����
		{
			return;
		}
		//����������ٶ�����
		DesTroyTree(root->_pLeft);
		DesTroyTree(root->_pRight);
		delete root;
		root = nullptr;
	}
};

