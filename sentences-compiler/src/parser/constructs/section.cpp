#include "section.h"

#include <algorithm>
#include <set>

namespace parser::constructs {
	using std::vector;
	using std::string;
	using std::pair;
	using std::set;


	Section::Section(const Id& id, const vector<Sentence>& sentences, const Code& code) :
		m_id{id}, m_sentences{sentences},
		m_unfoldedSentences{}, m_code{code} {}
	vector<vector<string>> Section::unfoldedSentences() const {
		if (m_unfoldedSentences.empty()) {
			set<vector<string>> removedDuplicates;
			for (auto&& sentence : m_sentences) {
				auto unfolded = sentence.unfolded();
				removedDuplicates.insert(unfolded.begin(), unfolded.end());
			}

			m_unfoldedSentences.assign(removedDuplicates.begin(), removedDuplicates.end());
			m_unfoldedSentences.erase(std::remove(m_unfoldedSentences.begin(), m_unfoldedSentences.end(), vector<string>{}), m_unfoldedSentences.end());
		}
		return m_unfoldedSentences;
	}

	std::string Section::cppSentencesList(const std::string& idObjectName, const std::string& codeObjectName, bool pretty) const {
		std::string result;
		unfoldedSentences();

		if (pretty) {
			for (auto&& sentence : m_unfoldedSentences) {
				result.append("\n\t\t{" + idObjectName + ", {");
				for (auto&& word : sentence)
					result.append("\"" + word + "\", ");
				*(result.end() - 2) = '}'; // replace last ,
				*(result.end() - 1) = ',';
				result.append(" " + codeObjectName + "},");
			}
		}
		else {
			for (auto&& sentence : m_unfoldedSentences) {
				result.append("{" + idObjectName + ",{");
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
	std::string Section::cppIdStringLiteral() const {
		return m_id.cppStringLiteral();
	}

	std::ostream& operator<< (std::ostream& stream, const Section& section) {
		stream << "SECTION \"" << section.m_id << "\":";
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


	CapturingSection::CapturingSection(const Id& id, const vector<CapturingSentence>& sentences, const Code& code) :
		m_id{id}, m_sentences{sentences},
		m_unfoldedSentences{}, m_code{code} {}
	vector<pair<vector<string>, vector<string>>> CapturingSection::unfoldedSentences() const {
		if (m_unfoldedSentences.empty()) {
			set<pair<vector<string>, vector<string>>> removedDuplicates;
			for (auto&& sentence : m_sentences) {
				auto unfolded = sentence.unfolded();
				removedDuplicates.insert(unfolded.begin(), unfolded.end());
			}

			m_unfoldedSentences.assign(removedDuplicates.begin(), removedDuplicates.end());
			m_unfoldedSentences.erase(std::remove(m_unfoldedSentences.begin(), m_unfoldedSentences.end(), pair<vector<string>, vector<string>>{{}, {}}), m_unfoldedSentences.end());
		}
		return m_unfoldedSentences;
	}

	std::string CapturingSection::cppSentencesList(const std::string& idObjectName, const std::string& codeObjectName, bool pretty) const {
		std::string result;
		unfoldedSentences();

		if (pretty) {
			for (auto&& sentence : m_unfoldedSentences) {
				result.append("\n\t\t{" + idObjectName + ", ");
				if (sentence.first.empty()) {
					result.append("{},");
				}
				else {
					result.append("{");
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
				result.append("{" + idObjectName + ", ");
				if (sentence.first.empty()) {
					result.append("{}");
				}
				else {
					result.append("{");
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
	std::string CapturingSection::cppIdStringLiteral() const {
		return m_id.cppStringLiteral();
	}

	std::ostream& operator<< (std::ostream& stream, const CapturingSection& section) {
		stream << "SECTION (with capturing group) \"" << section.m_id << "\":";
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