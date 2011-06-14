#ifndef LANGKIT_TBL_LEARNER_HH
#define LANGKIT_TBL_LEARNER_HH

#include <memory>
#include <utility>
#include <vector>

#include <simpletbl/corpus/Sentence.hh>
#include "TBLRule.hh"
#include "TBLRuleGenerator.hh"

namespace simpletbl {
namespace tagger {

class TBLLearner
{
	struct RuleScoreCmp {
		bool operator()(std::pair<TBLRule, size_t> l, std::pair<TBLRule, size_t> r);
	};

	std::auto_ptr<std::vector<TBLRuleGenerator> > d_ruleGenerators;
	size_t d_changeThreshold;
	size_t d_ruleThreshold;
public:
	TBLLearner(std::vector<TBLRuleGenerator> const &ruleGenerators,
		size_t changeThreshold, size_t ruleThreshold);
	TBLLearner(TBLLearner const &other);
	TBLLearner &operator=(TBLLearner const &other);
	std::vector<TBLRule> learn(
		std::vector<simpletbl::corpus::TaggedWord> const &goldCorpus,
		std::vector<simpletbl::corpus::TaggedWord> const &initialCorpus) const;
private:
	void copy(TBLLearner const &other);
};

inline TBLLearner::TBLLearner(TBLLearner const &other)
{
	copy(other);
}

}
}

#endif // LANGKIT_TBL_LEARNER_HH
