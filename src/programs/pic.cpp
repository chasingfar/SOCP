
#include <socp/program.hpp>
#include <soasm/soisv1.hpp>
#include <socp/dev/lcd.hpp>
#include <socp/dev/pic.hpp>

using namespace SOCP::DEV;
using namespace SOASM::SOISv1;

template<> Code SOCP::program<"pic80">(){
	Label::tbl_t L;
	LCD lcd;
	PIC pic;
	return {
		DEV::init(),
		lcd.init(),
		lcd.show("Hello, world!"),
		pic.init80(0x00a0),
		Halt{}(),
		L["iv"].set(0x00a0),
		Jump{}(L["isr0"]),0xff,
		Jump{}(L["isr1"]),0xff,
		Jump{}(L["isr2"]),0xff,
		Jump{}(L["isr3"]),0xff,
		Jump{}(L["isr4"]),0xff,
		Jump{}(L["isr5"]),0xff,
		Jump{}(L["isr6"]),0xff,
		Jump{}(L["isr7"]),0xff,
		L["isr0"],lcd.show('a'),Return{}(),
		L["isr1"],lcd.show('b'),Return{}(),
		L["isr2"],lcd.show('c'),Return{}(),
		L["isr3"],lcd.show('d'),Return{}(),
		L["isr4"],lcd.show('e'),Return{}(),
		L["isr5"],lcd.show('g'),Return{}(),
		L["isr6"],lcd.show('h'),Return{}(),
		L["isr7"],lcd.show('i'),Return{}(),
	};
}