#ifndef LANGKIT_BROWNCORPUSREADER_HH
#define LANGKIT_BROWNCORPUSREADER_HH

#include <istream>
#include <string>

#include "Corpus.hh"
#include "Sentence.hh"

namespace simpletbl {
namespace corpus {

class BrownCorpusReader {
public:
	static Corpus<TaggedWord> read(std::istream &in);
private:
	BrownCorpusReader();
	BrownCorpusReader(BrownCorpusReader const &other);
	BrownCorpusReader &operator=(BrownCorpusReader const &other);
	static Sentence<TaggedWord> parseLine(std::string const &line);
};

}
}

#endif // LANGKIT_BROWNCORPUSREADER_HH
