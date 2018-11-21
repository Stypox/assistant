#include "section.h"

#include <algorithm>
#include <set>

namespace parser::constructs {
	using std::vector;
	using std::string;
	using std::pair;
	using std::set;
	using std::get;


	Section::Section(const Id& id, const vector<Sentence>& sentences, const Code& code) :
		m_id{id}, m_sentences{sentences},
		m_unfoldedSentences{unfolded(sentences)}, m_code{code} {}

	std::string Section::cppSentencesList(const std::string& sectionIdObjectName, const std::string& codeObjectName, bool pretty) const {
		std::string result;

		if (pretty) {
			for (auto&& sentence : m_unfoldedSentences) {
				result.append("\n\t\t{" + sectionIdObjectName + ", " + sentence.id.cppStringLiteral() + ", {");
				for (auto&& word : sentence.words)
					result.append("\"" + word + "\", ");
				*(result.end() - 2) = '}'; // replace last ,
				*(result.end() - 1) = ',';
				result.append(" " + codeObjectName + "},");
			}
		}
		else {
			for (auto&& sentence : m_unfoldedSentences) {
				result.append("{" + sectionIdObjectName + "," + sentence.id.cppStringLiteral() + ",{");
				for (auto&& word : sentence.words)
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
		for (auto&& sentence : section.m_unfoldedSentences) {
			stream << "\n <" << sentence.id << ">";
			for (auto&& word : sentence.words)
				stream << " " << word;
		}
		stream << "\nCODE:\n" << section.m_code;
		return stream;
	}


	CapturingSection::CapturingSection(const Id& id, const vector<CapturingSentence>& sentences, const Code& code) :
		m_id{id}, m_sentences{sentences},
		m_unfoldedSentences{unfolded(sentences)}, m_code{code} {}

	std::string CapturingSection::cppSentencesList(const std::string& sectionIdObjectName, const std::string& codeObjectName, bool pretty) const {
		std::string result;

		if (pretty) {
			for (auto&& sentence : m_unfoldedSentences) {
				result.append("\n\t\t{" + sectionIdObjectName + ", " + sentence.id.cppStringLiteral() + ", ");
				if (sentence.wordsBefore.empty()) {
					result.append("{},");
				}
				else {
					result.append("{");
					for (auto&& word : sentence.wordsBefore)
						result.append("\"" + word + "\", ");
					*(result.end() - 2) = '}'; // replace last ,
					*(result.end() - 1) = ',';
				}

				if (sentence.wordsAfter.empty()) {
					result.append(" {},");
				}
				else {
					result.append(" {");
					for (auto&& word : sentence.wordsAfter)
						result.append("\"" + word + "\", ");
					*(result.end() - 2) = '}'; // replace last ,
					*(result.end() - 1) = ',';
				}

				result.append(" " + codeObjectName + "},");
			}
		}
		else {
			for (auto&& sentence : m_unfoldedSentences) {
				result.append("{" + sectionIdObjectName + "," + sentence.id.cppStringLiteral() + ",");
				if (sentence.wordsBefore.empty()) {
					result.append("{}");
				}
				else {
					result.append("{");
					for (auto&& word : sentence.wordsBefore)
						result.append("\"" + word + "\",");
					result.back() = '}'; // replace last ,
				}

				if (sentence.wordsAfter.empty()) {
					result.append(",{}");
				}
				else {
					result.append(",{");
					for (auto&& word : sentence.wordsAfter)
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
		for (auto&& sentence : section.m_unfoldedSentences) {
			stream << "\n <" << sentence.id << ">";
			for (auto&& word : sentence.wordsBefore)
				stream << " " << word;
			stream << " ...";
			for (auto&& word : sentence.wordsAfter)
				stream << " " << word;
		}
		stream << "\nCODE:\n" << section.m_code;
		return stream;
	}
}