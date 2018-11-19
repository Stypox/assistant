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
		m_unfoldedSentences{}, m_code{code} {}
	vector<UnfoldedSentence> Section::unfoldedSentences() const {
		if (m_unfoldedSentences.empty()) {
			auto comp = [](const UnfoldedSentence& a, const UnfoldedSentence& b) { return get<1>(a) < get<1>(b); };
			set<UnfoldedSentence, decltype(comp)> removedDuplicates{comp};
			for (auto&& sentence : m_sentences) {
				auto unfolded = sentence.unfolded();
				removedDuplicates.insert(unfolded.begin(), unfolded.end());
			}

			m_unfoldedSentences.assign(removedDuplicates.begin(), removedDuplicates.end());
			m_unfoldedSentences.erase(
				std::remove_if(m_unfoldedSentences.begin(), m_unfoldedSentences.end(), [](const UnfoldedSentence& a) { return get<1>(a).empty(); }),
				m_unfoldedSentences.end());
		}
		return m_unfoldedSentences;
	}

	std::string Section::cppSentencesList(const std::string& sectionIdObjectName, const std::string& codeObjectName, bool pretty) const {
		std::string result;
		unfoldedSentences();

		if (pretty) {
			for (auto&& sentence : m_unfoldedSentences) {
				result.append("\n\t\t{" + sectionIdObjectName + ", " + get<0>(sentence).cppStringLiteral() + ", {");
				for (auto&& word : get<1>(sentence))
					result.append("\"" + word + "\", ");
				*(result.end() - 2) = '}'; // replace last ,
				*(result.end() - 1) = ',';
				result.append(" " + codeObjectName + "},");
			}
		}
		else {
			for (auto&& sentence : m_unfoldedSentences) {
				result.append("{" + sectionIdObjectName + "," + get<0>(sentence).cppStringLiteral() + ",{");
				for (auto&& word : get<1>(sentence))
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
			stream << "\n <" << get<0>(sentence) << ">";
			for (auto&& word : get<1>(sentence))
				stream << " " << word;
		}
		stream << "\nCODE:\n" << section.m_code;
		return stream;
	}


	CapturingSection::CapturingSection(const Id& id, const vector<CapturingSentence>& sentences, const Code& code) :
		m_id{id}, m_sentences{sentences},
		m_unfoldedSentences{}, m_code{code} {}
	vector<UnfoldedCapturingSentence> CapturingSection::unfoldedSentences() const {
		if (m_unfoldedSentences.empty()) {
			auto comp = [](const UnfoldedCapturingSentence& a, const UnfoldedCapturingSentence& b) { return std::make_pair(get<1>(a), get<1>(b)) < std::make_pair(get<2>(a), get<2>(b)); };
			set<UnfoldedCapturingSentence, decltype(comp)> removedDuplicates{comp};
			for (auto&& sentence : m_sentences) {
				auto unfolded = sentence.unfolded();
				removedDuplicates.insert(unfolded.begin(), unfolded.end());
			}

			m_unfoldedSentences.assign(removedDuplicates.begin(), removedDuplicates.end());
			m_unfoldedSentences.erase(
				std::remove_if(m_unfoldedSentences.begin(), m_unfoldedSentences.end(), [](const UnfoldedCapturingSentence& a) { return get<0>(a).empty() && get<1>(a).empty(); }),
				m_unfoldedSentences.end());
		}
		return m_unfoldedSentences;
	}

	std::string CapturingSection::cppSentencesList(const std::string& sectionIdObjectName, const std::string& codeObjectName, bool pretty) const {
		std::string result;
		unfoldedSentences();

		if (pretty) {
			for (auto&& sentence : m_unfoldedSentences) {
				result.append("\n\t\t{" + sectionIdObjectName + ", " + get<0>(sentence).cppStringLiteral() + ", ");
				if (get<1>(sentence).empty()) {
					result.append("{},");
				}
				else {
					result.append("{");
					for (auto&& word : get<1>(sentence))
						result.append("\"" + word + "\", ");
					*(result.end() - 2) = '}'; // replace last ,
					*(result.end() - 1) = ',';
				}

				if (get<2>(sentence).empty()) {
					result.append(" {},");
				}
				else {
					result.append(" {");
					for (auto&& word : get<2>(sentence))
						result.append("\"" + word + "\", ");
					*(result.end() - 2) = '}'; // replace last ,
					*(result.end() - 1) = ',';
				}

				result.append(" " + codeObjectName + "},");
			}
		}
		else {
			for (auto&& sentence : m_unfoldedSentences) {
				result.append("{" + sectionIdObjectName + "," + get<0>(sentence).cppStringLiteral() + ",");
				if (get<1>(sentence).empty()) {
					result.append("{}");
				}
				else {
					result.append("{");
					for (auto&& word : get<1>(sentence))
						result.append("\"" + word + "\",");
					result.back() = '}'; // replace last ,
				}

				if (get<2>(sentence).empty()) {
					result.append(",{}");
				}
				else {
					result.append(",{");
					for (auto&& word : get<2>(sentence))
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
			for (auto&& word : get<1>(sentence))
				stream << " " << word;
			stream << " ...";
			for (auto&& word : get<2>(sentence))
				stream << " " << word;
		}
		stream << "\nCODE:\n" << section.m_code;
		return stream;
	}
}