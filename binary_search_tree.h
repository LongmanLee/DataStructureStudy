
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
	//���ƹ���;
	BinSearchTree(const BinSearchTree &rBST) {
		mRoot = clone(rBST.mRoot); 
		this->treeMinDepth=rBST.getMinDepth(); 
		this->treeMaxDepth = rBST.getMaxDepth();
	};
	//�ƶ�����;
	BinSearchTree(BinSearchTree &&rBST) :mRoot(rBST.mRoot) { rBST.mRoot = nullptr; };
	//���Ƹ�ֵ;
	BinSearchTree& operator=(const BinSearchTree& rBST) {
		BinSearchTree copyBST = rBST; 
		this->treeMaxDepth = copyBST.getMaxDepth();
		this->treeMinDepth = copyBST.getMinDepth();
		std::swap(*this, copyBST); 
		return *this;
	};
	//�ƶ���ֵ;
	BinSearchTree& operator=(BinSearchTree&& rBST) {
		std::swap(mRoot, rBST.mRoot); return *this;
	};
	~BinSearchTree() { clear(mRoot); };
	void insert(const com &rcon) {
		xInsert(rcon, mRoot); //����һ��Ԫ��;
		resetDepth(); //�������С���;
		findDepth(mRoot, 1); //��ȡ���С���;
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
		//�սӿڿ���com�ĳ�ʼ������,��Ϊ�࣬����ÿսӿڹ��캯��;
		TreeNode() :mContents(com()),mLeftNode(nullptr), mRightNode(nullptr),mHeight(0) {};
		//��ʼ���ӿ�;
		//height=0;
		TreeNode(const com &pcon, TreeNode* plNode, TreeNode* prNode) :
			mContents(pcon), mLeftNode(plNode), mRightNode(prNode),mHeight(0) {};
		TreeNode(const com &pcon, TreeNode* plNode, TreeNode* prNode, int ph) :
			mContents(pcon), mLeftNode(plNode), mRightNode(prNode), mHeight(ph) {};
		//mContents����ֵ���ýӿ�,ûд;
		//������תΪint��ӡ;
		int content2int() { return static_cast<int>(mContents); };
	};
	TreeNode* mRoot;
	int treeMinDepth = MAXINT;
	int treeMaxDepth = 0;
	//ʹ���˵�����ʽ���и���;
	TreeNode* clone(TreeNode* ptn) 
	{
		if (ptn == nullptr)//��������׼,�������֦Ϊnullptr;
		{
			return nullptr;
		}
		else//��������;
		{
			return new TreeNode(ptn->mContents, clone(ptn->mLeftNode), clone(ptn->mRightNode));
		}
	};
	//ʹ���˵�����ʽ�������;
	void clear(TreeNode* &tn) 
	{
		if (tn!=nullptr)//��������;
		{
			clear(tn->mLeftNode);
			clear(tn->mRightNode);
			//��������׼,������֦��Ϊnullptr;
			delete tn;
		}
		tn = nullptr;
	};
	//ʹ���˵�����ʽ���в��룬��Ҫ������;
	//ģ��������Ҫ�ṩ�Ƚ������(>,<)����;
	void xInsert(const com &rc,TreeNode *& bst)
	{
		if (bst == nullptr)//��������׼���Σ�����֦������֦Ϊnullptrʱ,���뵱ǰԪ��;
			bst = new TreeNode(rc, nullptr, nullptr);
		else if (rc < bst->mContents)//���������������(С��)ǰ��;
			xInsert(rc, bst->mLeftNode);
		else if (rc > bst->mContents)//�������������Ҷ�(���)ǰ��;
			xInsert(rc, bst->mRightNode);
		else//��������;
			;
		xBalance(bst);//ƽ����;
	}
	void xInsert(com &&rc, TreeNode *& bst)
	{
		if (bst == nullptr)//��������׼���Σ�����֦������֦Ϊnullptrʱ,���뵱ǰԪ��;
			bst = new TreeNode(std::move(rc), nullptr, nullptr);
		else if (rc < bst->mContents)//���������������(С��)ǰ��;
			xInsert(std::move(rc), bst->mLeftNode);
		else if (rc > bst->mContents)//�������������Ҷ�(���)ǰ��;
			xInsert(std::move(rc), bst->mRightNode);
		else//��������;
			;
		xBalance(bst);//ƽ����;

	}
	void resetDepth() {
		//ÿ�ε������²������С���ʱ����;
		treeMinDepth = MAXINT;
		treeMaxDepth = 0;
	};
	void findDepth(TreeNode* &tn,int d)
	{
		//�Լ�д�Ĳ������ֵС��Ⱥ���;
		//�о�Ч���е�ͣ����ǽ�Ͻڵ�߶�;
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
		//pc==tn->mContents,�ҵ�Ŀ��ڵ�
		else 
			{
				//Ŀ��ڵ������ӽڵ�;
				//Ч�ʲ��ߣ���֦��С�ڵ���Ҫ��������;
				//������θ���Ч;
				if (tn->mLeftNode!=nullptr&&tn->mRightNode != nullptr)
					{
						//��������֦����С�ڵ㣬���浱ǰҪɾ���Ľڵ�;
					//	tn->mContents = xgetMin(tn->mRightNode)->mContents;
						//ɾ����֦����С�ڵ�;
					//	xremoveNode(tn->mContents, tn->mRightNode);
						//���Ը���Ч�ķ���;
						tn->mContents = xremoveMin(tn->mRightNode);
					}
				//Ŀ��ڵ�����1��֮�ڵ�ʱ;
				else
					{
						//���Ƶ�ǰ�ڵ�ָ��ֵ;
						TreeNode *oldNode = tn;
						//�ı䵱ǰ�ڵ�ָ�룬�ǰָ��ָ���ӽڵ�;
						//���ӽڵ�ȫΪ�գ�tn=nullptr;
						//������һ���ӽڵ㣬tn=�ӽڵ�;
						tn = (tn->mLeftNode != nullptr) ? tn->mLeftNode : tn->mRightNode;
						//ɾ���ɵĽڵ�;
						delete oldNode;
					}
			}
	};
	TreeNode* xgetMin(TreeNode* tn)const 
	{
		//������ʽ;
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
		//ѭ����ʽ;
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
		//�Ƴ���С�ڵ�,ֻ����һ�Σ�Ӧ�ø���Ч��;
		//����ڵ�Ϊ��ʱ,�ҵ���С�ڵ�;
		//�ؼ�����:����֦����С�ڵ���浱ǰ�ڵ�,��Ϊ��С�ڵ㲻������֦;
		//����ʹ�÷�����֦ɾ������ɾ��;
		if (tn->mLeftNode==nullptr)
		{
			
			com tmv = tn->mContents;
			TreeNode* oldtn = tn;
			//����С�ڵ��滻Ϊ�����ҽڵ�;
			tn = tn->mRightNode;
			//ɾ����С�ڵ�;
			delete oldtn;
			//������С�ڵ�����;
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
		//��֦����;
		TreeNode* temTn = nullptr;
		if (getHeight(tn->mLeftNode)-getHeight(tn->mRightNode)>MaxDepthDiff)
		{
			temTn = tn->mLeftNode;
			//��֦����֦���>=��֦����֦���:�����;
			if (getHeight(temTn->mLeftNode) >= getHeight(temTn->mRightNode))
				//����ת;
				xSingleRotate(tn);
				//int x = 1;
			else
				//˫��ת;
				int x= 1;

		}
		//��֦����;
		else if (getHeight(tn->mRightNode) - getHeight(tn->mLeftNode)>MaxDepthDiff)
		{
			temTn = tn->mRightNode;
			//��֦����֦���>=��֦����֦���;
			if (getHeight(temTn->mRightNode) >= getHeight(temTn->mLeftNode))
				//����ת;
				xSingleRotate(tn, false);
				/*int x = 1;*/
			else
				//˫��ת;
				int x= 1;

		}
		//�߶ȸ���;
		//�ؼ����ڣ���ǰ�ڵ�����ҽڵ㶼Ϊ��ʱ��getHeight����������-1���Ӷ�ʹmHeight=0;
		//�����ϻ���ʱ���㿪ʼ����;
		tn->mHeight = std::max(getHeight(tn->mLeftNode), getHeight(tn->mRightNode)) + 1;
	};
	
	//����ת,���벻ƽ��ڵ�;
	void xSingleRotate(TreeNode* &k2, bool leftleft=true) {
		if (leftleft)
		//�����;
		//���ԣ�;

		{
			TreeNode *k1 = k2->mLeftNode;
			k2->mLeftNode = k1->mRightNode;
			k1->mRightNode = k2;
			k2->mHeight = max(getHeight(k2->mLeftNode), getHeight(k2->mRightNode)) + 1;
			k1->mHeight = max(getHeight(k1->mLeftNode), k2->mHeight) + 1;
			k2 = k1;
			int x = 0;
		}
		else//yo���;
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
