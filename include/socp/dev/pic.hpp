#ifndef SOCP_PIC_HPP
#define SOCP_PIC_HPP

#include "dev.hpp"

namespace SOCP::DEV{
	struct PIC{
		uint8_t addr=2;
		Code set_a0(uint8_t a0){return set_addr(addr+(a0&1u));}
		Code icw1(uint8_t AL,bool is_level_trig,bool is_4_interval,bool is_single,bool need_icw4){
			uint8_t data=0b00010000;
			data|=AL<<5;
			if(is_level_trig){data|=0b00001000;}
			if(is_4_interval){data|=0b00000100;}
			if(is_single    ){data|=0b00000010;}
			if(need_icw4    ){data|=0b00000001;}

			return {set_a0(0),write(data)};
		}
		Code icw2(uint8_t AH){
			return {set_a0(1),write(AH)};
		}
		Code icw4(bool SFNM,bool is_buf,bool is_master,bool AEOI,bool is_86){
			uint8_t data=0;
			if(SFNM     ){data|=0b00010000;}
			if(is_buf   ){data|=0b00001000;}
			if(is_master){data|=0b00000100;}
			if(AEOI     ){data|=0b00000010;}
			if(is_86    ){data|=0b00000001;}

			return {set_a0(1),write(data)};
		}
		Code init80(uint16_t addr){
			return {
				icw1((addr>>5)&0b111u,false,true,true,true),
				//icw2(0b10101000),
				icw2((addr>>8)&0xffu),
				icw4(false,false,false,true,false),
			};
		}
		Code init86(uint8_t addr){
			return {
				icw1(0,false,true,true,true),
				icw2((addr&0x1fu)<<3),
				icw4(false,false,false,false,true),
			};
		}
		Code eoi(uint8_t id){
			return {set_a0(0),write(0b00100000|(id&0b111))};
		}
	};
}
#endif //SOCP_PIC_HPP