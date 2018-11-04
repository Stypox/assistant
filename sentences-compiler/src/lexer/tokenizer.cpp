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
			word.push_back(ch);
			
			ch = get();
			while (isLetter(ch)) {
				word.push_back(ch);
				ch = get();
			}

			unget();
			return word;
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
			while (ch != '\n');
			unget();
		};

		while (input) {
			if (isLetter(ch))
				tokenStream.push({Token::letters, readWord(), currentLine, currentChar});
			else if (isGrammar(ch))
				tokenStream.push({Token::grammar, {ch}, currentLine, currentChar});
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