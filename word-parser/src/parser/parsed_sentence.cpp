#include "parsed_sentence.h"

namespace parser {
	using std::vector;
	using std::string;
	using std::unique_ptr;


	void escapeJson(const string& str, std::ostream& output) {
		for (auto&& ch : str) {
			switch (ch) {
			case '"':
				output << "\\\"";
				break;
			case '\\':
				output << "\\\\";
				break;
			case '\b':
				output << "\\b";
				break;
			case '\f':
				output << "\\f";
				break;
			case '\n':
				output << "\\n";
				break;
			case '\r':
				output << "\\r";
				break;
			case '\t':
				output << "\\t";
				break;
			default:
				if (ch >= 0x00 && ch <= 0x1f) {
					string chAsInt = std::to_string(ch);
					output << "\\u" << string(4 - chAsInt.size(), '0') << chAsInt;
				}
				else {
					output << ch;
				}
			}
		}
	}
	void wordsToJson(const vector<string>& words, std::ostream& output) {
		output << "[";
		if (!words.empty()) {
			output << "\"";
			escapeJson(words[0], output);
			output << "\"";
			for (auto word = words.begin() + 1; word != words.end(); ++word) {
				output << ",\"";
				escapeJson(*word, output);
				output << "\"";
			}
		}
		output << "]";
	}
	

	ParsedSentenceBase::ParsedSentenceBase(const string& sectionId, const std::vector<std::string>& insertedWords, const string& code) :
		m_sectionId{sectionId}, m_code{code},
		m_insertedWords{insertedWords} {}

	ParsedSentence::ParsedSentence(const Sentence& sentence, const vector<string>& insertedWords) :
		ParsedSentenceBase{sentence.m_sectionId, insertedWords, sentence.m_code}, m_sentenceId{sentence.m_sentenceId},
		m_sentenceWords{sentence.m_words} {}
	
	void ParsedSentence::json(std::ostream& output) {
		output << "{\"type\":\"normal\",\"section_id\":\"" << m_sectionId <<
			"\",\"sentence_id\":\"" << m_sentenceId << "\",\"inserted_words\":";
		wordsToJson(m_insertedWords, output);
		output << ",\"sentence_words\":";
		wordsToJson(m_sentenceWords, output);
		output << ",\"code\":\"";
		escapeJson(m_code, output);
		output << "\"}\n";
	}
	void ParsedSentence::log(std::ostream& output) {
		output << "* Normal sentence: (ID=" << m_sectionId << ":" << m_sentenceId << ")\n* Inserted: ";
		for (auto&& word : m_insertedWords)
			output << word << ",";
		output << "\n* Sentence: ";
		for (auto&& word : m_sentenceWords)
			output << word << ",";
		output << "\n* Code:\n" << m_code << "\n\n";
	}

	ParsedCapturingSentence::ParsedCapturingSentence(const CapturingSentence& sentence, const vector<string>& insertedWords, const vector<string>& capturedWords) :
		ParsedSentenceBase{sentence.m_sectionId, insertedWords, sentence.m_code}, m_sentenceId{sentence.m_sentenceId},
		m_capturedWords{capturedWords}, m_sentenceWordsBefore{sentence.m_wordsBefore},
		m_sentenceWordsAfter{sentence.m_wordsAfter} {}

	void ParsedCapturingSentence::json(std::ostream& output) {
		output << "{\"type\":\"capturing\",\"section_id\":\"" << m_sectionId <<
			"\",\"sentence_id\":\"" << m_sentenceId << "\",\"inserted_words\":";
		wordsToJson(m_insertedWords, output);
		output << ",\"captured_words\":";
		wordsToJson(m_capturedWords, output);
		output << ",\"sentence_words_before\":";
		wordsToJson(m_sentenceWordsBefore, output);
		output << ",\"sentence_words_after\":";
		wordsToJson(m_sentenceWordsAfter, output);
		output << ",\"code\":\"";
		escapeJson(m_code, output);
		output << "\"}\n";
	}
	void ParsedCapturingSentence::log(std::ostream& output) {
		output << "* Capturing sentence: (ID=" << m_sectionId << ":" << m_sentenceId << ")\n* Inserted: ";
		for (auto&& word : m_insertedWords)
			output << word << ",";
		output << "\n* Captured: ";
		for (auto&& word : m_capturedWords)
			output << word << ",";
		output << "\n* Sentence before: ";
		for (auto&& word : m_sentenceWordsBefore)
			output << word << ",";
		output << "\n* Sentence after: ";
		for (auto&& word : m_sentenceWordsAfter)
			output << word << ",";
		output << "\n* Code:\n" << m_code << "\n\n";
	}

	InvalidSentence::InvalidSentence(const string& sectionId, const string& code, const vector<string>& insertedWords) :
		ParsedSentenceBase{sectionId, insertedWords, code} {}

	void InvalidSentence::json(std::ostream& output) {
		output << "{\"type\":\"invalid\",\"section_id\":\"" << m_sectionId <<
			"\",\"inserted_words\":";
		wordsToJson(m_insertedWords, output);
		output << ",\"code\":\"";
		escapeJson(m_code, output);
		output << "\"}\n";
	}
	void InvalidSentence::log(std::ostream& output) {
		output << "* Invalid sentence: (ID=" << m_sectionId << ")\n* Inserted: ";
		for (auto&& word : m_insertedWords)
			output << word << ",";
		output << "\n* Code:\n" << m_code << "\n\n";
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
	unique_ptr<ParsedSentenceBase> makeParsed(const string& sectionId, const string& code, const vector<string>& insertedWords) {
		unique_ptr<ParsedSentenceBase> ptr{};
		ptr.reset(new InvalidSentence{sectionId, code, insertedWords});
		return ptr;
	}
}