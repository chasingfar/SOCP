#ifndef SOCP_KB64_HPP
#define SOCP_KB64_HPP

#include "dev.hpp"

namespace SOCP::DEV{
	struct KB64{
		uint8_t addr=4;
		Label tbl;
		ptr<u8> ptr{tbl};
		enum struct KeyCode:char{
			Null=0,Caps,Shift,Ctrl,Win,Alt,Right,Down,Left,Menu,Esc,Back,Del,Enter,Fn,Up,KB64_END
		};
		Code get_tbl() const;
		u8 get_scan() const;
		u8 to_key(u8 scan_code) const;
	};
}
#endif //SOCP_KB64_HPP