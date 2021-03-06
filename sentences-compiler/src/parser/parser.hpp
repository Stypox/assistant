#ifndef _SENTENCES_COMPILER_PARSER_PARSER_HPP_
#define _SENTENCES_COMPILER_PARSER_PARSER_HPP_

#include <optional>
#include <variant>
#include <filesystem>

#include "../lexer/stream.hpp"
#include "constructs/section.hpp"

namespace parser {
	class Parser {
		using Section = constructs::Section;
		using CapturingSection = constructs::CapturingSection;
		using Sentence = constructs::Sentence;
		using CapturingSentence = constructs::CapturingSentence;
		using OrWord = constructs::OrWord;
		using Id = constructs::Id;
		using Code = constructs::Code;

		lexer::Stream m_ts;
		bool m_readNext = false;

		std::vector<Section> m_sections;
		std::vector<CapturingSection> m_capturingSections;
		Id m_sectionIdWhenInvalid;
		Code m_codeWhenInvalid;

		
		void																				sections();
		std::optional<std::variant<Section, CapturingSection>>								section();
		std::optional<Id>																	sectionId();
		std::optional<std::variant<std::vector<Sentence>, std::vector<CapturingSentence>>>	sentences();
		std::optional<std::variant<Sentence, CapturingSentence>>							sentence();
		std::optional<Id>																	sentenceId();
		std::tuple<std::vector<OrWord>, std::vector<OrWord>, bool>							words();
		std::pair<std::optional<OrWord>, bool>												orWord();
		Code																				code();

		void parse(std::istream& input);

		friend std::tuple<std::vector<Section>, std::vector<CapturingSection>, Id, Code> parse(const std::vector<std::istream*>& inputs);
	};
	std::tuple<std::vector<constructs::Section>, std::vector<constructs::CapturingSection>, constructs::Id, constructs::Code> parse(const std::vector<std::istream*>& inputs);
}

#endif