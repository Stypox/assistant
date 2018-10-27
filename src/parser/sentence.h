#ifndef _VOICE_ASSISTANT_PARSER_SENTENCE_H_
#define _VOICE_ASSISTANT_PARSER_SENTENCE_H_

#include <vector>
#include <array>
#include <string>

namespace parser {
	template<typename Container, typename = typename std::enable_if_t<
		std::is_same_v<typename Container::value_type, std::string> ||
		std::is_same_v<typename Container::value_type, std::string_view>>>
	class SentenceBase {
	public:
		using string_type = std::conditional_t<std::is_literal_type_v<Container>, std::string_view, std::string>;
	private:
		const string_type m_code;
		const Container m_words;
	public:
		constexpr SentenceBase(const string_type& code, const Container& words) :
			m_code{code}, m_words{words} {}

		int score(const std::vector<std::string>& words) const {
			
		}
		void exec() const {

		}
	};

	template<size_t N>
	using ConstSentence = SentenceBase<std::array<std::string_view, N>>;
	using Sentence = SentenceBase<std::vector<std::string>>;

	template <typename N>
	struct is_sentence : std::false_type {};
	template <>
	struct is_sentence<Sentence> : std::true_type {};
	template <size_t N>
	struct is_sentence<ConstSentence<N>> : std::true_type {};
	template <typename T>
	inline constexpr bool is_sentence_v = is_sentence<T>::value;
}

#endif