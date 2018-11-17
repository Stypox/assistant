#ifndef _ASSISTANT_PARSER_PARSED_SENTENCE_H_
#define _ASSISTANT_PARSER_PARSED_SENTENCE_H_

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

		virtual void json(std::ostream& output) = 0;
		virtual void log(std::ostream& output) = 0;
	};

	class ParsedSentence : public ParsedSentenceBase {
		const std::vector<std::string>
			m_insertedWords,
			m_sentenceWords;
		
	public:
		ParsedSentence(const Sentence& sentence, const std::vector<std::string>& insertedWords);

		void json(std::ostream& output) override;
		void log(std::ostream& output) override;
	};

	class ParsedCapturingSentence : public ParsedSentenceBase {
		const std::vector<std::string>
			m_insertedWords,
			m_capturedWords,
			m_sentenceWordsBefore,
			m_sentenceWordsAfter;
		
	public:
		ParsedCapturingSentence(const CapturingSentence& sentence, const std::vector<std::string>& insertedWords, const std::vector<std::string>& capturedWords);

		void json(std::ostream& output) override;
		void log(std::ostream& output) override;
	};

	class InvalidSentence : public ParsedSentenceBase {
		const std::vector<std::string>
			m_insertedWords;
		
	public:
		InvalidSentence(const std::string& id, const std::string& code, const std::vector<std::string>& insertedWords);

		void json(std::ostream& output) override;
		void log(std::ostream& output) override;
	};

	std::unique_ptr<ParsedSentenceBase> makeParsed(const Sentence& sentence, const std::vector<std::string>& insertedWords);
	std::unique_ptr<ParsedSentenceBase> makeParsed(const CapturingSentence& sentence, const std::vector<std::string>& insertedWords, const std::vector<std::string>& capturedWords);
	std::unique_ptr<ParsedSentenceBase> makeParsed(const std::string& id, const std::string& code, const std::vector<std::string>& insertedWords);
}

#endif