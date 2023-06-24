#include <socp/dev/dev.hpp>
#include <socp/dev/lcd.hpp>
#include <soasm/soisv1.hpp>

using namespace SOCP::DEV;
using namespace SOASM::SOISv1;
using namespace SOASM::SOISv1::Regs;

Code LCD::set_data(){return set_addr(addr);}
Code LCD::set_cmd(){return set_addr(addr+1);}

Code LCD::cmd(uint8_t code){return {set_cmd(),write(code)};}
Code LCD::cmd(uint8_t code,uint8_t d1,uint8_t d2){
	return {
		set_data(),write(d1),write(d2),
		set_cmd(),write(code),
	};
}
		
Code LCD::data(uint8_t d){return {set_data(),write(d),};}

Code LCD::show(){
	return {
		ImmVal{}(-32),
		Calc{.fn=Calc::FN::ADD}(),
		set_data(),
		write(),
	};
}
void_ LCD::show(SOCC::u8 c){
	return asm_({
		c-32_u8,
		set_data(),
		write(),
	});
}
Code LCD::show(char c){return data(c-32);}
Code LCD::show(std::string str){
	Code code{set_data()};
	for(char c:str){
		code.add(write(c-32));
	}
	return code;
}
Code LCD::init(){
	return {
		cmd(0x40,0x00,0x00),// set text home address
		cmd(0x41,0x1E,0x00),// set text area
		cmd(0x80),// mode set - or mode
		cmd(0x94),// display mode - graphic off, text on
		cmd(0x24,0x00,0x00),// set address pointer
		cmd(0xB0),// auto write
	};
}