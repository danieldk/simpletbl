#ifndef LANGKIT_CORPUS_SENTENCE_HH
#define LANGKIT_CORPUS_SENTENCE_HH

#include <string>
#include <vector>

namespace simpletbl {
namespace corpus {

struct TaggedWord {
	std::string word;
	std::string tag;
};

struct PlainWord {
	std::string word;
};

template <typename T>
class Sentence {
public:
	typedef typename std::vector<T>::const_iterator const_iterator;
	typedef typename std::vector<T>::value_type value_type;
private:
	std::vector<T> d_words;
public:
	void appendWord(T const &word);
	const_iterator begin() const;
	const_iterator end() const;
	std::vector<T> const &words() const;
};

template <typename T>
inline void Sentence<T>::appendWord(T const &word)
{
	d_words.push_back(word);
}

template <typename T>
inline typename Sentence<T>::const_iterator Sentence<T>::begin() const
{
	return d_words.begin();
}

template <typename T>
inline typename Sentence<T>::const_iterator Sentence<T>::end() const
{
	return d_words.end();
}

template <typename T>
inline std::vector<T> const &Sentence<T>::words() const
{
	return d_words;
}

}
}

#endif // LANGKIT_CORPUS_SENTENCE_HH
