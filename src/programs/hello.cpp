
#include <socp/program.hpp>
#include <soasm/soisv1.hpp>
#include <socp/dev/lcd.hpp>

using namespace SOCP::DEV;
using namespace SOASM::SOISv1;
using u8_=SOCC::u8;

template<> SOASM::Code SOCP::program<"hello">(){
	LCD lcd;
	return {
		lcd.init(),
		InplaceFn<void_()>{[&](auto& _)->Stmt{
			u8_ i{_};
			return {
				for_(i=0_u8,i<40_u8,i+=1_u8).do_({
					asm_(lcd.show("Hello world!")),
				}).end(),
				_.return_(),
			};
		}},
		Halt{}(),
	};
}