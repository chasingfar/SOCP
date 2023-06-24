#include <socp/socp.hpp>
#include <soasm/asm.hpp>

using namespace SOCPU;
using namespace SOARCHv2;
using namespace std::views;

namespace SOCP{

	std::vector<link_t> get_linked(const DCSim::Wire *p){
		std::vector<link_t> arr;
		for(const auto& c:p->get_related()){
			if(auto chip=dynamic_cast<DCSim::Chip*>(c)){
				for(size_t i=0;const auto [w,b]:chip->pins){
					if(w->has(p)){
						arr.emplace_back(chip,i);
					}
					i++;
				}
			}
		}
		return arr;
	}
	std::ostream& operator<<(std::ostream& os,CTL ctl){
		using namespace magic_enum;
		using namespace ostream_operators;
		return os<<" Dir:"<<enum_name(ctl.dir)
			<<" ALU:"<<enum_name(ctl.xs)<<"="
				<<ALU::parse_fn(ctl.alu_c, ctl.alu_fn,
				enum_name(ctl.ys),
				enum_name(ctl.zs))
			<<" CF:"<<enum_name(ctl.CF)
			<<" index:"<<(unsigned)ctl.index;
	}
	void generateCUROM(std::string filename){
		generateROM(filename,iota(0uz)|take(1uz<<19uz)|transform([](size_t i){
			return SOARCHv2::uCode(i).generate<InstrSet>().val();
		}));
	}
	void list_instr(){
		for(auto [name,id,w]:InstrSet::list_instr()){
			std::cout<<std::format("{}:{} {}",name,id<<w,1<<w)<<std::endl;
		}
	}

	void disassemble(std::span<uint8_t> data){
		for(auto [addr,bytes,str]:SOASM::disassemble<InstrSet>(data)){
			std::string bytes_str;
			for(auto b:bytes){
				bytes_str+=std::bitset<8>(b).to_string()+" ";
			}
			std::cout<<std::format("{0:016b}:{1:27};{0}:{2}\n",addr,bytes_str,str);
		}
	}
	VM run_vm(SOASM::Code program,size_t N){
		VM vm;
	
		vm.load(program.assemble());
		vm.reset();
		for(size_t i=0;i<N;i++){
			vm.tick_instr();
			std::cout<<i<<std::endl;
			std::cout<<vm.print(vm.cpu.get_ptrs({MReg16::SP,MReg16::PC,MReg16::HL}))<<std::endl;
			if(vm.is_halt()){
				break;
			}
		}
		return vm;
	}
	Context run_emu(SOASM::Code program,size_t N,size_t stack_view_size){
		constexpr size_t mem_size=1<<16;
		std::array<uint8_t,mem_size> mem{};

		auto data=program.assemble();
		for(auto [addr,bytes,str]:SOASM::disassemble<InstrSet>(data)){
			std::string bytes_str;
			for(auto b:bytes){
				bytes_str+=std::bitset<8>(b).to_string()+" ";
			}
			std::cout<<std::format("{0:016b}:{1:27};{0}:{2}\n",addr,bytes_str,str);
		}
		std::ranges::move(data,mem.begin());
		Context ctx{mem};
		for(size_t i=0;i<N;i++){
			if(!ctx.run()){
				std::cout<<"halt"<<std::endl;
				break;
			}

			std::cout<<std::format(
				"{}:{:s};{:04x},{:04x};{:s}{:s}\n",
				i,
				ctx.reg.regs|transform([](auto i){
					return std::format(" {:02x}",i);
				})|join,
				ctx.pc,ctx.sp,
				iota((size_t)ctx.sp,mem_size)|take(stack_view_size)|transform([&](auto i){
					return std::format(" {:02x}",(int)ctx.mem[i]);
				})|join,
				(ctx.sp+stack_view_size<mem_size?"...":"")
			);
		}
		return ctx;
	}
}