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
	void ParsedSentence::log(std::ostream& stream) {
		stream << "* Normal sentence: (ID=" << m_id << ")\n* Inserted: ";
		for (auto&& word : m_insertedWords)
			stream << word << ",";
		stream << "\n* Sentence: ";
		for (auto&& word : m_sentenceWords)
			stream << word << ",";
		stream << "\n* Code:\n" << m_code << "\n\n";
	}

	ParsedCapturingSentence::ParsedCapturingSentence(const CapturingSentence& sentence, const vector<string>& insertedWords, const vector<string>& capturedWords) :
		ParsedSentenceBase{sentence.m_id, sentence.m_code}, m_insertedWords{insertedWords},
		m_capturedWords{capturedWords}, m_sentenceWordsBefore{sentence.m_wordsBefore},
		m_sentenceWordsAfter{sentence.m_wordsAfter} {}
	void ParsedCapturingSentence::log(std::ostream& stream) {
		stream << "* Capturing sentence: (ID=" << m_id << ")\n* Inserted: ";
		for (auto&& word : m_insertedWords)
			stream << word << ",";
		stream << "\n* Captured: ";
		for (auto&& word : m_capturedWords)
			stream << word << ",";
		stream << "\n* Sentence before: ";
		for (auto&& word : m_sentenceWordsBefore)
			stream << word << ",";
		stream << "\n* Sentence after: ";
		for (auto&& word : m_sentenceWordsAfter)
			stream << word << ",";
		stream << "\n* Code:\n" << m_code << "\n\n";
	}

	InvalidSentence::InvalidSentence(const string& id, const string& code, const vector<string>& insertedWords) :
		ParsedSentenceBase{id, code}, m_insertedWords{insertedWords} {}
	void InvalidSentence::log(std::ostream& stream) {
		stream << "* Invalid sentence: (ID=" << m_id << ")\n* Inserted: ";
		for (auto&& word : m_insertedWords)
			stream << word << ",";
		stream << "\n* Code:\n" << m_code << "\n\n";
	}

	
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
	unique_ptr<ParsedSentenceBase> makeParsed(const string& id, const string& code, const vector<string>& insertedWords) {
		unique_ptr<ParsedSentenceBase> ptr{};
		ptr.reset(new InvalidSentence{id, code, insertedWords});
		return ptr;
	}
}