#ifndef LANGKIT_INITIAL_TBL_TAGGER_HH
#define LANGKIT_INITIAL_TBL_TAGGER_HH

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <simpletbl/corpus/Sentence.hh>
#include <simpletbl/wordhandler/WordHandler.hh>

namespace simpletbl {
namespace tagger {

class TBLInitialTagger
{
	WordHandler const *d_wordHandler;
public:
	TBLInitialTagger(WordHandler const *wordHandler);
	TBLInitialTagger(TBLInitialTagger const &other);
	TBLInitialTagger &operator=(TBLInitialTagger const &other);
	std::vector<simpletbl::corpus::TaggedWord>
		tag(std::vector<std::string> const &words) const;
private:
	void copy(TBLInitialTagger const &other);
};

inline TBLInitialTagger::TBLInitialTagger(WordHandler const *wordHandler) :
		d_wordHandler(wordHandler)
{}

inline TBLInitialTagger::TBLInitialTagger(TBLInitialTagger const &other)
{
	copy(other);
}

inline void TBLInitialTagger::copy(TBLInitialTagger const &other)
{
	d_wordHandler = other.d_wordHandler;
}

}
}

#endif // LANGKIT_INITIAL_TBL_TAGGER_HH
