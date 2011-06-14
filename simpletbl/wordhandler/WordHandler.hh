#ifndef LANGKIT_TAGGER_WORDHANDLER
#define LANGKIT_TAGGER_WORDHANDLER

#include <set>
#include <string>
#include <utility>

#include <simpletbl/utility/PairSecondGtComp.hh>

namespace simpletbl {
namespace tagger {

class WordHandler
{
	typedef std::pair<std::string, double> TagProbPair;
public:
	typedef std::set<TagProbPair,
		simpletbl::utility::PairSecondGtComp<TagProbPair> > OrderedTagProbs;
	virtual OrderedTagProbs tags(std::string const &word) const = 0;
	virtual ~WordHandler() {}
};

}
}

#endif // LANGKIT_TAGGER_WORDHANDLER

