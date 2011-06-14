#include "TBLContextualTagger.ih"

TBLContextualTagger &TBLContextualTagger::operator=(TBLContextualTagger
	const &other)
{
	if (this != &other)
		copy(other);
	
	return *this;
}

vector<TaggedWord> TBLContextualTagger::tag(vector<TaggedWord> const
	&initialTaggedWords) const
{
	map<string, set<size_t> > tagIndices;

	for (size_t i = 0; i < initialTaggedWords.size(); ++i)
		tagIndices[initialTaggedWords[i].tag].insert(i);

	vector<TaggedWord> taggedWords(initialTaggedWords);
	for (vector<TBLRule>::const_iterator iter = d_rules->begin();
		iter != d_rules->end(); ++iter)
	{
		TBLRule const &rule = *iter;

		set<size_t> changedIndices;

		for (set<size_t>::const_iterator changedIndexIter =
			changedIndices.begin(); changedIndexIter != changedIndices.end();
			++changedIndexIter)
		{
			if (rule.appliesAt(taggedWords, *changedIndexIter))
			{
				taggedWords[*changedIndexIter].tag = rule.newTag();
				changedIndices.insert(*changedIndexIter);
			}
		}

		set<size_t> newSet;
		set_difference(tagIndices[rule.oldTag()].begin(),
			tagIndices[rule.oldTag()].end(),
			changedIndices.begin(),
			changedIndices.end(),
			inserter(newSet, newSet.begin()));
		tagIndices[rule.oldTag()] = newSet;

		newSet.clear();
		set_union(tagIndices[rule.newTag()].begin(),
			tagIndices[rule.newTag()].end(),
			changedIndices.begin(),
			changedIndices.end(),
			inserter(newSet, newSet.begin()));
		tagIndices[rule.newTag()] = newSet;
	}

	return taggedWords;
}
