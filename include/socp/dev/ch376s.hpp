#ifndef SOCP_CH376S_HPP
#define SOCP_CH376S_HPP

#include "dev.hpp"

namespace SOCP::DEV{
	namespace CH376S_CONST{
		static constexpr uint8_t CMD11_CHECK_EXIST=0x06;
		static constexpr uint8_t CMD11_CHECK_EXIST_VAL=0x65;
		static constexpr uint8_t CMD11_CHECK_EXIST_RET=0x9A;
		static constexpr uint8_t CMD11_SET_USB_MODE=0x15;
		enum struct USB_MODE:uint8_t{
			DEV_INVAILD,
			DEV_PERIPHERAL,
			DEV_INNER,
			SD_HOST,
			HOST_INVAILD,
			HOST,
			HOST_SOF,
			HOST_RESET,
		};
		static constexpr uint8_t CMD01_GET_STATUS=0x22;
		static constexpr uint8_t CMD01_RD_USB_DATA0=0x27;
		static constexpr uint8_t CMD10_SET_FILE_NAME=0x2F;
		static constexpr uint8_t CMD0H_DISK_MOUNT=0x31;
		static constexpr uint8_t CMD0H_FILE_OPEN=0x32;
		static constexpr uint8_t CMD2H_BYTE_READ=0x3A;
		static constexpr uint8_t CMD0H_BYTE_RD_GO=0x3B;

		static constexpr uint8_t CMD_RET_SUCCESS=0x51;
		static constexpr uint8_t USB_INT_SUCCESS=0x14;
		static constexpr uint8_t USB_INT_DISK_READ=0x1D;

		static constexpr uint8_t PARA_STATE_INTB=0x80;
	}
	struct CH376S{
		uint8_t addr=6;
		Code libs();
		Code set_data();
		Code set_cmd();

		SOCC::u8 data();
		SOCC::void_ data(uint8_t d);

		SOCC::u8 cmd();
		SOCC::void_ cmd(uint8_t code);
		SOCC::void_ cmd_s(uint8_t c,std::string str);
		SOCC::Fn<SOCC::u8()> wait_int;
		template<std::integral ...Args>
		SOCC::void_ cmd(uint8_t code,Args ...args){
			return SOCC::asm_({cmd(code),set_data(),write(args)...});
		}
		template<std::integral ...Args>
		auto cmd_r(uint8_t code,Args... args){
			return SOCC::u8{SOCC::expr({
				cmd(code,args...),
				data(),
			})};
		}
		template<std::integral ...Args>
		auto cmd_int(uint8_t code,Args... args){
			return SOCC::u8{SOCC::expr({
				cmd(code,args...),
				wait_int(),
			})};
		}
	};
}
#endif //SOCP_CH376S_HPP