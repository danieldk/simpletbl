#ifndef LANGKIT_TBL_RULE_HH
#define LANGKIT_TBL_RULE_HH

#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <simpletbl/corpus/Sentence.hh>

namespace simpletbl {
namespace tagger {

class TBLRule
{
public:
	enum TBLRuleConditionType {
		TagCondition,
		WordCondition
	};

	struct TBLRuleCondition {
		TBLRuleCondition(int condStart, int condEnd,
			TBLRuleConditionType condType, std::string const &condValue);
		int start;
		int end;
		TBLRuleConditionType type;
		std::string value;
	};

private:
	std::auto_ptr<std::set<TBLRuleCondition> > d_ruleConditions;
	std::auto_ptr<std::string> d_oldTag;
	std::auto_ptr<std::string> d_newTag;
	friend bool operator<(TBLRule const &r1, TBLRule const &r2);
public:
	TBLRule(std::string const &oldTag, std::string const &newTag,
		std::set<TBLRuleCondition> const &conditions);
	TBLRule(TBLRule const &other);
	TBLRule &operator=(TBLRule const &other);
	bool appliesAt(std::vector<simpletbl::corpus::TaggedWord> const
		&corpus, size_t index) const;
	std::set<TBLRuleCondition> const &conditions() const;
	std::string const &newTag() const;
	std::string const &oldTag() const;
protected:
	void copy(TBLRule const &other);
	bool match(TBLRuleConditionType condition,
		simpletbl::corpus::TaggedWord const &taggedWord,
		std::string const &value) const;
};

inline TBLRule::TBLRule(TBLRule const &other)
{
	copy(other);
}

inline std::set<TBLRule::TBLRuleCondition> const &TBLRule::conditions() const
{
	return *d_ruleConditions;
}

inline std::string const &TBLRule::newTag() const
{
	return *d_newTag;
}

inline std::string const &TBLRule::oldTag() const
{
	return *d_oldTag;
}


bool operator<(TBLRule::TBLRuleCondition const &c1,
	TBLRule::TBLRuleCondition const &c2);

bool operator<(TBLRule const &r1, TBLRule const &r2);

}
}

#endif // LANGKIT_TBL_RULE_HH
