
#include <socp/program.hpp>
#include <soasm/soisv1.hpp>
#include <socp/dev/lcd.hpp>
#include <socp/dev/pic.hpp>
#include <socp/dev/kb64.hpp>
#include <socp/dev/ch376s.hpp>

using namespace SOCP::DEV;
using namespace SOASM::SOISv1;
using u8_=SOCC::u8;
using namespace CH376S_CONST;

template<> SOASM::Code SOCP::program<"openfile">(){
	SOASM::Label::tbl_t L;
	LCD lcd;
	PIC pic;
	KB64 kb;
	CH376S sd;
	u8_ RegA{RegVar<Reg>::make(Reg::A)};
	u8_ RegB{RegVar<Reg>::make(Reg::B)};
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

		RegB=1_u8,
		do_({
			RegA=sd.cmd_r(CMD11_CHECK_EXIST,0x65),
		}).while_(RegA!=0x9A_u8),

		RegB=2_u8,
		RegA=sd.cmd_r(CMD11_SET_USB_MODE,(uint8_t)USB_MODE::SD_HOST),
		if_(RegA!=u8_(CMD_RET_SUCCESS)).then({asm_(Halt{}())}).end(),
		
		RegB=3_u8,
		RegA=sd.cmd_int(CMD0H_DISK_MOUNT),
		if_(RegA!=u8_(USB_INT_SUCCESS)).then({asm_(Halt{}())}).end(),
		
		RegB=4_u8,
		sd.cmd_s(CMD10_SET_FILE_NAME,"TEST4.TXT"),

		RegB=5_u8,
		RegA=sd.cmd_int(CMD0H_FILE_OPEN),
		if_(RegA!=u8_(USB_INT_SUCCESS)).then({asm_(Halt{}())}).end(),
		
		RegB=6_u8,
		RegA=sd.cmd_int(CMD2H_BYTE_READ,255,0),
		if_(RegA!=u8_(USB_INT_DISK_READ)).then({asm_(Halt{}())}).end(),

		RegA=sd.cmd_r(CMD01_RD_USB_DATA0),
		RegB=RegA,
		while_(RegA).do_({
			lcd.show(sd.data()),
			RegA-=1_u8,
		}).end(),
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
		sd.libs(),
	};
}