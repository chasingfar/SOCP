
#include <socp/socp.hpp>
#include <socp/program.hpp>

using namespace SOCP;

int main() {
	generateCUROM("soarchv2-soisv1-5.txt");
	auto data=program<"bootloader">().assemble();
	generateROM("bootloader-soisv1-5.txt",data);
	std::cout<<"bootloader"<<std::endl;
	disassemble(data);

	auto hello=program<"hello">().assemble(0x0000);
	if(std::ofstream fout{"BOOT.BIN"};fout){
		fout.write((char*)hello.data(),hello.size());
	}
	std::cout<<"hello"<<std::endl;
	disassemble(hello);
	return 0;
}
