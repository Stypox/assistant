#ifndef _SENTENCES_COMPILER_PARSER_PARSER_H_
#define _SENTENCES_COMPILER_PARSER_PARSER_H_

#include <optional>
#include <variant>
#include <filesystem>

#include "../lexer/stream.h"
#include "constructs.h"

namespace parser {
	class Parser {
		lexer::Stream m_ts;
		Code m_codeWhenNotUnderstood;
		std::vector<Section> m_sections;
		std::vector<CapturingSection> m_capturingSections;
		bool m_readNext = false;
		
		void																				sections();
		std::optional<std::variant<Section, CapturingSection>>								section();
		std::optional<std::variant<std::vector<Sentence>, std::vector<CapturingSentence>>>	sentences();
		std::optional<std::variant<Sentence, CapturingSentence>>							sentence();
		std::tuple<std::vector<OrWord>, std::vector<OrWord>, bool>							words();
		std::pair<std::optional<OrWord>, bool>												orWord();
		std::optional<Code>																	code();

	public:
		void parse(std::istream& input);
	};
}

#endif