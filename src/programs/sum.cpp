
#include <socp/program.hpp>
#include <soasm/soisv1.hpp>

using namespace SOASM;
using namespace SOISv1;

template<> Code SOCP::program<"sum">(){
	Label::tbl_t L;
	return {
		ImmVal{}(3),
		Pop{.to=Reg::A}(),
		ImmVal{}(0),
		Pop{.to=Reg::B}(),
		L["start"],
		Push{.from=Reg::A}(),
		BranchZero{}(L["end"]),

		Push{.from=Reg::A}(),
		Push{.from=Reg::B}(),
		Calc{.fn=Calc::FN::ADD}(),
		Pop{.to=Reg::B}(),

		Push{.from=Reg::A}(),
		ImmVal{}(1),
		Calc{.fn=Calc::FN::SUB}(),
		Pop{.to=Reg::A}(),

		Jump{}(L["start"]),
		L["end"],
		Halt{}(),
	};
}