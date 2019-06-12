
#include <cstring>

#include "vfthook.hpp"

namespace neko::hack {

VFTHook::VFTHook(){}
VFTHook::~VFTHook() {
    try {
        this->Undo();
    }catch(...){}
}

void VFTHook::IntInit(void* instance) {
    try {
        this->Undo();
    }catch(...){}

    this->vtable = reinterpret_cast<void***>(instance);
    this->original_data = *this->vtable;

    this->size = 0;
    while (this->original_data[this->size]) this->size++;

    this->replacement = std::make_unique<VFT>(this->size);
    memcpy(this->replacement.get(), this->original_data, sizeof(VF) * this->size);
};
void VFTHook::Finish() {
    if (this->IsHooked())
        throw std::logic_error("VTFHook: Cant finish, hook in place");
    *this->vtable = this->replacement.get();
}
void VFTHook::Undo() {
    if (!this->IsHooked())
        throw std::logic_error("VTFHook: Cant undo a hook that isnt there");
}
bool VFTHook::IsHooked() {
    if (!this->vtable || !this->replacement)
        return false;
    return *this->vtable == this->replacement.get();
}

void VFTHook::IntHook(VF func, std::size_t idx) {
    //if (this->original[idx] != this->replacement[idx])
        //throw std::logic_error("Already Hooked");
    this->replacement[idx] = func;
}

}
