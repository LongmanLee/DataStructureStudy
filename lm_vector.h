#ifndef lm_vector_h__
#define lm_vector_h__

template<typename obj>
class lm_vector
{
public:
	explicit lm_vector(int intisize = 0) :lm_VecSize(intisize), lm_Capacity(intisize + lm_SpareCapacity) { 
		lm_Eles = new obj[lm_Capacity];
		for (int i = 0; i < lm_Capacity; i++)
		{
			lm_Eles[i] = 0;
		}
	};
	lm_vector(lm_vector &rp) :lm_VecSize(rp.lm_VecSize), lm_Capacity(rp.lm_Capacity), lm_Eles(nullptr) {
		lm_Eles = new obj[lm_Capacity];
		for (int i = 0; i < lm_Capacity;++i)
		{
			lm_Eles[i] = rp.lm_Eles[i];
		}
	}
	~lm_vector() { delete[] lm_Eles; };
private:
	static const int lm_SpareCapacity = 2;
	int lm_VecSize;
	int lm_Capacity;
	obj *lm_Eles;
};
#endif // lm_vector_h__
