#include "code.h"

namespace parser::constructs {
	using std::string;

	void replaceAll(Code& oldCode, const string& before, const string& after) {		
		Code newCode;
		newCode.reserve(oldCode.length());

		string::size_type findPosLast = 0,
			findPos;
		while((findPos = oldCode.find(before, findPosLast)) != string::npos) {
			newCode.append(oldCode, findPosLast, findPos - findPosLast);
			newCode += after;
			findPosLast = findPos + before.length();
		}

		newCode += oldCode.substr(findPosLast);
		newCode.swap(oldCode);
	}

	Code Code::cppStringLiteral() {
		static const string codeDelimiter {"_Code_Delimiter_"};
		Code copy = *this;
		copy.insert(0, "R\"" + codeDelimiter + "(");
		copy.append(")" + codeDelimiter + "\"");
		return copy;
	}
}