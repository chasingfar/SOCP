#include <socp/dev/kb64.hpp>

using namespace SOCP::DEV;

#define KC(name) std::to_underlying(KeyCode::name)

Code KB64::get_tbl() const{
	return {tbl,
		'`','1','2','3','4','5','6','7',
		'\t','Q','W','E','R','T','Y','U',
		KC(Caps),'A','S','D','F','G','H','J',
		KC(Shift),'Z','X','C','V','B','N','M',
		KC(Ctrl),KC(Win),KC(Alt),KC(Right),KC(Down),KC(Left),KC(Menu),' ',
		KC(Esc),KC(Back),'=','-','0','9','8',
		'I',KC(Del),'\\',']','[','P','O','L','K',
		KC(Enter),'\'',';',KC(Fn),KC(Up),'/','.',','
	};
}
u8 KB64::get_scan() const{
	return u8(expr({set_addr(addr),read()}));
}
u8 KB64::to_key(u8 scan_code) const{
	return *(ptr+u16::make(0_u8,scan_code));
}