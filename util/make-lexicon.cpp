#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include <simpletbl/corpus/BrownCorpusReader.hh>
#include <simpletbl/corpus/Corpus.hh>
#include <simpletbl/corpus/Sentence.hh>

using namespace std;
using namespace simpletbl::corpus;

map<string, map<string, double> > readLexicon(Corpus<TaggedWord> const &corpus)
{
	map<string, map<string, size_t> > wordFreqs;

	for (Corpus<TaggedWord>::const_iterator iter = corpus.begin();
		iter != corpus.end(); ++iter)
		for (Sentence<TaggedWord>::const_iterator sentenceIter = iter->begin();
			sentenceIter != iter->end(); ++sentenceIter)
			++wordFreqs[sentenceIter->word][sentenceIter->tag];

	map<string, map<string, double> > wordProbs;

	return wordProbs;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "Syntax: " << argv[0] << " corpus" << endl;
		return 1;
	}

	// Read corpus
	ifstream corpusFile(argv[1]);
	Corpus<TaggedWord> corpus = BrownCorpusReader::read(corpusFile);

}
