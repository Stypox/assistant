#include "parser.h"

#include <fstream>
#include <iostream>

using namespace tokenizer;

namespace parser {
	std::string Parser::readAllFile(std::filesystem::path path, unsigned int tokenLine) {
		if (std::filesystem::is_directory(path))
			throw std::runtime_error{"Filesystem error: file \"" + path.string() + "\" specified on line " + std::to_string(tokenLine) + " is a directory"};
		else if (!std::filesystem::exists(path))
			throw std::runtime_error{"Filesystem error: file \"" + path.string() + "\" specified on line " + std::to_string(tokenLine) + " does not exist"};
		
		std::string code;
		size_t fileSize = std::filesystem::file_size(path);
		code.resize(fileSize + 1);

		std::ifstream file{path, std::ios::binary};
		if (!file.is_open())
			throw std::runtime_error{"Filesystem error: unable to open file \"" + path.string() + "\" specified on line " + std::to_string(tokenLine)};
		file.read(code.data(), fileSize);

		if (code.empty() || code.back() != '\n')
			code.push_back('\n');
		
		return code;
	}

	void Parser::sections() {
		auto resSection = section();
		while (resSection.has_value()) {
			m_sections.push_back(*resSection);
			resSection = section();
		}
		if (Token token = m_ts.get(false); token)
			throw std::runtime_error("Grammar error: expected sentence but got token \"" + token.value + "\" on line " + std::to_string(token.line));
	}
	std::optional<Section> Parser::section() {
		auto resSentences = sentences();
		if (resSentences.empty())
			return {};
		else 
			return std::optional<Section>{std::in_place, resSentences, code()};
	}
	std::vector<Sentence> Parser::sentences() {
		std::vector<Sentence> resSentences;
		auto resSentence = sentence();
		while (resSentence.has_value()) {
			resSentences.push_back(*resSentence);
			resSentence = sentence();
		}
		return resSentences;
	}
	std::optional<Sentence> Parser::sentence() {
		auto resWords = words();
		if (resWords.empty())
			return {};

		if (Token token = m_ts.get(m_readNext); token.type == Token::grammar && token.ch() == ';') {
			m_readNext = true;
			return std::optional<Sentence>{std::in_place, resWords};
		}
		else {
			throw std::runtime_error{"Grammar error: excepted ';' but got \"" + token.value + "\" at the end of sentence on line " + std::to_string(token.line)};
		}
	}
	std::vector<OrWord> Parser::words() {
		std::vector<OrWord> resWords;
		auto resWord = orWord();
		while (resWord.has_value()) {
			resWords.push_back(*resWord);
			resWord = orWord();
		}
		return resWords;
	}
	std::optional<OrWord> Parser::orWord() {
		std::vector<ImportanceWord> resImportanceWords;
		auto resImportanceWord = importanceWord();
		if (!resImportanceWord.has_value())
			return {};
		resImportanceWords.push_back(*resImportanceWord);
		
		while (1) {
			Token token = m_ts.get(m_readNext);
			if (token.type == Token::grammar && token.ch() == '|') {
				m_readNext = true;
				resImportanceWord = importanceWord();
				if (resImportanceWord.has_value())
					resImportanceWords.push_back(*resImportanceWord);
				else
					throw std::runtime_error{"Grammar error: excepted importanceWord after '|' token on line " + std::to_string(token.line)};
			}
			else {
				m_readNext = false;
				break;
			}
		}
		return std::optional<OrWord>{std::in_place, resImportanceWords};
	}
	std::optional<ImportanceWord> Parser::importanceWord() {
		Token token = m_ts.get(m_readNext);
		if (token.type == Token::letters) {
			std::string resLetters = token.value;
			token = m_ts.get(true);
			if (token.type == Token::grammar && token.ch() == '?') {
				m_readNext = true;
				return std::optional<ImportanceWord>{std::in_place, resLetters, false};
			}
			else {
				m_readNext = false;
				return std::optional<ImportanceWord>{std::in_place, resLetters, true};
			}
		}
		else {
			m_readNext = false;
			return {};
		}
	}
	Code Parser::code() {
		std::string lines;
		bool noCode = true;
		
		Token token = m_ts.get(m_readNext);
		while (1) {
			if (token.type == Token::code) {
				lines += token.value + "\n";
				noCode = false;
			}
			else if (token.type == Token::include) {
				lines += readAllFile(token.value, token.line);
				noCode = false;
			}
			else
				break;
			token = m_ts.get(true);
		}

		if (noCode)
			throw std::runtime_error{"Grammar error: missing code after section " + std::to_string(m_sections.size() + 1)};
		m_readNext = false;
		return Code{lines};
	}

	void Parser::parse(std::istream& input) {
		m_ts = tokenize(input);

		sections();

		for (auto&& section : m_sections)
			std::cout << section << "\n";
	}
}