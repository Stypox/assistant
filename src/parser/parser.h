#ifndef _VOICE_ASSISTANT_PARSER_PARSER_H_
#define _VOICE_ASSISTANT_PARSER_PARSER_H_

#include "sentence.h"

#include <tuple>

namespace parser {
	template <bool ... Conditions> struct true_all {};
	template <>					   struct true_all<>					 : std::true_type {};
	template <bool ... Conditions> struct true_all<false, Conditions...> : std::false_type {};
	template <bool ... Conditions> struct true_all<true, Conditions...>  : true_all<Conditions...>::type {};
	template <bool ... Conditions> constexpr bool true_all_v = true_all<Conditions...>::value;
	
	template<typename ... Container>
	class Parser {
		using rightTypesGuard = std::enable_if_t<true_all_v<is_sentence_v<typename Container::value_type>...>>;
		std::tuple<Container...> m_sentences;
	public:
		constexpr Parser(Container ... sentences) :
			m_sentences{sentences...} {}
	};

	template<typename T> struct has_value_type { 
		struct Fallback { int value_type; };
		struct Derived : T, Fallback { };

		template<typename C, C> struct ChT; 

		template<typename C> static char (&f(ChT<int Fallback::*, &C::value_type>*))[1]; 
		template<typename C> static char (&f(...))[2]; 

		static bool const value = sizeof(f<Derived>(0)) == 2;
	}; 
	template <typename T> constexpr bool has_value_type_v = has_value_type<T>::value;

	template <typename T> using to_array_t = std::conditional_t<has_value_type_v<T>, T, std::array<T, 1>>;

	template<typename ... Types>
	auto make_parser(Types... sentences) -> Parser<to_array_t<Types>...> {
		return {to_array_t<Types>{sentences}...};
	} 
}

#endif