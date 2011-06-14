#include "TBLInitialTagger.ih"

TBLInitialTagger &TBLInitialTagger::operator=(
	TBLInitialTagger const &other)
{
	if (this != &other)
		copy(other);
	
	return *this;
}

vector<TaggedWord> TBLInitialTagger::tag(vector<string> const &words) const
{
	vector<TaggedWord> initialTagging;

	for(vector<string>::const_iterator iter = words.begin(); iter != words.end();
		++iter)
	{
		TaggedWord taggedWord;
		taggedWord.word = *iter;

		taggedWord.tag = d_wordHandler->tags(*iter).begin()->first;

		initialTagging.push_back(taggedWord);
	}

	return initialTagging;
}
