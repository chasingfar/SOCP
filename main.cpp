
#include <socp/socp.hpp>
#include <socp/program.hpp>

using namespace SOCP;

int main() {
	//generateCUROM("soarchv2-soisv1-5.txt");
	auto data=program<"openfile">().assemble();
	generateROM("openfile-soisv1-5.txt",data);
	disassemble(data);
	return 0;
}
