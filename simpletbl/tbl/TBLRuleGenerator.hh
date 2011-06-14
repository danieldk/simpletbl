#ifndef LANGKIT_TBL_RULE_GENERATOR_HH
#define LANGKIT_TBL_RULE_GENERATOR_HH

#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "TBLRule.hh"
#include <simpletbl/corpus/Sentence.hh>

namespace simpletbl {
namespace tagger {

// This class generates a set of TBL rules, given a predefined context,
// and corpus material.
class TBLRuleGenerator
{
public:
	struct Context {
		Context(int newStart, int newEnd, TBLRule::TBLRuleConditionType newType) :
			start(newStart), end(newEnd), type(newType) {}
		int start;
		int end;
		TBLRule::TBLRuleConditionType type;
	};

	typedef std::set<Context> Contexts;
private:
	std::auto_ptr<Contexts> d_contexts;
public:
	TBLRuleGenerator(Contexts const &tagContexts);
	TBLRuleGenerator(TBLRuleGenerator const &other);
	TBLRuleGenerator &operator=(TBLRuleGenerator const &other);
	std::set<TBLRule> generate(std::vector<simpletbl::corpus::TaggedWord>
		const &corpus,
		size_t index, std::string newTag) const;
private:
	void copy(TBLRuleGenerator const &other);
};

inline TBLRuleGenerator::TBLRuleGenerator(TBLRuleGenerator const &other)
{
	copy(other);
}

bool operator<(TBLRuleGenerator::Context const &c1,
	TBLRuleGenerator::Context const &c2);

}
}

#endif // LANGKIT_TBL_RULE_GENERATOR_HH
