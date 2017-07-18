
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
	void insert(const com &rcon) {
		xInsert(rcon, mRoot); //插入一个元素;
		resetDepth(); //重置最大小深度;
		findDepth(mRoot, 1); //获取最大小深度;
	};
	void insert(com &&rcon) { xInsert(std::move(rcon), mRoot); };
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
		int pNumSpace = int(pow(2.0, dep));
		int pcurHeight = 0;
		int pSpaceStep = 0;
		for (showImg::iterator i = showImage.begin(); i != showImage.end(); ++i)
		{
			++pcurHeight;
			pSpaceStep = int(pNumSpace / pow(2.0, pcurHeight));
			pSpaceStep = pSpaceStep < 20 ? pSpaceStep : 10;
			for (auto j = ++(i->begin()); j != i->end(); ++j)
			{
				for (int s = 0; s < pSpaceStep; s++)
				{
					out << " ";
				}
				out << *j;
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
	void removeNode(const com &pc) {
		xremoveNode(pc, mRoot);
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
		int mHeight;
		//空接口考虑com的初始化问题,若为类，会调用空接口构造函数;
		TreeNode() :mContents(com()),mLeftNode(nullptr), mRightNode(nullptr),mHeight(0) {};
		//初始化接口;
		//height=0;
		TreeNode(const com &pcon, TreeNode* plNode, TreeNode* prNode) :
			mContents(pcon), mLeftNode(plNode), mRightNode(prNode),mHeight(0) {};
		TreeNode(const com &pcon, TreeNode* plNode, TreeNode* prNode, int ph) :
			mContents(pcon), mLeftNode(plNode), mRightNode(prNode), mHeight(ph) {};
		//mContents的右值引用接口,没写;
		//将内容转为int打印;
		int content2int() { return static_cast<int>(mContents); };
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
		xBalance(bst);//平衡树;
	}
	void xInsert(com &&rc, TreeNode *& bst)
	{
		if (bst == nullptr)//迭代到基准情形，即左枝或者右枝为nullptr时,插入当前元素;
			bst = new TreeNode(std::move(rc), nullptr, nullptr);
		else if (rc < bst->mContents)//继续迭代，往左端(小端)前进;
			xInsert(std::move(rc), bst->mLeftNode);
		else if (rc > bst->mContents)//继续迭代，往右端(大端)前进;
			xInsert(std::move(rc), bst->mRightNode);
		else//其他情形;
			;
		xBalance(bst);//平衡树;

	}
	void resetDepth() {
		//每次调用重新查找最大小深度时重置;
		treeMinDepth = MAXINT;
		treeMaxDepth = 0;
	};
	void findDepth(TreeNode* &tn,int d)
	{
		//自己写的查找最大值小深度函数;
		//感觉效率有点低，考虑结合节点高度;
		if (tn==nullptr)
		{
			treeMinDepth = 0;
			treeMaxDepth = 0;
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
			rpShow->push_back(t->content2int());
		}
		else
		{
			rpShow->push_back(t->content2int());
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
	void xremoveNode(const com &pc,TreeNode* &tn) 
	{

		if (tn == nullptr)
			return;
		if (pc < tn->mContents)
			xremoveNode(pc, tn->mLeftNode);
		else if (pc > tn->mContents)
			xremoveNode(pc, tn->mRightNode);
		//pc==tn->mContents,找到目标节点
		else 
			{
				//目标节点有俩子节点;
				//效率不高，右枝最小节点需要查找两次;
				//考虑如何更高效;
				if (tn->mLeftNode!=nullptr&&tn->mRightNode != nullptr)
					{
						//查找用右枝中最小节点，代替当前要删除的节点;
					//	tn->mContents = xgetMin(tn->mRightNode)->mContents;
						//删除右枝中最小节点;
					//	xremoveNode(tn->mContents, tn->mRightNode);
						//尝试更高效的方法;
						tn->mContents = xremoveMin(tn->mRightNode);
					}
				//目标节点至多1个之节点时;
				else
					{
						//复制当前节点指针值;
						TreeNode *oldNode = tn;
						//改变当前节点指针，令当前指针指向子节点;
						//当子节点全为空，tn=nullptr;
						//当存在一个子节点，tn=子节点;
						tn = (tn->mLeftNode != nullptr) ? tn->mLeftNode : tn->mRightNode;
						//删除旧的节点;
						delete oldNode;
					}
			}
	};
	TreeNode* xgetMin(TreeNode* tn)const 
	{
		//迭代方式;
		if (tn==nullptr)
			return nullptr;
		if (tn->mLeftNode==nullptr)
		{
			return tn;
		}
		return xgetMin(tn->mLeftNode);
	};
	TreeNode* xgetMax(TreeNode* tn)const
	{
		//循环方式;
		if (tn == nullptr)
			return nullptr;
		while (tn->mRightNode!=nullptr)
		{
			tn = tn->mRightNode;
		}
		return tn;
	};
	com xremoveMin(TreeNode* &tn)
	{
		//移除最小节点,只查找一次，应该更高效了;
		//当左节点为空时,找到最小节点;
		//关键策略:用右枝中最小节点代替当前节点,因为最小节点不存在左枝;
		//容易使用非左右枝删除策略删除;
		if (tn->mLeftNode==nullptr)
		{
			
			com tmv = tn->mContents;
			TreeNode* oldtn = tn;
			//把最小节点替换为的其右节点;
			tn = tn->mRightNode;
			//删除最小节点;
			delete oldtn;
			//返回最小节点数据;
			return tmv;

		}
		return xremoveMin(tn->mLeftNode);

	};
	static const int MaxDepthDiff = 1;
	inline int getHeight(TreeNode* &tn) const { return tn == nullptr ? -1 : tn->mHeight; };
	void xBalance(TreeNode* &tn)
	{
		if (tn==nullptr)
			return;
		//左枝过深;
		TreeNode* temTn = nullptr;
		if (getHeight(tn->mLeftNode)-getHeight(tn->mRightNode)>MaxDepthDiff)
		{
			temTn = tn->mLeftNode;
			//左枝的左枝深度>=左枝的右枝深度:左外侧;
			if (getHeight(temTn->mLeftNode) >= getHeight(temTn->mRightNode))
				//单旋转;
				xSingleRotate(tn);
				//int x = 1;
			else
				//双旋转;
				int x= 1;

		}
		//右枝过深;
		else if (getHeight(tn->mRightNode) - getHeight(tn->mLeftNode)>MaxDepthDiff)
		{
			temTn = tn->mRightNode;
			//右枝的左枝深度>=左枝的右枝深度;
			if (getHeight(temTn->mRightNode) >= getHeight(temTn->mLeftNode))
				//单旋转;
				xSingleRotate(tn, false);
				/*int x = 1;*/
			else
				//双旋转;
				int x= 1;

		}
		//高度更新;
		//关键在于，当前节点的左右节点都为空时，getHeight函数都返回-1，从而使mHeight=0;
		//当往上回退时，便开始自增;
		tn->mHeight = std::max(getHeight(tn->mLeftNode), getHeight(tn->mRightNode)) + 1;
	};
	
	//单旋转,传入不平衡节点;
	void xSingleRotate(TreeNode* &k2, bool leftleft=true) {
		if (leftleft)
		//左外侧;
		//策略：;

		{
			TreeNode *k1 = k2->mLeftNode;
			k2->mLeftNode = k1->mRightNode;
			k1->mRightNode = k2;
			k2->mHeight = max(getHeight(k2->mLeftNode), getHeight(k2->mRightNode)) + 1;
			k1->mHeight = max(getHeight(k1->mLeftNode), k2->mHeight) + 1;
			k2 = k1;
			int x = 0;
		}
		else//yo外侧;
		{
			TreeNode *k1 = k2->mRightNode;
			k2->mRightNode = k1->mLeftNode;
			k1->mLeftNode = k2;
			k2->mHeight = max(getHeight(k2->mLeftNode), getHeight(k2->mRightNode)) + 1;
			k1->mHeight = max(getHeight(k1->mRightNode), k2->mHeight) + 1;
			k2 = k1;

		}
	};
};//class end


#endif // binary_search_tree_h__
