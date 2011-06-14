#include "TBLRuleGenerator.ih"

TBLRuleGenerator::TBLRuleGenerator(Contexts const &tagContexts)
{
	d_contexts.reset(new Contexts(tagContexts));
}

TBLRuleGenerator &TBLRuleGenerator::operator=(TBLRuleGenerator const &other)
{
	if (this != &other)
		copy(other);
	
	return *this;
}

void TBLRuleGenerator::copy(TBLRuleGenerator const &other)
{
	d_contexts.reset(new Contexts(*other.d_contexts));
}

set<TBLRule> TBLRuleGenerator::generate(vector<TaggedWord> const &corpus,
	size_t index, string newTag) const
{
	// Stores possible conditions per context.
	vector<vector<TBLRule::TBLRuleCondition> > allConditions;

	for (Contexts::const_iterator iter = d_contexts->begin();
		iter != d_contexts->end(); ++iter)
	{
		assert(iter->start <= iter->end);

		vector<TBLRule::TBLRuleCondition> curContextConditions;
		
		// Given a context n..m, give all possible conditions for this
		// context.
		for (int i = iter->start; i <= iter->end; ++i) {
			// Determine the value of this condition. The actual value
			// depends on the condition type.
			string value;
			if (iter->type == TBLRule::TagCondition)
				value = corpus[index + i].tag;
			else
				value = corpus[index + i].word;

			curContextConditions.push_back(TBLRule::TBLRuleCondition(
				iter->start,
				iter->end,
				iter->type,
				value
			));
		}

		// Store conditions generated from the context.
		allConditions.push_back(curContextConditions);
	}

	// Generate all possible combinations of context conditions.
	vector<set<TBLRule::TBLRuleCondition> > conditionCombinations;
	for (vector<TBLRule::TBLRuleCondition>::const_iterator iter =
		allConditions[0].begin();
		iter != allConditions[0].end();
		++iter)
	{
		set<TBLRule::TBLRuleCondition> initialConditionSet;
		initialConditionSet.insert(*iter);
		conditionCombinations.push_back(initialConditionSet);
	}

	for (size_t i = 1; i < allConditions.size(); ++i)
	{
		vector<set<TBLRule::TBLRuleCondition> > newCombinations;

		// Get all combinations up till this point, and add all conditions
		// from context i.
		for (vector<set<TBLRule::TBLRuleCondition> >::const_iterator iter =
			conditionCombinations.begin();
			iter != conditionCombinations.end();
			++iter)
		{
			for (size_t j = 0; j < allConditions[i].size(); ++j)
			{
				set<TBLRule::TBLRuleCondition> newCondition = *iter;
				newCondition.insert(allConditions[i][j]);
				newCombinations.push_back(newCondition);
			}
		}

		conditionCombinations = newCombinations;
	}

	string oldTag = corpus[index].tag;

	// Generate rules from the conditions.
	set<TBLRule> rules;
	for (vector<set<TBLRule::TBLRuleCondition> >::const_iterator iter =
		conditionCombinations.begin();
		iter != conditionCombinations.end();
		++iter)
		rules.insert(TBLRule(oldTag, newTag, *iter));

	return rules;
}

bool simpletbl::tagger::operator<(TBLRuleGenerator::Context const &c1,
	TBLRuleGenerator::Context const &c2)
{
	if (c1.start != c2.start)
		return c1.start < c2.start;
	if (c1.end != c2.end)
		return c1.end < c2.end;
	return c1.type < c2.type;
}
