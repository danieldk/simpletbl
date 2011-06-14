#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

#include <simpletbl/corpus/BrownCorpusReader.hh>
#include <simpletbl/corpus/Corpus.hh>
#include <simpletbl/corpus/Sentence.hh>
#include <simpletbl/tbl/TBLLearner.hh>
#include <simpletbl/tbl/TBLRule.hh>
#include <simpletbl/tbl/TBLRuleGenerator.hh>

using namespace std;
using namespace simpletbl::corpus;
using namespace simpletbl::tagger;

map<string,string> readLexicon(istream &lexiconStream)
{
	map<string,string> lexicon;

	string line;
	while (getline(lexiconStream, line)) {
		istringstream lineStream(line);

		string word, tag;
		lineStream >> word >> tag;

		lexicon[word] = tag;
	}

	return lexicon;
}

vector<TaggedWord> corpusToTaggedWords(Corpus<TaggedWord> const &corpus)
{
	vector<TaggedWord> sequence;

	vector<Sentence<TaggedWord> > sentences = corpus.sentences();

	for (vector<Sentence<TaggedWord> >::const_iterator iter = sentences.begin();
		iter != sentences.end(); ++iter)
	{
		// Add begin of sentence markers.
		TaggedWord staart = {"STAART", "STAART"};
		sequence.push_back(staart);

		copy(iter->words().begin(), iter->words().end(),
			back_inserter(sequence));

		// Add end of sentence markers.
		sequence.push_back(staart);
	}

	return sequence;
}

