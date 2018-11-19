#include "code.h"

namespace parser::constructs {
	std::string Code::cppStringLiteral() const {
		static const std::string codeDelimiter {"_Code_Delimiter_"};
		if (empty())
			return "\"\"";

		std::string copy = *this;
		copy.insert(0, "R\"" + codeDelimiter + "(");
		copy.append(")" + codeDelimiter + "\"");
		return copy;
	}
}