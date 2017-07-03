#ifndef lmfunc_h__
#define lmfunc_h__
#include <vector>
#include <array>
using std::vector;
class lmfunc
{
public:
	lmfunc();
	~lmfunc();
	void findMaxInK(vector<int>::iterator begvec, vector<int>::iterator endvec);
	int lmtest(int ix);
	void lmfunc::inverseWord(std::string &instr);
};

#endif // lmfunc_h__