int main(int argc, char *argv[])
{
	if (argc != 5)
	{
		cout << "Syntax: " << argv[0] <<
			" maxrules changethreshold goldstandard dummy" << endl;
		return 1;
	}

	// Get learning thresholds
	istringstream maxRulesStream(argv[1]);
	size_t maxRules;
	maxRulesStream >> maxRules;

	istringstream changeThreshStream(argv[2]);
	size_t changeThresh;
	changeThreshStream >> changeThresh; 

	// Tag-based rule generators 
	// NEXTTAG
	TBLRuleGenerator::Contexts nextTagContexts;
	nextTagContexts.insert(TBLRuleGenerator::Context(1, 1,
		TBLRule::TagCondition));
	TBLRuleGenerator nextTagRuleGen(nextTagContexts);

	// NEXT2TAG
	TBLRuleGenerator::Contexts next2TagContexts;
	next2TagContexts.insert(TBLRuleGenerator::Context(2, 2,
		TBLRule::TagCondition));
	TBLRuleGenerator next2TagRuleGen(next2TagContexts);

	// PREVTAG
	TBLRuleGenerator::Contexts prevTagContexts;
	prevTagContexts.insert(TBLRuleGenerator::Context(-1,-1,
		TBLRule::TagCondition));
	TBLRuleGenerator prevTagRuleGen(prevTagContexts);

	// PREV2TAG
	TBLRuleGenerator::Contexts prev2TagContexts;
	prev2TagContexts.insert(TBLRuleGenerator::Context(-2,-2,
		TBLRule::TagCondition));
	TBLRuleGenerator prev2TagRuleGen(prev2TagContexts);

	// NEXT1OR2TAG
	TBLRuleGenerator::Contexts next1or2TagContexts;
	next1or2TagContexts.insert(TBLRuleGenerator::Context(1,2,
		TBLRule::TagCondition));
	TBLRuleGenerator next1or2TagRuleGen(next1or2TagContexts);

	// NEXT1OR2OR3TAG
	TBLRuleGenerator::Contexts next1or2or3TagContexts;
	next1or2or3TagContexts.insert(TBLRuleGenerator::Context(1,3,
		TBLRule::TagCondition));
	TBLRuleGenerator next1or2or3TagRuleGen(next1or2or3TagContexts);

	// PREV1OR2TAG
	TBLRuleGenerator::Contexts prev1or2TagContexts;
	prev1or2TagContexts.insert(TBLRuleGenerator::Context(-2,-1,
		TBLRule::TagCondition));
	TBLRuleGenerator prev1or2TagRuleGen(prev1or2TagContexts);

	// PREV1OR2OR3TAG
	TBLRuleGenerator::Contexts prev1or2or3TagContexts;
	prev1or2or3TagContexts.insert(TBLRuleGenerator::Context(-3,-1,
		TBLRule::TagCondition));
	TBLRuleGenerator prev1or2or3TagRuleGen(prev1or2or3TagContexts);

	// SURROUNDTAG
	TBLRuleGenerator::Contexts surroundTagContexts;
	surroundTagContexts.insert(TBLRuleGenerator::Context(-1,-1,
		TBLRule::TagCondition));
	surroundTagContexts.insert(TBLRuleGenerator::Context(1,1,
		TBLRule::TagCondition));
	TBLRuleGenerator surroundTagRuleGen(surroundTagContexts);

	// PREVBIGRAM
	TBLRuleGenerator::Contexts prevBigramContexts;
	prevBigramContexts.insert(TBLRuleGenerator::Context(-2,-2,
		TBLRule::TagCondition));
	prevBigramContexts.insert(TBLRuleGenerator::Context(-1,-1,
		TBLRule::TagCondition));
	TBLRuleGenerator prevBigramRuleGen(prevBigramContexts);

	// NEXTBIGRAM
	TBLRuleGenerator::Contexts nextBigramContexts;
	nextBigramContexts.insert(TBLRuleGenerator::Context(1, 1,
		TBLRule::TagCondition));
	nextBigramContexts.insert(TBLRuleGenerator::Context(2, 2,
		TBLRule::TagCondition));
	TBLRuleGenerator nextBigramRuleGen(nextBigramContexts);

	// Word-based rule generators.
	// CURWD
	TBLRuleGenerator::Contexts curWdContexts;
	curWdContexts.insert(TBLRuleGenerator::Context(0, 0,
		TBLRule::WordCondition));
	TBLRuleGenerator curWdRuleGen(curWdContexts);

	// LBIGRAM 
	TBLRuleGenerator::Contexts lBigramContexts;
	lBigramContexts.insert(TBLRuleGenerator::Context(-1, -1,
		TBLRule::WordCondition));
	lBigramContexts.insert(TBLRuleGenerator::Context(0, 0,
		TBLRule::WordCondition));
	TBLRuleGenerator lBigramRuleGen(lBigramContexts);

	// PREVWD
	TBLRuleGenerator::Contexts prevWdContexts;
	prevWdContexts.insert(TBLRuleGenerator::Context(-1, -1,
		TBLRule::WordCondition));
	TBLRuleGenerator prevWdRuleGen(prevWdContexts);

	// PREV1OR2WD
	TBLRuleGenerator::Contexts prev1Or2WdContexts;
	prev1Or2WdContexts.insert(TBLRuleGenerator::Context(-2, -1,
		TBLRule::WordCondition));
	TBLRuleGenerator prev1Or2WdRuleGen(prev1Or2WdContexts);

	// PREV1OR2OR3WD
	TBLRuleGenerator::Contexts prev1Or2Or3WdContexts;
	prev1Or2Or3WdContexts.insert(TBLRuleGenerator::Context(-3, -1,
		TBLRule::WordCondition));
	TBLRuleGenerator prev1Or2Or3WdRuleGen(prev1Or2Or3WdContexts);

	// PREV2WD
	TBLRuleGenerator::Contexts prev2WdContexts;
	prev2WdContexts.insert(TBLRuleGenerator::Context(-2, -2,
		TBLRule::WordCondition));
	TBLRuleGenerator prev2WdRuleGen(prev2WdContexts);

	// NEXTWD
	TBLRuleGenerator::Contexts nextWdContexts;
	nextWdContexts.insert(TBLRuleGenerator::Context(1, 1,
		TBLRule::WordCondition));
	TBLRuleGenerator nextWdRuleGen(nextWdContexts);

	// NEXT1OR2WD
	TBLRuleGenerator::Contexts next1Or2WdContexts;
	next1Or2WdContexts.insert(TBLRuleGenerator::Context(1, 2,
		TBLRule::WordCondition));
	TBLRuleGenerator next1Or2WdRuleGen(next1Or2WdContexts);

	// NEXT1OR2OR3WD
	TBLRuleGenerator::Contexts next1Or2Or3WdContexts;
	next1Or2Or3WdContexts.insert(TBLRuleGenerator::Context(1, 3,
		TBLRule::WordCondition));
	TBLRuleGenerator next1Or2Or3WdRuleGen(next1Or2Or3WdContexts);

	// NEXT2WD
	TBLRuleGenerator::Contexts next2WdContexts;
	next2WdContexts.insert(TBLRuleGenerator::Context(2, 2,
		TBLRule::WordCondition));
	TBLRuleGenerator next2WdRuleGen(next2WdContexts);

	// RBIGRAM 
	TBLRuleGenerator::Contexts rBigramContexts;
	rBigramContexts.insert(TBLRuleGenerator::Context(0, 0,
		TBLRule::WordCondition));
	rBigramContexts.insert(TBLRuleGenerator::Context(1, 1,
		TBLRule::WordCondition));
	TBLRuleGenerator rBigramRuleGen(rBigramContexts);

	// SURROUNDWD
	TBLRuleGenerator::Contexts surroundWdContexts;
	surroundWdContexts.insert(TBLRuleGenerator::Context(-1, -1,
		TBLRule::WordCondition));
	surroundWdContexts.insert(TBLRuleGenerator::Context(1, 1,
		TBLRule::WordCondition));
	TBLRuleGenerator surroundWdRuleGen(surroundWdContexts);

	// WDAND2AFT 
	TBLRuleGenerator::Contexts wdAnd2AftContexts;
	wdAnd2AftContexts.insert(TBLRuleGenerator::Context(0, 0,
		TBLRule::WordCondition));
	wdAnd2AftContexts.insert(TBLRuleGenerator::Context(2, 2,
		TBLRule::TagCondition));
	TBLRuleGenerator wdAnd2AftRuleGen(wdAnd2AftContexts);

	// WDAND2BFR
	TBLRuleGenerator::Contexts wdAnd2BfrContexts;
	wdAnd2BfrContexts.insert(TBLRuleGenerator::Context(-2, -2,
		TBLRule::TagCondition));
	wdAnd2BfrContexts.insert(TBLRuleGenerator::Context(0, 0,
		TBLRule::WordCondition));
	TBLRuleGenerator wdAnd2BfrRuleGen(wdAnd2BfrContexts);

	vector<TBLRuleGenerator> ruleGenerators;
	ruleGenerators.push_back(nextTagRuleGen);
	ruleGenerators.push_back(next2TagRuleGen);
	ruleGenerators.push_back(next1or2TagRuleGen);
	ruleGenerators.push_back(next1or2or3TagRuleGen);
	ruleGenerators.push_back(prevTagRuleGen);
	ruleGenerators.push_back(prev2TagRuleGen);
	ruleGenerators.push_back(prev1or2TagRuleGen);
	ruleGenerators.push_back(prev1or2or3TagRuleGen);
	ruleGenerators.push_back(surroundTagRuleGen);
	ruleGenerators.push_back(prevBigramRuleGen);
	ruleGenerators.push_back(nextBigramRuleGen);

	ruleGenerators.push_back(curWdRuleGen);
	ruleGenerators.push_back(lBigramRuleGen);
	ruleGenerators.push_back(prevWdRuleGen);
	ruleGenerators.push_back(prev1Or2WdRuleGen);
	ruleGenerators.push_back(prev1Or2Or3WdRuleGen);
	ruleGenerators.push_back(prev2WdRuleGen);
	ruleGenerators.push_back(nextWdRuleGen);
	ruleGenerators.push_back(next1Or2WdRuleGen);
	ruleGenerators.push_back(next1Or2Or3WdRuleGen);
	ruleGenerators.push_back(next2WdRuleGen);
	ruleGenerators.push_back(rBigramRuleGen);
	ruleGenerators.push_back(surroundWdRuleGen);

	ruleGenerators.push_back(wdAnd2AftRuleGen);
	ruleGenerators.push_back(wdAnd2BfrRuleGen);

	TBLLearner tblLearner(ruleGenerators, changeThresh, maxRules);

	// Read gold standard corpus
	ifstream goldCorpusFile(argv[3]);
	Corpus<TaggedWord> *goldCorpus = new Corpus<TaggedWord>(BrownCorpusReader::read(goldCorpusFile));

	// Flatten gold standard corpus
	vector<TaggedWord> *goldTaggedWords = new vector<TaggedWord>(corpusToTaggedWords(*goldCorpus));

	// Read dummy corpus
	ifstream dummyCorpusFile(argv[4]);
	Corpus<TaggedWord> *dummyCorpus = new Corpus<TaggedWord>(BrownCorpusReader::read(dummyCorpusFile));

	// Flatten dummy corpus
	vector<TaggedWord> *dummyTaggedWords = new vector<TaggedWord>(corpusToTaggedWords(*dummyCorpus));

	// Learn rules.
	vector<TBLRule> rules = tblLearner.learn(*goldTaggedWords, *dummyTaggedWords);

	// Print learned rules.
	for (vector<TBLRule>::const_iterator iter = rules.begin();
		iter != rules.end(); ++iter)
	{
		cout << iter->oldTag() << " " << iter->newTag();
		set<TBLRule::TBLRuleCondition> ruleConditions = iter->conditions();
		for (set<TBLRule::TBLRuleCondition>::const_iterator conditionIter = 
			ruleConditions.begin();
			conditionIter != ruleConditions.end();
			++conditionIter)
		{
			if (conditionIter->type == TBLRule::TagCondition)
				cout << " t(";
			else
				cout << " w(";
			cout << conditionIter->start << "," << conditionIter->end <<
				"," << conditionIter->value << ")";
		}
		cout << endl;
	}

	delete goldCorpus;
	delete goldTaggedWords;
	delete dummyCorpus;
	delete dummyTaggedWords;
}
