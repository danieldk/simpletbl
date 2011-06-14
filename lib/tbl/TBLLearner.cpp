#include "TBLLearner.ih"

TBLLearner::TBLLearner(vector<TBLRuleGenerator> const &ruleGenerators,
	size_t changeThreshold, size_t ruleThreshold) :
	d_changeThreshold(changeThreshold), d_ruleThreshold(ruleThreshold)
{
	d_ruleGenerators.reset(new vector<TBLRuleGenerator>(ruleGenerators));
}

TBLLearner &TBLLearner::operator=(TBLLearner const &other)
{
	if (this != &other)
		copy(other);
	
	return *this;
}

void TBLLearner::copy(TBLLearner const &other)
{
	d_ruleGenerators.reset(new vector<TBLRuleGenerator>(*other.d_ruleGenerators));
	d_changeThreshold = other.d_changeThreshold;
	d_ruleThreshold = other.d_ruleThreshold;
}

vector<TBLRule> TBLLearner::learn(vector<TaggedWord> const &goldCorpus,
	vector<TaggedWord> const &initialCorpus) const
{
	vector<TaggedWord> *curCorpus = new vector<TaggedWord>(initialCorpus);

	vector<TBLRule> generatedRules;

	// Rule generation loop.
	do {
		map<string, set<size_t> > tagIndices;
		map<TBLRule, size_t> ruleGood;

		for (size_t i = 0; i < curCorpus->size(); ++i)
		{
			string goldTag = goldCorpus[i].tag;
			string dummyTag = (*curCorpus)[i].tag;

			// Register the index of this tag.
			tagIndices[dummyTag].insert(i);

			// If the tag of the dummy corpus does not match that of the gold
			// corpus, generate possible rules for correcting the tag. This
			// is done by applying each rule generator to this corpus position.
			if (dummyTag != goldTag) {
				for(vector<TBLRuleGenerator>::const_iterator iter =
					d_ruleGenerators->begin();
					iter != d_ruleGenerators->end();
					++iter)
				{
					set<TBLRule> rules =
						iter->generate(*curCorpus, i, goldTag);

					// Each generated rule corrects (another) dummy corpus error. 
					for (set<TBLRule>::const_iterator ruleIter = rules.begin();
						ruleIter != rules.end(); ++ruleIter)
						++ruleGood[*ruleIter];
				}
			}
		}

		// Remove rules that have previously been applied from the set.
		for (vector<TBLRule>::const_iterator iter = generatedRules.begin();
			iter != generatedRules.end(); ++iter)
			ruleGood.erase(*iter);

		// Copy the rules to a vector, and sort them in order of the number
		// of tags they correct.
		vector<pair<TBLRule, size_t> > orderedRuleGood;
		std::copy(ruleGood.begin(), ruleGood.end(), back_inserter(orderedRuleGood));
		sort(orderedRuleGood.begin(), orderedRuleGood.end(), RuleScoreCmp());

		int highest = 0;
		TBLRule const *highestRule = 0;

		// Loop through the list of rules, 
		typedef vector<pair<TBLRule, size_t> >::value_type RuleScore;
		for (vector<pair<TBLRule, size_t> >::const_iterator iter =
			orderedRuleGood.begin(); iter != orderedRuleGood.end(); ++iter)
		{
			TBLRule const &rule = iter->first;

			// If the number of tags that the current rule corrects is lower than
			// the overall score of the best rule found at this point, we have
			// already found the best possible rule for this cycle.
			if (static_cast<int>(iter->second) < highest)
				break;

			// Count the number of mistags for this rule.
			int ruleBad = 0;
			for (set<size_t>::const_iterator tagIndexIter = 
				tagIndices[rule.oldTag()].begin();
				tagIndexIter != tagIndices[rule.oldTag()].end();
				++tagIndexIter)
				if (rule.appliesAt(*curCorpus, *tagIndexIter) &&
					goldCorpus[*tagIndexIter].tag != rule.newTag())
					++ruleBad;

			// Calculate the overall rule score, and store this rule if it
			// has the highest score up till this point.
			int score = iter->second - ruleBad;
			if (score > highest) {
				highest = score;
				highestRule = &iter->first;
			}
		}

		// If the number of changes of the best rule is below the threshold,
		// stop rule learning.
		if (highest < static_cast<int>(d_changeThreshold))
			break;

		generatedRules.push_back(*highestRule);

		// Retag the dummy corpus.
		for (set<size_t>::const_iterator iter =
			tagIndices[highestRule->oldTag()].begin();
			iter != tagIndices[highestRule->oldTag()].end();
			++iter)
			if (highestRule->appliesAt(*curCorpus, *iter))
				(*curCorpus)[*iter].tag = highestRule->newTag();

	} while(generatedRules.size() < d_ruleThreshold);

	delete curCorpus;
	
	return generatedRules;
}

bool TBLLearner::RuleScoreCmp::operator()(pair<TBLRule, size_t> l,
	pair<TBLRule, size_t> r)
{
	if (l.second != r.second)
		return l.second > r.second;
	return !(l.first < r.first);
}
