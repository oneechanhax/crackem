
#include <iostream>
#include <hack/sharedlibrary.hpp>
using namespace neko;

int main() {
    hack::SharedLibrary dummysl("libnekohack-dummy");
    std::cout << "Lmap: " << dummysl.GetLMap() << std::endl;

    using GetInt_f = int (*)();
    GetInt_f GetInt = dummysl.GetSym<GetInt_f>("GetInt");
    std::cout << "GetInt Location: " << std::hex << GetInt << std::endl;
    std::cout << "GetInt: " << GetInt() << std::endl;

}
