#include "parsed_sentence.h"

namespace parser {
	using std::vector;
	using std::string;
	using std::unique_ptr;
	

	ParsedSentenceBase::ParsedSentenceBase(const string& id, const string& code) :
		m_id{id}, m_code{code} {}

	ParsedSentence::ParsedSentence(const Sentence& sentence, const vector<string>& insertedWords) :
		ParsedSentenceBase{sentence.m_id, sentence.m_code}, m_insertedWords{insertedWords},
		m_sentenceWords{sentence.m_words} {}

	ParsedCapturingSentence::ParsedCapturingSentence(const CapturingSentence& sentence, const vector<string>& insertedWords, const vector<string>& capturedWords) :
		ParsedSentenceBase{sentence.m_id, sentence.m_code}, m_insertedWords{insertedWords},
		m_capturedWords{capturedWords}, m_sentenceWordsBefore{sentence.m_wordsBefore},
		m_sentenceWordsAfter{sentence.m_wordsAfter} {}

	InvalidSentence::InvalidSentence(const vector<string>& insertedWords) :
		ParsedSentenceBase{idWhenInvalid, codeWhenInvalid}, m_insertedWords{insertedWords} {}

	
	unique_ptr<ParsedSentenceBase> makeParsed(const Sentence& sentence, const vector<string>& insertedWords) {
		unique_ptr<ParsedSentenceBase> ptr{};
		ptr.reset(new ParsedSentence{sentence, insertedWords});
		return ptr;
	}
	unique_ptr<ParsedSentenceBase> makeParsed(const CapturingSentence& sentence, const vector<string>& insertedWords, const vector<string>& capturedWords) {
		unique_ptr<ParsedSentenceBase> ptr{};
		ptr.reset(new ParsedCapturingSentence{sentence, insertedWords, capturedWords});
		return ptr;
	}
	unique_ptr<ParsedSentenceBase> makeParsed(const vector<string>& insertedWords) {
		unique_ptr<ParsedSentenceBase> ptr{};
		ptr.reset(new InvalidSentence{insertedWords});
		return ptr;
	}
}