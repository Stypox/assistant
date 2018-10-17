#ifndef _SENTENCES_COMPILER_PARSER_PARSER_H_
#define _SENTENCES_COMPILER_PARSER_PARSER_H_

#include <optional>
#include <filesystem>

#include "../lexer/stream.h"
#include "constructs.h"

namespace parser {
	class Parser {
		lexer::Stream m_ts;
		std::vector<Section> m_sections;
		bool m_readNext = false;

		std::string readAllFile(std::filesystem::path path, unsigned int tokenLine);
		
		void sections();
		std::optional<Section> section();
		std::vector<Sentence> sentences();
		std::optional<Sentence> sentence();
		std::vector<OrWord> words();
		std::optional<OrWord> orWord();
		std::optional<ImportanceWord> importanceWord();
		Code code();

	public:
		void parse(std::istream& input);
	};
}

#endif