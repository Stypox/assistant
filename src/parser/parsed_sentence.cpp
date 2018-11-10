#include "parsed_sentence.h"

namespace parser {	
	ParsedSentenceBase::ParsedSentenceBase(std::string id, std::string code) :
		m_id{id}, m_code{code} {}

	ParsedSentence::ParsedSentence(Sentence sentence, std::vector<std::string> insertedWords) :
		ParsedSentenceBase{sentence.m_id, sentence.m_code}, m_insertedWords{insertedWords},
		m_sentenceWords{sentence.m_words} {}

	ParsedCapturingSentence::ParsedCapturingSentence(CapturingSentence sentence, std::vector<std::string> insertedWords, std::vector<std::string> capturedWords) :
		ParsedSentenceBase{sentence.m_id, sentence.m_code}, m_insertedWords{insertedWords},
		m_capturedWords{capturedWords}, m_sentenceWordsBefore{sentence.m_wordsBefore},
		m_sentenceWordsAfter{sentence.m_wordsAfter} {}

	InvalidSentence::InvalidSentence(std::vector<std::string> insertedWords) :
		ParsedSentenceBase{idWhenInvalid, codeWhenInvalid}, m_insertedWords{insertedWords} {}

	
	std::unique_ptr<ParsedSentenceBase> makeParsed(Sentence sentence, std::vector<std::string> insertedWords) {
		std::unique_ptr<ParsedSentenceBase> ptr{};
		ptr.reset(new ParsedSentence{sentence, insertedWords});
		return ptr;
	}
	std::unique_ptr<ParsedSentenceBase> makeParsed(CapturingSentence sentence, std::vector<std::string> insertedWords, std::vector<std::string> capturedWords) {
		std::unique_ptr<ParsedSentenceBase> ptr{};
		ptr.reset(new ParsedCapturingSentence{sentence, insertedWords, capturedWords});
		return ptr;
	}
	std::unique_ptr<ParsedSentenceBase> makeParsed(std::vector<std::string> insertedWords) {
		std::unique_ptr<ParsedSentenceBase> ptr{};
		ptr.reset(new InvalidSentence{insertedWords});
		return ptr;
	}
}