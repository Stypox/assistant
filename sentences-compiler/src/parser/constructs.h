#ifndef _SENTENCES_COMPILER_PARSER_SECTION_H_
#define _SENTENCES_COMPILER_PARSER_SECTION_H_

#include <vector>
#include <string>
#include <ostream>

namespace parser {
	class Section;
	class CapturingSection;
	class Sentence;
	class CapturingSentence;
	class OrWord;
	using Code = std::string;

	class Section {
		std::vector<Sentence> m_sentences;
		Code m_code;
	public:
		Section(const std::vector<Sentence>& sentences, const Code& code);
		std::vector<std::vector<std::string>> unfoldedSentences() const;

		friend std::ostream& operator<< (std::ostream& stream, const Section& section);
	};

	class CapturingSection {
		std::vector<CapturingSentence> m_sentences;
		Code m_code;
	public:
		CapturingSection(const std::vector<CapturingSentence>& sentences, const Code& code);
		std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> unfoldedSentences() const;

		friend std::ostream& operator<< (std::ostream& stream, const CapturingSection& section);
	};

	class Sentence {
		std::vector<OrWord> m_orWords;
	public:
		Sentence(const std::vector<OrWord>& orWords);
		std::vector<std::vector<std::string>> unfolded() const;

		friend std::ostream& operator<< (std::ostream& stream, const Sentence& sentence);
	};
	
	class CapturingSentence {
		std::vector<OrWord> m_orWordsBefore;
		std::vector<OrWord> m_orWordsAfter;
	public:
		CapturingSentence(const std::vector<OrWord>& orWordsBefore, const std::vector<OrWord>& orWordsAfter);
		std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> unfolded() const;

		friend std::ostream& operator<< (std::ostream& stream, const CapturingSentence& sentence);
	};

	class OrWord {
		std::vector<std::string> m_words;
		bool m_required;
	public:
		OrWord(const std::vector<std::string>& words, bool required);

		friend std::vector<std::vector<std::string>> unfolded(std::vector<OrWord>::const_iterator thisWord, std::vector<OrWord>::const_iterator lastWord);
		friend std::ostream& operator<< (std::ostream& stream, const OrWord& orWord);
	};
}

#endif