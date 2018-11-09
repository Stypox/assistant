#ifndef _VOICE_ASSISTANT_PARSER_PARSED_SENTENCE_H_
#define _VOICE_ASSISTANT_PARSER_PARSED_SENTENCE_H_

#include <vector>
#include <string>

#include "sentence.h"

namespace parser {
	class ParsedSentenceBase {
		std::string m_code, m_id;
	};

	class ParsedSentence : ParsedSentenceBase {
		std::vector<std::string>
			m_insertedWords,
			m_sentenceWords;
		
	public:
		ParsedSentence(Sentence sentence, std::vector<std::string> insertedWords);
	};

	class ParsedCapturingSentence : ParsedSentenceBase {
		std::vector<std::string>
			m_insertedWords,
			m_capturedWords,
			m_sentenceWordsBefore,
			m_sentenceWordsAfter;
		
	public:
		ParsedCapturingSentence(CapturingSentence sentence, std::vector<std::string> insertedWords, std::vector<std::string> captuedWords);
	};

	class InvalidSentence : ParsedSentenceBase {
		std::vector<std::string>
			m_insertedWords;
		
	public:
		InvalidSentence(std::vector<std::string> insertedWords);
	};
}

#endif