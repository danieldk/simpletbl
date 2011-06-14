#ifndef LANGKIT_TBL_CONTEXTUAL_TAGGER_HH
#define LANGKIT_TBL_CONTEXTUAL_TAGGER_HH

#include <memory>
#include <vector>
#include <simpletbl/corpus/Sentence.hh>
#include "TBLRule.hh"

namespace simpletbl {
namespace tagger {

class TBLContextualTagger
{
	std::auto_ptr<std::vector<TBLRule> > d_rules;
public:
	TBLContextualTagger(std::vector<TBLRule> const &rules);
	TBLContextualTagger(TBLContextualTagger const &other);
	TBLContextualTagger &operator=(TBLContextualTagger const &other);
	std::vector<simpletbl::corpus::TaggedWord> tag(
		std::vector<simpletbl::corpus::TaggedWord> const &initialTaggedWords) const;
private:
	void copy(TBLContextualTagger const &other);
};

inline TBLContextualTagger::TBLContextualTagger(
	std::vector<TBLRule> const &rules)
{
	d_rules.reset(new std::vector<TBLRule>(rules));
}

TBLContextualTagger::TBLContextualTagger(TBLContextualTagger const
	&other)
{
	copy(other);
}

void TBLContextualTagger::copy(TBLContextualTagger const &other)
{
	d_rules.reset(new std::vector<TBLRule>(*other.d_rules));
}

}
}

#endif // LANGKIT_TBL_CONTEXTUAL_TAGGER_HH
