#ifndef SOCP_PROGRAM_HPP
#define SOCP_PROGRAM_HPP

#include <soasm/asm.hpp>

namespace SOCP{
	//from https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
	template<size_t N>
	struct StringLiteral {
		constexpr StringLiteral(const char (&str)[N]) {
			std::copy_n(str, N, value);
		}
		char value[N];
	};

	template<StringLiteral lit>
	SOASM::Code program();
}
#endif //SOCP_PROGRAM_HPP