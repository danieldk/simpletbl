#include "TBLRule.ih"

TBLRule::TBLRule(string const &oldTag, string const &newTag,
	set<TBLRuleCondition> const &conditions)
{
	d_ruleConditions.reset(new set<TBLRuleCondition>(conditions));
	d_oldTag.reset(new string(oldTag));
	d_newTag.reset(new string(newTag));
}

void TBLRule::copy(TBLRule const &other)
{
	d_ruleConditions.reset(
		new set<TBLRuleCondition>(*other.d_ruleConditions));
	d_oldTag.reset(new string(*other.d_oldTag));
	d_newTag.reset(new string(*other.d_newTag));
}

TBLRule &TBLRule::operator=(TBLRule const &other)
{
	if (this != &other)
		copy(other);
	
	return *this;
}

bool TBLRule::appliesAt(vector<TaggedWord> const &corpus, size_t index) const
{
	if (corpus[index].tag != *d_oldTag)
		return false;

	for (set<TBLRule::TBLRuleCondition>::const_iterator iter =
		d_ruleConditions->begin();
		iter != d_ruleConditions->end();
		++iter)
	{
		// Don't try to read outside the corpus.
		if ((static_cast<int>(index) + iter->start < 0) ||
			(index + iter->end >= corpus.size()))
			return false;

		bool condMatch = false;
		for (int i = iter->start; i <= iter->end; ++i)
			if (match(iter->type, corpus[static_cast<int>(index) + i],
				iter->value)) {
				condMatch = true;
				break;
			}

		if (!condMatch)
			return false;
	}

	return true;
}

bool TBLRule::match(TBLRuleConditionType conditionType,
	simpletbl::corpus::TaggedWord const &taggedWord,
	string const &value) const
{
	if (conditionType == TBLRule::TagCondition)
		return taggedWord.tag == value;
	else
		return taggedWord.word == value;
}

bool simpletbl::tagger::operator<(TBLRule const &r1, TBLRule const &r2)
{
	if (*r1.d_oldTag != *r2.d_oldTag)
		return *r1.d_oldTag < *r2.d_oldTag;
	else if (*r1.d_newTag != *r2.d_newTag)
		return *r1.d_newTag < *r2.d_newTag;
	else
		return *r1.d_ruleConditions < *r2.d_ruleConditions;
}

TBLRule::TBLRuleCondition::TBLRuleCondition(int condStart, int condEnd,
	TBLRuleConditionType condType, std::string const &condValue)
{
	start = condStart;
	end = condEnd;
	type = condType;
	value = condValue;
}

bool simpletbl::tagger::operator<(TBLRule::TBLRuleCondition const &c1,
	TBLRule::TBLRuleCondition const &c2)
{
	if (c1.start != c2.start)
		return c1.start < c2.start;
	if (c1.end != c2.end)
		return c1.end < c2.end;
	else if (c1.type != c2.type)
		return c1.type < c2.type;
	else
		return c1.value < c2.value;
}
