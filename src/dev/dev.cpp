#include <socp/dev/dev.hpp>
#include <soasm/soisv1.hpp>

using namespace SOCP::DEV;
using namespace SOASM::SOISv1;
using namespace SOASM::SOISv1::Regs;

static constexpr Reg16 dev_ptr=Reg16::DC;
inline Code imm(Reg reg,uint8_t v){return {ImmVal{}(v),Pop{.to=reg}(),};}

namespace SOCP::DEV{
	Code init(){return imm(toH(dev_ptr),0x40);}
	Code set_addr(uint8_t addr){return imm(toL(dev_ptr),addr);}
	Code write(uint8_t v){return SaveImm{.to=dev_ptr}(v);}
	Code write(){return Save{.to=dev_ptr}();}
	Code read(){return Load{.from=dev_ptr}();}
}