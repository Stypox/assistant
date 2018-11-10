#ifndef _VOICE_ASSISTANT_PARSER_PARSED_SENTENCE_H_
#define _VOICE_ASSISTANT_PARSER_PARSED_SENTENCE_H_

#include <vector>
#include <string>
#include <memory>

#include "sentence.h"

namespace parser {
	constexpr const char* idWhenInvalid= "",
		* codeWhenInvalid = "invalid";

	class ParsedSentenceBase {
		std::string m_id, m_code;
	
	public:
		ParsedSentenceBase(std::string id, std::string code);
	};

	class ParsedSentence : public ParsedSentenceBase {
		std::vector<std::string>
			m_insertedWords,
			m_sentenceWords;
		
	public:
		ParsedSentence(Sentence sentence, std::vector<std::string> insertedWords);
	};

	class ParsedCapturingSentence : public ParsedSentenceBase {
		std::vector<std::string>
			m_insertedWords,
			m_capturedWords,
			m_sentenceWordsBefore,
			m_sentenceWordsAfter;
		
	public:
		ParsedCapturingSentence(CapturingSentence sentence, std::vector<std::string> insertedWords, std::vector<std::string> capturedWords);
	};

	class InvalidSentence : public ParsedSentenceBase {
		std::vector<std::string>
			m_insertedWords;
		
	public:
		InvalidSentence(std::vector<std::string> insertedWords);
	};

	std::unique_ptr<ParsedSentenceBase> makeParsed(Sentence sentence, std::vector<std::string> insertedWords);
	std::unique_ptr<ParsedSentenceBase> makeParsed(CapturingSentence sentence, std::vector<std::string> insertedWords, std::vector<std::string> capturedWords);
	std::unique_ptr<ParsedSentenceBase> makeParsed(std::vector<std::string> insertedWords);
}

#endif