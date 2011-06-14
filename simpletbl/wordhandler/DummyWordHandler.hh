#ifndef LANGKIT_TAGGER_DUMMYWORDHANDLER
#define LANGKIT_TAGGER_DUMMYWORDHANDLER

#include <string>

#include "WordHandler.hh"

namespace simpletbl {
namespace tagger {

class DummyWordHandler : public WordHandler
{
	std::string d_tag;
	std::string d_capitalTag;
public:
	DummyWordHandler(std::string const &tag, std::string const &capitalTag) :
		d_tag(tag), d_capitalTag(capitalTag) {}
	virtual ~DummyWordHandler ();
	WordHandler::OrderedTagProbs tags(std::string const &word) const;
};

}
}

#endif // LANGKIT_TAGGER_DUMMYWORDHANDLER

