
#ifndef ARRBASE_HPP
#define ARRBASE_HPP

// Simplest structure giving a sized array. 
// Through the ArrBase class, all ops that don't increase the array size can be done
// This has the same data element layout as the ExpArr class
template<class T, class INT=int>
class ArrBase {
public:
	T   *ta;
	INT size;
};

#endif // ARRBASE_HPP


