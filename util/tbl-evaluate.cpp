#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <boost/bind.hpp>

#include <simpletbl/corpus/BrownCorpusReader.hh>
#include <simpletbl/corpus/Corpus.hh>
#include <simpletbl/corpus/Sentence.hh>
#include <simpletbl/tbl/TBLContextualTagger.hh>
#include <simpletbl/tbl/TBLInitialTagger.hh>
#include <simpletbl/tbl/TBLRule.hh>
#include <simpletbl/wordhandler/DummyWordHandler.hh>
#include <simpletbl/wordhandler/KnownWordHandler.hh>


using namespace std;
using namespace boost;
using namespace simpletbl::corpus;
using namespace simpletbl::tagger;

map<string, map<string, double> > readLexicon(istream &lexiconStream)
{
	map<string, map<string, double> > lexicon;

	string line;
	while (getline(lexiconStream, line)) {
		istringstream lineStream(line);

		string word, tag;
		lineStream >> word >> tag;

		lexicon[word].insert(make_pair(tag, 1.0));
	}

	return lexicon;
}

vector<TBLRule> readRules(istream &ruleStream)
{
	vector<TBLRule> rules;

	string line;
	while (getline(ruleStream, line)) {
		istringstream lineStream(line);

		string oldTag;
		string newTag;

		lineStream >> oldTag >> newTag;

		set<TBLRule::TBLRuleCondition> ruleConditions;

		while(lineStream.good())
		{
			string condString;
			lineStream >> condString;

			// Condition type
			TBLRule::TBLRuleConditionType conditionType;
			if (condString[0] == 't')
				conditionType = TBLRule::TagCondition;
			else
				conditionType = TBLRule::WordCondition;

			// Condition start position.
			string::size_type firstComma = condString.find(",",0);
			istringstream intStream(condString.substr(2, firstComma - 2));
			int start;
			intStream >> start;

			// Condition end position.
			string::size_type secondComma = condString.find(",", firstComma + 1);
			istringstream intStream2(condString.substr(firstComma + 1,
				secondComma - firstComma - 1));
			int end;
			intStream2 >> end;

			// Condition word/tag
			string::size_type closingBracket = condString.find(")", secondComma + 1);
			string tag = condString.substr(secondComma + 1,
				closingBracket - secondComma - 1);
		

			ruleConditions.insert(TBLRule::TBLRuleCondition(start, end,
				conditionType, tag));
		}

		rules.push_back(TBLRule(oldTag, newTag, ruleConditions));
	}

	return rules;
}

void evaluate(vector<TaggedWord> const &corpus1,
	vector<TaggedWord> const &corpus2,
	map<string, map<string, double> > const &lexicon)
{
	size_t knownGood = 0;
	size_t knownBad = 0;
	size_t unknownGood = 0;
	size_t unknownBad = 0;

	for (size_t i = 0; i < corpus1.size(); ++i)
	{
		if (corpus1[i].tag == corpus2[i].tag)
		{
			if (lexicon.find(corpus2[i].word) != lexicon.end())
				++knownGood;
			else
				++unknownGood;
		}
		else
			if (lexicon.find(corpus2[i].word) != lexicon.end())
				++knownBad;
			else
				++unknownBad;
	}

	cout << "Good (known): " << static_cast<float>(knownGood) /
		(knownGood + knownBad) << endl;

	if (unknownGood + unknownBad != 0)
		cout << "Good (unknown): " << static_cast<float>(unknownGood) /
			(unknownGood + unknownBad) << endl;

	cout << "Accuracy: " << static_cast<float>(knownGood + unknownGood) /
		(knownGood + unknownGood + knownBad + unknownBad) << endl;
}

vector<TaggedWord> corpusToTaggedWords(Corpus<TaggedWord> const &corpus)
{
	vector<TaggedWord> sequence;

	vector<Sentence<TaggedWord> > sentences = corpus.sentences();
	for (vector<Sentence<TaggedWord> >::const_iterator iter = sentences.begin();
		iter != sentences.end(); ++iter)
		copy(iter->words().begin(), iter->words().end(),
			back_inserter(sequence));

	return sequence;
}

vector<string> taggedWordsToWords(vector<TaggedWord> const &corpus)
{
	vector<string> sequence;

	transform(corpus.begin(), corpus.end(), back_inserter(sequence),
		bind(&TaggedWord::word, _1));
	
	return sequence;
}

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		cout << "Syntax: " << argv[0] << " lexicon rules evalcorpus" << endl;
		return 1;
	}

	// Read lexicon.
	ifstream lexiconFile(argv[1]);
	map<string, map<string, double> > lexicon = readLexicon(lexiconFile);

	// Read rules.
	ifstream ruleStream(argv[2]);
	vector<TBLRule> rules = readRules(ruleStream);

	// Read corpus.
	ifstream brownCorpusFile(argv[3]);
	Corpus<TaggedWord> *brownCorpus =
		new Corpus<TaggedWord>(BrownCorpusReader::read(brownCorpusFile));

	// Flatten corpus.
	vector<TaggedWord> *brownTaggedWords =
		new vector<TaggedWord>(corpusToTaggedWords(*brownCorpus));

	delete brownCorpus;

	// Extract words.
	vector<string> *brownWords =
		new vector<string>(taggedWordsToWords(*brownTaggedWords));

	// Create word handlers.
	DummyWordHandler dummyWordHandler("NN", "NP");
	KnownWordHandler knownWordHandler(lexicon, &dummyWordHandler);
	
	// Initial tagging of words, based on the lexicon that was provided.
	TBLInitialTagger tagger(&knownWordHandler);

	// Tag.
	vector<TaggedWord> *tagged =
		new vector<TaggedWord>(tagger.tag(*brownWords));
	TBLContextualTagger contextualTagger(rules);
	vector<TaggedWord> *finalTagged =
		new vector<TaggedWord>(contextualTagger.tag(*tagged));

	// Print evaluation information.
	evaluate(*brownTaggedWords, *finalTagged, lexicon);
	
	delete brownTaggedWords;
	delete brownWords;
}
