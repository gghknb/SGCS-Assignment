#include "Array.h"

class RangeArray : public Array{
	protected:
		int low; //ó���ε���
		int high; //�������ε���
		
	public:
		RangeArray(int, int);
		~RangeArray();

		int baseValue(); //
		int endValue();
	
		int& operator[](int);
		int operator[](int) const;
};
