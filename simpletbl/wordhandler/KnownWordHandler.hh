#ifndef LANGKIT_TAGGER_KNOWNWORDHANDLER
#define LANGKIT_TAGGER_KNOWNWORDHANDLER

#include <map>
#include <memory>
#include <string>

#include "WordHandler.hh"

namespace simpletbl {
namespace tagger {

class KnownWordHandler : public WordHandler
{
	typedef std::map<std::string, std::map<std::string, double> >
		StringCondProbDist;
	std::auto_ptr<StringCondProbDist> d_condProbDist;
	WordHandler const *d_fallbackWordHandler;
public:
	KnownWordHandler(StringCondProbDist	const &condProbDist,
		WordHandler const *fallbackWordHandler) :
		d_condProbDist(new StringCondProbDist(condProbDist)),
		d_fallbackWordHandler(fallbackWordHandler)
		{}
	KnownWordHandler(KnownWordHandler const &other) :
		d_condProbDist(new StringCondProbDist(*other.d_condProbDist)),
		d_fallbackWordHandler(other.d_fallbackWordHandler)
		{}
	virtual ~KnownWordHandler();
	KnownWordHandler &operator=(KnownWordHandler const &other);
	WordHandler::OrderedTagProbs tags(std::string const &word) const;
private:
	void copy(KnownWordHandler const &other);
};

}
}

#endif // LANGKIT_TAGGER_KNOWNWORDHANDLER

