#include "KnownWordHandler.ih"

KnownWordHandler &KnownWordHandler::operator=(KnownWordHandler const &other)
{
	if (this != &other)
		copy(other);
	
	return *this;
}

void KnownWordHandler::copy(KnownWordHandler const &other)
{
	d_condProbDist.reset(new StringCondProbDist(*other.d_condProbDist));
	d_fallbackWordHandler = other.d_fallbackWordHandler;
}

WordHandler::OrderedTagProbs KnownWordHandler::tags(string const &word) const
{
	StringCondProbDist::const_iterator match =
		d_condProbDist->find(word);
	if (match != d_condProbDist->end()) {
		OrderedTagProbs results;
		std::copy(match->second.begin(), match->second.end(),
			inserter(results, results.begin()));
		return results;
	}
	else if (d_fallbackWordHandler)
		return d_fallbackWordHandler->tags(word);
	
	return OrderedTagProbs();
}

KnownWordHandler::~KnownWordHandler() {}

