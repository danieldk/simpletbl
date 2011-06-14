#ifndef LANGKIT_CORPUS_HH
#define LANGKIT_CORPUS_HH

#include <vector>

#include "Sentence.hh"

namespace simpletbl {
namespace corpus {

template <typename T>
class Corpus {
public:
	typedef typename std::vector<Sentence<T> >::const_iterator const_iterator;
	typedef typename std::vector<Sentence<T> >::value_type value_type;
private:
	std::vector<Sentence<T> > d_sentences;
public:
	void appendSentence(Sentence<T> const &sentence);
	const_iterator begin() const;
	const_iterator end() const;	
	std::vector<Sentence<T> > const &sentences() const;
};

template <typename T>
inline void Corpus<T>::appendSentence(Sentence<T> const &sentence)
{
	d_sentences.push_back(sentence);
}

template <typename T>
inline typename Corpus<T>::const_iterator Corpus<T>::begin() const
{
	return d_sentences.begin();
}

template <typename T>
inline typename Corpus<T>::const_iterator Corpus<T>::end() const
{
	return d_sentences.end();
}

template <typename T>
std::vector<Sentence<T> > const &Corpus<T>::sentences() const
{
	return d_sentences;
}

}
}

#endif // LANGKIT_CORPUS_HH
