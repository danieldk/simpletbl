#ifndef LANGKIT_UTILITY_PAIRSECONDGTCOMP_HH
#define LANGKIT_UTILITY_PAIRSECONDGTCOMP_HH

namespace simpletbl {
namespace utility {

// This function object class compares two pairs, and returns true when
// the second member of the first pair is greater than the second member
// of the second pair. This can be used for descending ordering on the
// second member.
template <typename T>
struct PairSecondGtComp
{
	bool operator()(T const &p1, T const &p2);
};

template <typename T>
bool PairSecondGtComp<T>::operator()(T const &p1, T const &p2)
{
	if (p1.second != p2.second)
		return p1.second > p2.second;
	else
		return p1.first < p2.first;
}

}	
}

#endif // LANGKIT_UTILITY_PAIRSECONDGTCOMP_HH

