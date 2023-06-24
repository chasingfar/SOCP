
#include <socp/program.hpp>
#include <soasm/soisv1.hpp>
#include <socp/dev/lcd.hpp>
#include <socp/dev/pic.hpp>
#include <socp/dev/kb64.hpp>

using namespace SOCP::DEV;
using namespace SOASM::SOISv1;
using u8_=SOCC::u8;

template<> Code SOCP::program<"kb64">(){
	SOASM::Label::tbl_t L;
	LCD lcd;
	PIC pic;
	KB64 kb;
	u8_ Reg_A{RegVar<Reg>::make(Reg::A)};
	u8_ Reg_B{RegVar<Reg>::make(Reg::B)};
	return {
		Jump{}(L["main"]),0xff,
		Jump{}(L["isr_kb"]),0xff,
		Jump{}(L["isr2"]),0xff,
		Jump{}(L["isr3"]),0xff,
		Jump{}(L["isr4"]),0xff,
		Jump{}(L["isr5"]),0xff,
		Jump{}(L["isr6"]),0xff,
		Jump{}(L["isr7"]),0xff,
		kb.get_tbl(),
		L["main"],
		DEV::init(),
		lcd.init(),
		lcd.show("Hello, world!"),
		pic.init86(0x00),
		Halt{}(),
		L["isr_kb"],
		kb.to_key(kb.get_scan()),
		lcd.show(),
		pic.eoi(1),
		Return{}(),
		L["isr2"],lcd.show('2'),pic.eoi(2),Return{}(),
		L["isr3"],lcd.show('3'),pic.eoi(3),Return{}(),
		L["isr4"],lcd.show('4'),pic.eoi(4),Return{}(),
		L["isr5"],lcd.show('5'),pic.eoi(5),Return{}(),
		L["isr6"],lcd.show('6'),pic.eoi(6),Return{}(),
		L["isr7"],lcd.show('7'),pic.eoi(7),Return{}(),
	};
}