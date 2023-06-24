#ifndef SOCP_LCD_HPP
#define SOCP_LCD_HPP

#include "dev.hpp"

namespace SOCP::DEV{
	struct LCD{
		uint8_t addr=0;

		Code set_data();
		Code set_cmd();

		Code cmd(uint8_t code);
		Code cmd(uint8_t code,uint8_t d1,uint8_t d2);
		
		Code data(uint8_t d);
		
		Code show();
		Code show(char c);
		void_ show(SOCC::u8);
		Code show(std::string str);
		Code init();
	};
}
#endif //SOCP_LCD_HPP