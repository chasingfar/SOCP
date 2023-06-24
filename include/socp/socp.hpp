#ifndef SOCP_SOCP_HPP
#define SOCP_SOCP_HPP

#include <cstdint>
#include <iostream>
#include <fstream>
#include <socpu/vm.hpp>
#include <socpu/soarchv2.hpp>

namespace SOCP{
	using SOCPU::SOARCHv2::CPU;
	using SOASM::SOISv1::InstrSet;
	using SOASM::SOISv1::Context;
	using VM=SOCPU::Sim::VM<CPU,InstrSet>;

	using link_t=std::pair<DCSim::Chip*,size_t>;
	std::vector<link_t> get_linked(const DCSim::Wire *p);
	template<typename T>
	void port_print_linked(const T& port){
		for(const auto& p: port.pins){
			for(auto [chip,i]:get_linked(&p)){
				std::cout<<chip->name<<"."<<i<<" ";
			}
			std::cout<<std::endl;
		}
	}

	std::ostream& operator<<(std::ostream& os,SOCPU::SOARCHv2::CTL ctl);

	void generateROM(std::ostream& os,std::ranges::input_range auto data){
		os<<"v2.0 raw\r\n";
		os<<std::hex;
		for(auto [i,v]:std::views::zip(std::views::iota(0),data)){
			os<<static_cast<uintmax_t>(v)<<(i%8==7?"\r\n":" ");
		}
	}
	void generateROM(std::string filename,std::ranges::input_range auto data){
		if(std::ofstream fout(filename);fout) {
			generateROM(fout,data);
		}else{
			std::cout<<std::format("Open {} failed\n",filename);
		}
	}
	void generateCUROM(std::string filename);
	void list_instr();
	void disassemble(std::span<uint8_t> data);
	VM run_vm(SOASM::Code program,size_t N);
	Context run_emu(SOASM::Code program,size_t N,size_t stack_view_size=10);
}
#endif //SOCP_SOCP_HPP