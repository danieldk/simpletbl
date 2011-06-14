#include "BrownCorpusReader.ih"

Corpus<TaggedWord> BrownCorpusReader::read(istream &in)
{
	Corpus<TaggedWord> corpus;
	
	string line;
	while (getline(in, line)) {
		trim(line);

		if (!line.empty())
			corpus.appendSentence(parseLine(line));
	}
	
	return corpus;
}

Sentence<TaggedWord> BrownCorpusReader::parseLine(std::string const &line) {
	Sentence<TaggedWord> sentence;
	
	string::size_type i = 0;
	string::size_type j;
	do {
		// Find the next word separator.
		j = line.find(" ", i);
		
		// Extract the word/tag pair.
		string wordTag = line.substr(i, j - i);
		
		// Find the word/tag delimiter within the word/tag combination.
		// Characters before the separator denote the word, characters
		// after the separator denote the tag.
		string::size_type sepIndex = wordTag.rfind("/");
		TaggedWord taggedWord = {wordTag.substr(0, sepIndex),
			wordTag.substr(sepIndex + 1)};
		sentence.appendWord(taggedWord);
		
		i = j + 1;
	} while (j != string::npos);
	
	return sentence;
}
