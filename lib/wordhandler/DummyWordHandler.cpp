#include "DummyWordHandler.ih"

DummyWordHandler::~DummyWordHandler()
{
}

WordHandler::OrderedTagProbs DummyWordHandler::tags(string const &word) const
{
	WordHandler::OrderedTagProbs result;
	
	if (isupper(word.at(0)))
		result.insert(make_pair(d_capitalTag, 1.0));
	else
		result.insert(make_pair(d_tag, 1.0));

	return result;
}

