#ifndef _VOICE_ASSISTANT_PARSER_PARSED_SENTENCE_H_
#define _VOICE_ASSISTANT_PARSER_PARSED_SENTENCE_H_

#include <vector>
#include <string>
#include <memory>
#include <ostream>

#include "sentence.h"

namespace parser {
	class ParsedSentenceBase {
	protected:
		const std::string m_id, m_code;
	
	public:
		ParsedSentenceBase(const std::string& id, const std::string& code);

		virtual void log(std::ostream& stream) = 0;
	};

	class ParsedSentence : public ParsedSentenceBase {
		const std::vector<std::string>
			m_insertedWords,
			m_sentenceWords;
		
	public:
		ParsedSentence(const Sentence& sentence, const std::vector<std::string>& insertedWords);
		void log(std::ostream& stream);
	};

	class ParsedCapturingSentence : public ParsedSentenceBase {
		const std::vector<std::string>
			m_insertedWords,
			m_capturedWords,
			m_sentenceWordsBefore,
			m_sentenceWordsAfter;
		
	public:
		ParsedCapturingSentence(const CapturingSentence& sentence, const std::vector<std::string>& insertedWords, const std::vector<std::string>& capturedWords);
		void log(std::ostream& stream);
	};

	class InvalidSentence : public ParsedSentenceBase {
		const std::vector<std::string>
			m_insertedWords;
		
	public:
		InvalidSentence(const std::string& id, const std::string& code, const std::vector<std::string>& insertedWords);
		void log(std::ostream& stream);
	};

	std::unique_ptr<ParsedSentenceBase> makeParsed(const Sentence& sentence, const std::vector<std::string>& insertedWords);
	std::unique_ptr<ParsedSentenceBase> makeParsed(const CapturingSentence& sentence, const std::vector<std::string>& insertedWords, const std::vector<std::string>& capturedWords);
	std::unique_ptr<ParsedSentenceBase> makeParsed(const std::string& id, const std::string& code, const std::vector<std::string>& insertedWords);
}

#endif