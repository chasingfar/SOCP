#ifndef SOCP_DEV_HPP
#define SOCP_DEV_HPP

#include <soasm/asm.hpp>
#include <socc/library.hpp>

namespace SOCP::DEV{
	using namespace SOCC;
	Code set_addr(uint8_t addr);
	Code init();
	Code write(uint8_t v);
	Code write();
	Code read();
}
#endif //SOCP_DEV_HP