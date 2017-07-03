
#ifndef binary_search_tree_h__
#define binary_search_tree_h__
#include <iostream>
#include <algorithm>
using namespace std;
static const int MAXINT = 0x7FFFFFFF;
template <typename com>
class BinSearchTree
{
public:
	
	explicit BinSearchTree(const com &p):mRoot(new TreeNode(p)) {};
	BinSearchTree():mRoot {nullptr}{};
	//复制构造;
	BinSearchTree(const BinSearchTree &rBST) {
		mRoot = clone(rBST.mRoot); 
		this->treeMinDepth=rBST.getMinDepth(); 
		this->treeMaxDepth = rBST.getMaxDepth();
	};
	//移动构造;
	BinSearchTree(BinSearchTree &&rBST) :mRoot(rBST.mRoot) { rBST.mRoot = nullptr; };
	//复制赋值;
	BinSearchTree& operator=(const BinSearchTree& rBST) {
		BinSearchTree copyBST = rBST; 
		this->treeMaxDepth = copyBST.getMaxDepth();
		this->treeMinDepth = copyBST.getMinDepth();
		std::swap(*this, copyBST); 
		return *this;
	};
	//移动赋值;
	BinSearchTree& operator=(BinSearchTree&& rBST) {
		std::swap(mRoot, rBST.mRoot); return *this;
	};
	~BinSearchTree() { clear(mRoot); };
	void insert(const com &rcon) { xInsert(rcon, mRoot); findDepth(mRoot,1); };
	void insert(com &&rcon) { insert(std::move(rcon), mRoot); };
	typedef vector<vector<int>> showImg;
	void printTreeGraph(ostream & out) {
		if (mRoot == nullptr)
			return;
		showImg showImage;
		
		int dep = treeMaxDepth;
		for (int i = 0; i < treeMaxDepth; ++i)
		{

			showImage.push_back({ 0 });
			//vector<com> & de = showImage[i];
			//for (int j = 0; j < 2 * i; ++j)
			//	de.push_back({ 0 });
		}
		showImg::iterator s = showImage.begin();
		xPrintTreeGraph(mRoot,s);
		for (showImg::iterator i = showImage.begin(); i != showImage.end(); ++i)
		{
			for (auto j = ++(i->begin()); j != i->end();++j)
			{
				out << *j << " " ;
			}
			out << '\n';
		}
		return;
	};
	void printTree(ostream & out) { xPrintTree(mRoot,out); }
	bool contains(const com & p) { return xcontain(p, mRoot); };
	void clearAll() {
		xclearAll(mRoot); treeMaxDepth = 0; treeMinDepth = MAXINT;
	};
	//inline
public:
	inline int getMaxDepth()const { return treeMaxDepth; };
	inline int getMinDepth()const { return treeMinDepth; };
	//inline void setMaxDepth(int p) { treeMaxDepth = p; };
	//inline void setMinDepth(int p) { treeMinDepth = p; };
	//static int TreeMaxDepth(BinSearchTree& bst) { findDepth(bst.mRoot, 1); return bst.getMaxDepth(); };
	//static int TreeMinDepth(BinSearchTree& bst) { findDepth(bst.mRoot, 1); return bst.getMinDepth(); };
private:
	struct TreeNode
	{
		com mContents;
		TreeNode* mLeftNode;
		TreeNode* mRightNode;
		//空接口考虑com的初始化问题;
		TreeNode() :mContents(com()),mLeftNode(nullptr), mRightNode(nullptr) {};
		//初始化接口;
		TreeNode(const com &pcon, TreeNode* plNode, TreeNode* prNode) :
			mContents(pcon), mLeftNode(plNode), mRightNode(prNode) {};
		//mContents的右值引用接口,没写;
	};
	TreeNode* mRoot;
	int treeMinDepth = MAXINT;
	int treeMaxDepth = 0;
	//使用了迭代方式进行复制;
	TreeNode* clone(TreeNode* ptn) 
	{
		if (ptn == nullptr)//迭代到基准,即左或右枝为nullptr;
		{
			return nullptr;
		}
		else//继续迭代;
		{
			return new TreeNode(ptn->mContents, clone(ptn->mLeftNode), clone(ptn->mRightNode));
		}
	};
	//使用了迭代方式进行清空;
	void clear(TreeNode* &tn) 
	{
		if (tn!=nullptr)//继续迭代;
		{
			clear(tn->mLeftNode);
			clear(tn->mRightNode);
			//迭代到基准,即左右枝均为nullptr;
			delete tn;
		}
		tn = nullptr;
	};
	//使用了迭代方式进行插入，重要函数！;
	//模板类型需要提供比较运算符(>,<)操作;
	void xInsert(const com &rc,TreeNode *& bst)
	{
		if (bst == nullptr)//迭代到基准情形，即左枝或者右枝为nullptr时,插入当前元素;
			bst = new TreeNode(rc, nullptr, nullptr);
		else if (rc < bst->mContents)//继续迭代，往左端(小端)前进;
			xInsert(rc, bst->mLeftNode);
		else if (rc > bst->mContents)//继续迭代，往右端(大端)前进;
			xInsert(rc, bst->mRightNode);
		else//其他情形;
			;

	}
	void findDepth(TreeNode* &tn,int d)
	{
		if (tn==nullptr)
		{
			treeMinDepth = 0;
			return;
		}
		if (tn->mLeftNode == nullptr&&tn->mRightNode == nullptr)
		{
			if (d > treeMaxDepth)
				treeMaxDepth = d;
			if (d < treeMinDepth)
				treeMinDepth = d;
		}
		else if (tn->mLeftNode != nullptr&&tn->mRightNode == nullptr)
		{
			findDepth(tn->mLeftNode,d+1);
		}
		else if (tn->mLeftNode == nullptr&&tn->mRightNode != nullptr)
		{
			findDepth(tn->mRightNode,d+1);
		}
		else
		{
			findDepth(tn->mLeftNode, d + 1);
			findDepth(tn->mRightNode, d + 1);
		}

	};
	void printTree(TreeNode *&proot, vector<vector<int>> &pshowImage)
	{
		vector<int> &line= pshowImage::iterator;
		line.push_back(proot->mContents);
	}
	void xPrintTree(TreeNode *t, ostream & out) const
	{

		if (t != nullptr)
		{
			if (t->mLeftNode == nullptr&&t->mRightNode == nullptr)
			{
				out << "/";
			}
			xPrintTree(t->mLeftNode, out);
			out << t->mContents << "  ";
			xPrintTree(t->mRightNode, out);
		}
	}
	void xPrintTreeGraph(TreeNode *t,showImg::iterator rpShow)
	{
		if (t == nullptr)
			return;
		if (t->mLeftNode != nullptr|| t->mRightNode!=nullptr)
		{
			showImg::iterator temit = rpShow;
			++temit;
			xPrintTreeGraph(t->mLeftNode,temit);
			xPrintTreeGraph(t->mRightNode,temit);
			rpShow->push_back(t->mContents);
			//*(temit->begin()) = t->mContents;
		}
		else
		{
			rpShow->push_back(t->mContents);
		}
	}
	bool xcontain(const com &p, TreeNode *&t) const {
		if (t == nullptr)
			return false;
		if (p<t->mContents)
		{
			return xcontain(p, t->mLeftNode);
		}
		else if(p>t->mContents)
		{
			return xcontain(p, t->mRightNode);
		}
		else
		{
			return true;
		}
	};
	void xclearAll(TreeNode* &tn) {
		if (tn = nullptr)return;
		if (tn!=nullptr)
		{
			xclearAll(tn->mLeftNode);
			xclearAll(tn->mRightNode);
			delete tn;
		}
		tn = nullptr;
	};
};


#endif // binary_search_tree_h__
