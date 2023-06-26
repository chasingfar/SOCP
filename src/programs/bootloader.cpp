
#include <socp/program.hpp>
#include <soasm/soisv1.hpp>
#include <socp/dev/ch376s.hpp>

using namespace SOCP::DEV;
using namespace SOASM::SOISv1;
using u8_=SOCC::u8;
using namespace CH376S_CONST;

template<> SOASM::Code SOCP::program<"bootloader">(){
	SOASM::Label::tbl_t L;
	CH376S sd;
	u8_ RegA{RegVar<Reg>::make(Reg::A)};
	u8_ RegB{RegVar<Reg>::make(Reg::B)};
	L["start"].set(0x0400);
	return {
		Jump{}(L["main"]),0xff,
		Jump{}(L["isr2"]),0xff,
		Jump{}(L["isr2"]),0xff,
		Jump{}(L["isr3"]),0xff,
		Jump{}(L["isr4"]),0xff,
		Jump{}(L["isr5"]),0xff,
		Jump{}(L["isr6"]),0xff,
		Jump{}(L["isr7"]),0xff,
		L["isr1"],ImmVal{}(1),Jump{}(L["start"]),
		L["isr2"],ImmVal{}(2),Jump{}(L["start"]),
		L["isr3"],ImmVal{}(3),Jump{}(L["start"]),
		L["isr4"],ImmVal{}(4),Jump{}(L["start"]),
		L["isr5"],ImmVal{}(5),Jump{}(L["start"]),
		L["isr6"],ImmVal{}(6),Jump{}(L["start"]),
		L["isr7"],ImmVal{}(7),Jump{}(L["start"]),
		sd.libs(),
		L["main"],
		DEV::init(),

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
		sd.cmd_s(CMD10_SET_FILE_NAME,"BOOT.BIN"),

		RegB=5_u8,
		RegA=sd.cmd_int(CMD0H_FILE_OPEN),
		if_(RegA!=u8_(USB_INT_SUCCESS)).then({asm_(Halt{}())}).end(),
		
		RegB=6_u8,
		InplaceFn<void_()>{[&](auto& _)->Stmt{
			u8_ s{_};
			ptr<u8_> pos{_};
			return {
				pos=ptr<u8_>(L["start"]),
				RegB=8_u8,
				sd.cmd(CMD2H_BYTE_READ,0xff,0xff),
				loop_({
					RegB+=1_u8,
					RegA=sd.wait_int(),
					if_(RegA==u8_(USB_INT_DISK_READ)).then({
						s=sd.cmd_r(CMD01_RD_USB_DATA0),
						do_({
							*pos=sd.data(),
							pos+=1_u16,
							s-=1_u8,
						}).while_(s),
						sd.cmd(CMD0H_BYTE_RD_GO),
					}).else_({
						_.return_(),
					}),
				}).end(),
			};
		}},
		RegB=7_u8,
		ImmVal{}(0),
		Jump{}(L["start"]),
		Halt{}(),
	};
}