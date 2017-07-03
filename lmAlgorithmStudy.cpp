// lmAlgorithmStudy.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "lmfunc.h"
#include "binary_search_tree.h"
#include "lm_vector.h"
#include <ctime>
#include <iostream>
using std::cout; 
using std::endl;
class lmdata
{
public:
	lmdata() :idxe (0), idxb(1){};
	~lmdata() {};

private:
	int idxe;
	int idxb;
};
int main()
{

#if 1
	lmdata ldata;
	int rval=std::rand();
	//vector<int> idata = { 1,2,3,4,5,22,6,7,8,9,10,11,12,13 };//depth=3
	//vector<int> idata = { 6,1,0,10,5,7,18,11,21 };//depth=3
	vector<int> idata = {6,1,0,10,5,7,11};//depth=2
	//vector<int> idata = { 6,1,10,};//depth=1
	//vector<int> idata = {0};//depth=0
	BinSearchTree<int> imtree;
// 	for (auto i = 0; i < 20; ++i)
// 	{
// 		//imtree.insert(*i);
// 		int rval = std::rand();
// 		imtree.insert(rval);
// 	}
	
	for (auto i= idata.begin(); i != idata.end(); ++i)
	{
		imtree.insert(*i);
	}
	BinSearchTree<int> imtreecopy;
	imtreecopy = imtree;
	imtree.printTreeGraph(std::cout);
	std::cout << std::endl;
	int maxdepth = imtree.getMaxDepth();
	int mindepth = imtree.getMinDepth();
	std::cout << "MaxDepth:" << maxdepth << endl;
	std::cout << "MinDepth:" << mindepth << endl;
	bool x = imtree.contains(-1);
	imtreecopy.clearAll();
#endif
#if 0
	double be = clock();
	lm_vector<int> lv(1000000);
	double cu = (double)(clock() - be) / 1.0;
	std::vector<int> vx;
	std::string inWord = "cdefab";// "I am a student.";
	lmfunc lmf;
	lmf.inverseWord(inWord);
#endif
    return 0;
};

