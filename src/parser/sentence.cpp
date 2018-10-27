#include "sentence.h"

namespace parser {
	Sentence::Sentence(const std::vector<std::string>& words, const std::string& code) :
		m_code{code}, m_words{words} {}

	int Sentence::score(const std::vector<std::string>& words) const {

	}
	void Sentence::exec() const {
		
	}
}