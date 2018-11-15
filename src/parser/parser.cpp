#include "parser.h"

namespace parser {
	using std::string;
	using std::vector;
	using std::pair;
	using std::tuple;


	pair<const Sentence*, int> Parser::getHighestScoreSentence(const vector<string>& insertedWords) const {
		const Sentence* bestSentence = nullptr;
		int maxScoreSoFar = std::numeric_limits<int>::min();

		for (auto&& sentence : m_sentences) {
			int score = sentence.score(insertedWords);
			if (score > maxScoreSoFar) {
				maxScoreSoFar = score;
				bestSentence = &sentence;
			}
		}

		return {bestSentence, maxScoreSoFar};
	}
	tuple<const CapturingSentence*, vector<string>, int> Parser::getHighestScoreCapturingSentence(const vector<string>& insertedWords) const {
		const CapturingSentence* bestSentence = nullptr;
		vector<string> bestSentenceCapturedWords;
		int maxScoreSoFar = std::numeric_limits<int>::min();

		for (auto&& sentence : m_capturingSentences) {
			const auto& [score, capturedWords] = sentence.score(insertedWords);
			if (score > maxScoreSoFar) {
				maxScoreSoFar = score;
				bestSentence = &sentence;
				bestSentenceCapturedWords = capturedWords;
			}
		}

		return {bestSentence, bestSentenceCapturedWords, maxScoreSoFar};
	}

	Parser::Parser(const vector<Sentence>& sentences, const vector<CapturingSentence>& capturingSentences, const string& idWhenInvalid, const string& codeWhenInvalid) :
		m_sentences{sentences}, m_capturingSentences{capturingSentences},
		m_idWhenInvalid{idWhenInvalid}, m_codeWhenInvalid{codeWhenInvalid} {}

	std::unique_ptr<ParsedSentenceBase> Parser::parse(const vector<string>& insertedWords) const {
		auto [bestSentence, scoreSentence] = getHighestScoreSentence(insertedWords);
		auto [bestCapturingSentence, capturedWords, scoreCapturingSentence] = getHighestScoreCapturingSentence(insertedWords);

		if (scoreSentence < minimumRequiredScore && scoreCapturingSentence < minimumRequiredScore)
			return makeParsed(m_idWhenInvalid, m_codeWhenInvalid, insertedWords);

		if (bestSentence && bestCapturingSentence) {
			if (scoreSentence > scoreCapturingSentence)
				return makeParsed(*bestSentence, insertedWords);
			else
				return makeParsed(*bestCapturingSentence, insertedWords, capturedWords);
		}
		else if (bestSentence)
			return makeParsed(*bestSentence, insertedWords);
		else if (bestCapturingSentence)
			return makeParsed(*bestCapturingSentence, insertedWords, capturedWords);
		else
			return makeParsed(m_idWhenInvalid, m_codeWhenInvalid, insertedWords);
	}

	void Parser::add(const Sentence& sentence) {
		m_sentences.push_back(sentence);
	}
	void Parser::add(const CapturingSentence& capturingSentence) {
		m_capturingSentences.push_back(capturingSentence);
	}
}