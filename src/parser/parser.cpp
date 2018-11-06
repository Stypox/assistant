#include "parser.h"

#include "../exec/executer.h"

namespace parser {
	using std::string;
	using std::vector;
	using std::pair;
	using std::tuple;


	pair<const Sentence*, int> Parser::getHighestScoreSentence(const vector<string>& words) const {
		const Sentence* bestSentence = nullptr;
		int maxScoreSoFar = std::numeric_limits<int>::min();

		for (auto&& sentence : m_sentences) {
			int score = sentence.score(words);
			if (score > maxScoreSoFar) {
				maxScoreSoFar = score;
				bestSentence = &sentence;
			}
		}

		return {bestSentence, maxScoreSoFar};
	}
	tuple<const CapturingSentence*, vector<string>, int> Parser::getHighestScoreCapturingSentence(const vector<string>& words) const {
		const CapturingSentence* bestSentence = nullptr;
		vector<string> bestSentenceCapturedWords;
		int maxScoreSoFar = std::numeric_limits<int>::min();

		for (auto&& sentence : m_capturingSentences) {
			const auto& [score, capturedWords] = sentence.score(words);
			if (score > maxScoreSoFar) {
				maxScoreSoFar = score;
				bestSentence = &sentence;
				bestSentenceCapturedWords = capturedWords;
			}
		}

		return {bestSentence, bestSentenceCapturedWords, maxScoreSoFar};
	}

	Parser::Parser(const vector<Sentence>& sentences, const vector<CapturingSentence>& capturingSentences, const string& codeWhenNotUnderstood) :
		m_sentences{sentences}, m_capturingSentences{capturingSentences}, m_codeWhenNotUnderstood{codeWhenNotUnderstood} {}

	void Parser::parse(const vector<string>& words) const {
		auto [bestSentence, scoreSentence] = getHighestScoreSentence(words);
		auto [bestCapturingSentence, capturedWords, scoreCapturingSentence] = getHighestScoreCapturingSentence(words);

		if (scoreSentence < minimumRequiredScore && scoreCapturingSentence < minimumRequiredScore) {
			exec::execute(
				exec::buildArray(receivedWordsPyName, words) +
				m_codeWhenNotUnderstood);
			return;
		}

		if (bestSentence && bestCapturingSentence) {
			if (scoreSentence > scoreCapturingSentence)
				bestSentence->exec(words);
			else
				bestCapturingSentence->exec(words, capturedWords);
		}
		else if (bestSentence)
			bestSentence->exec(words);
		else if (bestCapturingSentence)
			bestCapturingSentence->exec(words, capturedWords);
		else
			exec::execute(
				exec::buildArray(receivedWordsPyName, words) +
				m_codeWhenNotUnderstood);
	}

	void Parser::add(const Sentence& sentence) {
		m_sentences.push_back(sentence);
	}
	void Parser::add(const CapturingSentence& capturingSentence) {
		m_capturingSentences.push_back(capturingSentence);
	}
}