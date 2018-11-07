#include "id.h"

namespace parser::constructs {
	std::string Id::cppStringLiteral() const {
		static const std::string idDelimiter {"__Id_Delimiter__"};
		std::string copy = *this;
		copy.insert(0, "R\"" + idDelimiter + "(");
		copy.append(")" + idDelimiter + "\"");
		return copy;
	}
}