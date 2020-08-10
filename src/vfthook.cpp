
#include <cstring>

#include "memory.hpp"
#include "vfthook.hpp"

namespace crackem::vft::hook {

struct Ref::HookData {
    bool Verify() {
        return this->magic = kMagic;
    }
    static HookData* Get(VInst* v) {
        return mem::Offset<HookData>(*v, -int(sizeof(HookData)));
    }
    VInst* instance;
    VFT* original;
    std::size_t size;
    constexpr static uint32_t kMagic = 0xCAFEBEEF;
    uint32_t magic;
    VF* replacement[];
};

Ref::HookData* Ref::IntCreate(VInst* v) {
    if (HookData::Get(v)->Verify())
        throw std::runtime_error("Hook already exists!");
    
    std::size_t size = 0;
    while ((*v)[size]) 
        size++;
    
    HookData* hd = reinterpret_cast<HookData*>(malloc(sizeof(HookData) + (sizeof(VF*) * size)));
    hd->instance = v;
    hd->original = *v;
    hd->size = size;
    hd->magic = HookData::kMagic;
    memcpy(hd->replacement, hd->original, sizeof(VF*) * size);

    *hd->instance = hd->replacement;
    return hd; 
}
Ref::HookData* Ref::IntGet(VInst* v) {
    HookData* r = HookData::Get(v);
    if (r->Verify())
        return r;
    else
        throw std::runtime_error("Hook not found!");
}
bool Ref::IntContains(VInst* v) {
    return HookData::Get(v)->Verify();
}
void Ref::IntReplace(int idx, VF* fn) const {
    assert(this->data && !this->Exists(idx));
    this->data->replacement[idx] = fn;
}
VF* Ref::IntGetOriginal(int idx) const {
    assert(this->data);
    return this->data->original[idx];
}
void Ref::Clear(int idx) const {
    assert(this->data && this->Exists(idx));
    this->data->replacement[idx] = this->data->original[idx];
}
bool Ref::Exists(int idx) const {
    assert(this->data);
    return this->data->original[idx] == this->data->replacement[idx];
}
void Ref::Destroy() {
    assert(this->data);
    
    this->data = nullptr;
}

}
