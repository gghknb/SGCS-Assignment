#include "Array.h"

class RangeArray : public Array{
	protected:
		int low; //처음인덱스
		int high; //마지막인덱스
		
	public:
		RangeArray(int, int);
		~RangeArray();

		int baseValue(); //
		int endValue();
	
		int& operator[](int);
		int operator[](int) const;
};
