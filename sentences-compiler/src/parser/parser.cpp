#include "parser.h"

#include <fstream>
#include <iostream>

#include "../lexer/tokenizer.h"
#include "../app/application.h"

using namespace lexer;
using namespace parser::constructs;
namespace fs = std::filesystem;
using std::vector;
using std::optional;
using std::variant;
using std::tuple;
using std::pair;


namespace parser {
	std::string readAllFile(fs::path path, std::string tokenPosition) {
		if (fs::is_directory(path))
			throw std::runtime_error{"Filesystem error:" + tokenPosition + ": file \"" + path.string() + "\" is a directory"};
		else if (!fs::exists(path))
			throw std::runtime_error{"Filesystem error:" + tokenPosition + ": file \"" + path.string() + "\" does not exist"};
		
		std::string code;
		size_t fileSize = fs::file_size(path);
		code.reserve(fileSize + 1);
		code.resize(fileSize);

		std::ifstream file{path, std::ios::binary};
		if (!file.is_open())
			throw std::runtime_error{"Filesystem error:" + tokenPosition + ": unable to open file \"" + path.string() + "\""};
		file.read(code.data(), fileSize);

		if (code.empty() || code.back() != '\n')
			code += '\n';
		
		return code;
	}

	void Parser::sections() {
		auto resSection = section();
		while (resSection.has_value()) {
			if (std::holds_alternative<Section>(*resSection)) // this is a normal section
				m_sections.push_back(std::get<Section>(*resSection));
			else // this section has capturing groups
				m_capturingSections.push_back(std::get<CapturingSection>(*resSection));
			resSection = section();
		}
		if (Token token = m_ts.get(false); !token.empty()) // there are still tokens left after all has been processed. This means an unexpected token was read.
			throw std::runtime_error("Grammar error:" + token.position() + ": expected sentence but got token \"" + token.str() + "\"");
	}
	optional<Id> Parser::id() {
		auto token = m_ts.get(m_readNext);

		if (token & Token::lettersOrOther) {
			Id resId{token.str()};
			token = m_ts.get(true);

			if (token == ':') {
				m_readNext = true;
				return resId;
			}
			else {
				throw std::runtime_error{"Grammar error:" + token.position() + ": excepted ':' but got " + token.str() + " after \"" + resId + "\" token"};
			}
		}
		else {
			return {};
		}
	}
	optional<variant<Section, CapturingSection>> Parser::section() {
		auto resId = id();
		if (resId.has_value()) {
			auto resSentences = sentences();
			if (!resSentences.has_value())
				throw std::runtime_error{"Grammar error:" + m_ts.get(false).position() + ": missing sentences in section " + std::to_string(m_sections.size() + m_capturingSections.size() + 1)};
			auto resCode = code();

			if (std::holds_alternative<vector<Sentence>>(*resSentences)) // this is a normal section
				return Section{*resId, std::get<vector<Sentence>>(*resSentences), resCode};
			else // this section has capturing groups
				return CapturingSection{*resId, std::get<vector<CapturingSentence>>(*resSentences), resCode};
		}
		else {
			// no sentence was read
			return {};
		}
	}
	optional<variant<vector<Sentence>, vector<CapturingSentence>>> Parser::sentences() {
		vector<Sentence> resSentences;
		vector<CapturingSentence> resCapturingSentences;

		auto resSentence = sentence();
		while (resSentence.has_value()) {
			// while sentences are being read
			if (std::holds_alternative<Sentence>(*resSentence)) // this sentence contains a capturing group
				resSentences.push_back(std::get<Sentence>(*resSentence));
			else // this sentence does not contain a capturing group
				resCapturingSentences.push_back(std::get<CapturingSentence>(*resSentence));
			resSentence = sentence();
		}

		if (resCapturingSentences.empty() && resSentences.empty()) // no sentence was read
			return {};
		if (resCapturingSentences.empty()) // only normal sencences were read. This is a normal section.
			return resSentences;
		else if (resSentences.empty()) // only capturing sencences were read. This section has capturing groups
			return resCapturingSentences;
		else // both normal and capturing sentences were read
			throw std::runtime_error{"Grammar error:" + m_ts.get(false).position() + ": sentences with and without capturing groups in the same section are not allowed"};
	}
	optional<variant<Sentence, CapturingSentence>> Parser::sentence() {
		auto [resOrWordsBefore, resOrWordsAfter, hasCapturingGroup] = words();
		if (resOrWordsBefore.empty() && resOrWordsAfter.empty()) // no sentence was read
			return {};

		if (Token token = m_ts.get(m_readNext); token == ';') {
			m_readNext = true;
			if (hasCapturingGroup) // this sentence contains a capturing group
				return CapturingSentence{resOrWordsBefore, resOrWordsAfter};
			else // this sentence does not contain a capturing group
				return Sentence{resOrWordsBefore};
		}
		else {
			throw std::runtime_error{"Grammar error:" + token.position() + ": excepted ';' but got \"" + token.str() + "\" at the end of sentence"};
		}
	}
	tuple<vector<OrWord>, vector<OrWord>, bool> Parser::words() {
		bool hasCapturingGroup = false;
		vector<OrWord> resOrWordsBefore, resOrWordsAfter;

		while (1) {
			auto [resWord, isCapturingGroup] = orWord();
			if (isCapturingGroup) {
				// a capturing group was read
				if (hasCapturingGroup)
					throw std::runtime_error{"Grammar error:" + m_ts.get(false).position() + ": only one capturing group per sentence is allowed"};

				hasCapturingGroup = true;
				continue;
			}
			else {
				if (resWord.has_value()) {
					// a word was read
					if (hasCapturingGroup)
						resOrWordsAfter.push_back(*resWord);
					else
						resOrWordsBefore.push_back(*resWord);
				}
				else // no word was read
					break;
			}
		}
		return {resOrWordsBefore, resOrWordsAfter, hasCapturingGroup};
	}
	pair<optional<OrWord>, bool> Parser::orWord() {
		vector<std::string> resWords;

		Token token = m_ts.get(m_readNext);
		if (token == '.') {
			// this word is a capturing group
			token = m_ts.get(true);
			if (token == '.') {
				m_readNext = true;
				return {{}, true};
			}
			else {
				throw std::runtime_error{"Grammar error:" + token.position() + ": capturing group is represented by two points \"..\" but there is one alone"};
			}
		}
		else if (token != Token::letters) {
			m_readNext = false;
			return {{}, false};
		}
		resWords.push_back(token.str());
		
		while (1) {
			token = m_ts.get(true);
			if (token == '|') {
				// this word can be one of the or-red list (e.g. wa|wb|wc) 
				token = m_ts.get(true);
				if (token == Token::letters)
					resWords.push_back(token.str());
				else
					throw std::runtime_error{"Grammar error:" + token.position() + ": excepted letters after '|' token"};
			}
			else if (token == '?') {
				// word is not required (e.g. word?)
				m_readNext = true;
				return {OrWord{resWords, false}, false};
			}
			else {
				m_readNext = false;
				return {OrWord{resWords, true}, false};
			}
		}
	}
	Code Parser::code() {
		Code lines;
		
		Token token = m_ts.get(m_readNext);
		while (1) {
			if (token == Token::code) {
				lines.append(token.str());
				lines += '\n';
			}
			else if (token == Token::include) {
				// read code from file
				lines.append(readAllFile(token.str(), token.position()));
			}
			else
				break;
			token = m_ts.get(true);
		}

		m_readNext = false;
		return lines;
	}

	void Parser::parse(std::istream& input) {
		m_ts = tokenize(input);

		auto resId = id();
		if (resId.has_value())
			m_idWhenInvalid = *resId;
		else
			throw std::runtime_error("Grammar error:" + m_ts.get(false).position() + ": expected id for invalid sentences");
		m_codeWhenInvalid = code();

		sections();
	}

	tuple<vector<Section>, vector<CapturingSection>, Id, Code> parse(const vector<std::istream*>& inputs) {
		Parser parser;
		for (auto&& input : inputs)
			parser.parse(*input);

		if (app::Application::args.getBool("verbose")) {
			for (auto&& section : parser.m_sections)
				std::cout << section << "\n";
			for (auto&& section : parser.m_capturingSections)
				std::cout << section << "\n";
		}

		return {parser.m_sections, parser.m_capturingSections, parser.m_idWhenInvalid, parser.m_codeWhenInvalid};
	}
}