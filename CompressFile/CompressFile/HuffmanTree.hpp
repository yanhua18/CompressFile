#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include<queue>


template <class w>
struct HuffmanTreeNode
{
	HuffmanTreeNode(const w& value = w())//w如果是内置类型，就相当于0
		:_pLeft(nullptr)
		, _pRight(nullptr)
		, _pParent(nullptr)
		, _value(value)
	{}
	
	HuffmanTreeNode<w>* _pLeft;
	HuffmanTreeNode<w>* _pRight;
	HuffmanTreeNode<w>* _pParent;
	w _value;//节点的权值
};

template<class w>
struct Compare
{
	typedef HuffmanTreeNode<w>* pNode;
	bool operator()(const pNode pLeft,const pNode pRight)//重载()
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
		DesTroyTree(_pRoot);//销毁这个Huffman的构造
	}
#
	void CreatHuffmanTree(const vector<w>& v,const w& invalid)
	{//构建森林
		if (v.empty())
		{
			return;
		}
		//由于优先级队列默认的是大堆，需要自己给定比较的方法
		//                节点的类型         底层实现的容器          默认的比较规则（按less的方式）
		priority_queue<HuffmanTreeNode<w>*, vector<HuffmanTreeNode<w>*>, Compare<w>> minheap;
		for (size_t i = 0; i < v.size(); i++)
		{
			if (v[i] != invalid)
			{
				minheap.push(new HuffmanTreeNode<w>(v[i]));
			}
		}
		//看森林是否超过两棵树
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
		//当森林只有一棵树了，这个数就是Huffman数
		_pRoot = minheap.top();
	}
	HuffmanTreeNode<w>* GetRoot()
	{
		return _pRoot;
	}

private:
	void DesTroyTree(HuffmanTreeNode<w>* &root)//必须要是引用，因为需要改变root的指向
	{
		if (root == nullptr)//看树是否存在
		{
			return;
		}
		//后序遍历销毁二叉树
		DesTroyTree(root->_pLeft);
		DesTroyTree(root->_pRight);
		delete root;
		root = nullptr;
	}
};

