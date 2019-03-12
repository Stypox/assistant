#ifndef _SENTENCES_COMPILER_PARSER_CONSTRUCTS_ORWORD_HPP_
#define _SENTENCES_COMPILER_PARSER_CONSTRUCTS_ORWORD_HPP_

#include <vector>
#include <string>
#include <ostream>

namespace parser::constructs {
	class OrWord {
		std::vector<std::string> m_words;
		bool m_required;
	public:
		OrWord(const std::vector<std::string>& words, bool required);

		friend std::vector<std::vector<std::string>> unfolded(std::vector<OrWord>::const_iterator thisWord, std::vector<OrWord>::const_iterator lastWord);
		friend std::ostream& operator<< (std::ostream& stream, const OrWord& orWord);
	};
	std::vector<std::vector<std::string>> unfolded(std::vector<OrWord>::const_iterator thisWord, std::vector<OrWord>::const_iterator lastWord);
}

#endif