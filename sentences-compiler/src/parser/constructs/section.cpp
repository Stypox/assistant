#include "section.h"

namespace parser::constructs {
	using std::vector;
	using std::string;
	using std::pair;


	Section::Section(const vector<Sentence>& sentences, const Code& code) :
		m_sentences{sentences}, m_unfoldedSentences{},
		m_code{code} {}
	vector<vector<string>> Section::unfoldedSentences() const {
		if (m_unfoldedSentences.empty()) {
			for (auto&& sentence : m_sentences) {
				auto unfolded = sentence.unfolded();
				m_unfoldedSentences.insert(m_unfoldedSentences.end(), unfolded.begin(), unfolded.end());
			}
		}
		return m_unfoldedSentences;
	}

	std::string Section::cppSentencesList(const std::string& codeObjectName, bool pretty) const {
		std::string result;
		unfoldedSentences();

		if (pretty) {
			for (auto&& sentence : m_unfoldedSentences) {
				result.append("\n\t\t{{");
				for (auto&& word : sentence)
					result.append("\"" + word + "\", ");
				*(result.end() - 2) = '}'; // replace last ,
				*(result.end() - 1) = ',';
				result.append(" " + codeObjectName + "},");
			}
		}
		else {
			for (auto&& sentence : m_unfoldedSentences) {
				result.append("{{");
				for (auto&& word : sentence)
					result.append("\"" + word + "\",");
				result.back() = '}'; // replace last ,
				result.append("," + codeObjectName + "},");
			}
		}

		if (!m_unfoldedSentences.empty())
			result.resize(result.size() - 1); // remove last ,
		return result;
	}
	std::string Section::cppCodeStringLiteral() const {
		return m_code.cppStringLiteral();
	}

	std::ostream& operator<< (std::ostream& stream, const Section& section) {
		stream << "SECTION:";
		for (auto&& sentence : section.m_sentences)
			stream << "\n" << sentence;
		stream << "\nUNFOLDED:";
		for (auto&& sentence : section.unfoldedSentences()) {
			stream << "\n -";
			for (auto&& word : sentence)
				stream << " " << word;
		}
		stream << "\nCODE:\n" << section.m_code;
		return stream;
	}


	CapturingSection::CapturingSection(const vector<CapturingSentence>& sentences, const Code& code) :
		m_sentences{sentences}, m_unfoldedSentences{},
		m_code{code} {}
	vector<pair<vector<string>, vector<string>>> CapturingSection::unfoldedSentences() const {
		if (m_unfoldedSentences.empty()) {
			for (auto&& sentence : m_sentences) {
				auto unfolded = sentence.unfolded();
				m_unfoldedSentences.insert(m_unfoldedSentences.end(), unfolded.begin(), unfolded.end());
			}
		}
		return m_unfoldedSentences;
	}

	std::string CapturingSection::cppSentencesList(const std::string& codeObjectName, bool pretty) const {
		std::string result;
		unfoldedSentences();

		if (pretty) {
			for (auto&& sentence : m_unfoldedSentences) {
				if (sentence.first.empty()) {
					result.append("\n\t\t{{}, ");
				}
				else {
					result.append("\n\t\t{{");
					for (auto&& word : sentence.first)
						result.append("\"" + word + "\", ");
					*(result.end() - 2) = '}'; // replace last ,
					*(result.end() - 1) = ',';
				}

				if (sentence.second.empty()) {
					result.append(" {},");
				}
				else {
					result.append(" {");
					for (auto&& word : sentence.second)
						result.append("\"" + word + "\", ");
					*(result.end() - 2) = '}'; // replace last ,
					*(result.end() - 1) = ',';
				}

				result.append(" " + codeObjectName + "},");
			}
		}
		else {
			for (auto&& sentence : m_unfoldedSentences) {
				if (sentence.first.empty()) {
					result.append("{{}");
				}
				else {
					result.append("{{");
					for (auto&& word : sentence.first)
						result.append("\"" + word + "\",");
					result.back() = '}'; // replace last ,
				}

				if (sentence.second.empty()) {
					result.append(",{}");
				}
				else {
					result.append(",{");
					for (auto&& word : sentence.second)
						result.append("\"" + word + "\",");
					result.back() = '}'; // replace last ,
				}

				result.append("," + codeObjectName + "},");
			}
		}

		if (!m_unfoldedSentences.empty())
			result.resize(result.size() - 1); // remove last ,
		return result;
	}
	std::string CapturingSection::cppCodeStringLiteral() const {
		return m_code.cppStringLiteral();
	}

	std::ostream& operator<< (std::ostream& stream, const CapturingSection& section) {
		stream << "SECTION (with capturing group):";
		for (auto&& sentence : section.m_sentences)
			stream << "\n" << sentence;
		stream << "\nUNFOLDED:";
		for (auto&& sentence : section.unfoldedSentences()) {
			stream << "\n *";
			for (auto&& word : sentence.first)
				stream << " " << word;
			stream << " ...";
			for (auto&& word : sentence.second)
				stream << " " << word;
		}
		stream << "\nCODE:\n" << section.m_code;
		return stream;
	}
}