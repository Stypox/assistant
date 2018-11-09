#include "tokenizer.h"

#include <string>

namespace lexer {
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
				(ch >= 'A' && ch <= 'Z') ||
				(ch == '\''));
	}
	bool isOtherValid(char ch) {
		return ((ch >= '0' && ch <= '9') ||
				(ch == '_'));
	}
	bool isGrammar(char ch) {
		switch (ch) {
			case ':': case ';': case '|': case '?': case '.':
				return true;
			default:
				return false;
		}
	}

	Stream tokenize(std::istream& input) {
		Stream tokenStream;
		char ch = '\n'; // simulate newline
		unsigned int currentLine = 0,
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
			Token::Type type {0};

			while (1) {
				if (isLetter(ch))
					type = static_cast<Token::Type>(type | Token::letters);
				else if (isOtherValid(ch))
					type = static_cast<Token::Type>(type | Token::otherValid);
				else
					break;
				word.push_back(ch);
				ch = get();
			}

			unget();
			return std::make_pair(word, type);
		};
		auto readLine = [&]{
			std::string line;
			while ((ch = input.get()) != '\n' && input)
				line.push_back(ch);
			if (ch == '\n')
				input.unget();
			return line;
		};
		auto skipLine = [&]{
			do ch = get();
			while (ch != '\n' && input);
			unget();
		};

		while (input) {
			if (isLetter(ch) || isOtherValid(ch)) {
				auto [word, type] = readWord();
				tokenStream.push({type, word, currentLine, currentChar});
			}
			else if (isGrammar(ch)) {
				tokenStream.push({Token::grammar, {ch}, currentLine, currentChar});
			}
			else if (ch == '\n') {
				++currentLine;
				currentChar = 0;

				ch = get();
				if (ch == '\t')
					tokenStream.push({Token::code, readLine(), currentLine, currentChar});
				else {
					while (isSpace(ch))
						ch = get();
					if (ch == '-')
						tokenStream.push({Token::include, readLine(), currentLine, currentChar});
					else
						unget();
				}
			}
			else if (ch == '#')
				skipLine();
			else if (!isSpace(ch))
				throw std::runtime_error{"Invalid syntax: Unknown character '" + std::string(1, ch) + "' at line " + std::to_string(currentLine) + ", column " + std::to_string(currentChar)};
			
			ch = get();
		}

		return tokenStream;
	}
}