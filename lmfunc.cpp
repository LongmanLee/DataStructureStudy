#include "stdafx.h"
#include "lmfunc.h"


lmfunc::lmfunc()
{
}
lmfunc::~lmfunc()
{
}
void lmfunc::findMaxInK(vector<int>::iterator begvec, vector<int>::iterator endvec)
{

}

int lmfunc::lmtest(int ix)
{
	if (!ix)
	{
		return 0;
	}
	else
	{
		return lmtest(ix - 1)+ix;
	}
}

//字符翻转,自己写的迭代法;
//首尾交换更快？;迭代法初步分析复杂度为O(NlogN)
void lmfunc::inverseWord(std::string &instr)
{
	int len = instr.length();
	if (len<=1)//基准情况，一个字母;
		return;
	int s = len / 2;
	int ss = len % 2;
	std::string frontStr, backStr;
	for (int i = 0; i < s;++i)//复杂度O(n/2)
	{
		frontStr.push_back(instr[i]);
		backStr.push_back(instr[len - s + i]);
	}
	inverseWord(frontStr);
	inverseWord(backStr);
	if (ss)
		instr = backStr + instr[s] + frontStr;
	else
		instr = backStr + frontStr;

}
