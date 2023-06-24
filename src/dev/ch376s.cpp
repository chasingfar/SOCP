#include <socp/dev/ch376s.hpp>

using namespace SOCP::DEV;

Code CH376S::set_data(){return set_addr(addr);}
Code CH376S::set_cmd(){return set_addr(addr+1);}

void_ CH376S::cmd(uint8_t code){return asm_({set_cmd(),write(code)});}
void_ CH376S::data(uint8_t d){return asm_({set_data(),write(d)});}
u8 CH376S::cmd(){return u8(expr({set_cmd(),read()}));}
u8 CH376S::data(){return u8(expr({set_data(),read()}));}
void_ CH376S::cmd_s(uint8_t c,std::string str){
	Code code{cmd(c),set_data()};
	for(char s:str){
		code.add(write(s));
	}
	code.add(write(0));
	return asm_(code);
}

Code CH376S::libs(){
	using namespace CH376S_CONST;
	return {
		wait_int.impl([&](auto& _)->Stmt{return {
			while_(cmd()&u8(PARA_STATE_INTB)).do_({
			}).end(),
			_.return_(cmd_r(CMD01_GET_STATUS)),
		};}),
	};
};