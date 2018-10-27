#ifndef _VOICE_ASSISTANT_PARSER_SENTENCE_H_
#define _VOICE_ASSISTANT_PARSER_SENTENCE_H_

#include <vector>
#include <string>

namespace parser {
	constexpr int pointsAtBeginning = 0,
		pointsFoundWord = 4,
		pointsWordsInMiddle = -1,
		pointsFoundAllWords = 10,
		pointsExactMatch = 5;

	class Sentence {
		const std::vector<std::string> m_words;
		const std::string& m_code;
	public:
		Sentence(const std::vector<std::string>& words, const std::string& code);

		int score(const std::vector<std::string>& compareWords) const;
		void exec() const;
	};
}

#endif