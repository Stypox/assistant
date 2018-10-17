#include "tokenizer.h"

#include <string>

namespace tokenizer {
	bool isSpace(char ch) {
		switch (ch) {
			case ' ': case '\f': case '\r': case '\t': case '\v':
				return true;
			default:
				return false;
		}
	}
	bool isLetter(char ch) {
		return ((ch >= 'a' && ch <= 'z') ||
				(ch >= 'A' && ch <= 'Z'));
	}
	bool isGrammar(char ch) {
		switch (ch) {
			case ';': case '|': case '?': case '.':
				return true;
			default:
				return false;
		}
	}

	Stream tokenize(std::istream& input) {
		Stream tokenStream;
		char ch;
		unsigned int currentLine = 1,
			currentChar = 0;

		auto get = [&]{
			++currentChar;
			return input.get();
		};
		auto unget = [&]{
			--currentChar;
			input.unget();
		};

		auto readWord = [&]{
			std::string word;
			word.push_back(ch);
			
			ch = get();
			while (isLetter(ch)) {
				word.push_back(ch);
				ch = get();
			}

			unget();
			return word;
		};
		auto readCode = [&]{
			std::string code;
			std::getline(input, code);
			return code + "\n";
		};
		auto skipLine = [&]{
			do ch = get();
			while (ch != '\n');
			unget();
		};

		while (input) {
			ch = get();

			if (isLetter(ch))
				tokenStream.push({Token::letters, readWord()});
			else if (isGrammar(ch))
				tokenStream.push({Token::grammar, std::string{1, ch}});
			else if (ch == '\n') {
				++currentLine;
				currentChar = 0;

				ch = get();
				if (ch == '\t')
					tokenStream.push({Token::code, readCode()});
				unget();
			}
			else if (ch == '#')
				skipLine();
			else if (!isSpace(ch))
				throw std::runtime_error{"Invalid syntax: Unknown character '" + std::string(1, ch) + "' at line " + std::to_string(currentLine) + ", column " + std::to_string(currentChar)};
		}

		return tokenStream;
	}
}