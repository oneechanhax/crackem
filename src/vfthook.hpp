
/*
 * Nekohack: Break things in cool ways!
 * Copyright (C) 2018 Rebekah Rowe
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <cassert>
#include <cstdint>
#include <memory>

namespace crackem::vft::hook {

using VF = void;
using VFT = VF*;
using VInst = VFT*;

class Ref {
    struct HookData;
    static HookData* IntCreate(VInst*);
    static HookData* IntGet(VInst*);
    static bool IntContains(VInst*);

    HookData* data = nullptr;
    void IntReplace(int idx, VF*) const;
    VF* IntGetOriginal(int idx) const;
public:
    Ref() {}
    Ref(HookData* _data) : data(_data) { assert(this->data); }
    void Clear(int idx) const;
    bool Exists(int idx) const;
    void Destroy();

    template<class T> Ref(T* inst) : Ref(Ref::Get(inst)) {
        static_assert(!std::is_same_v<T, HookData>);
    }
    template<class T> void Replace(int idx, T replace_func) const {
        this->IntReplace(idx, reinterpret_cast<void*>(&replace_func));
    }
    template<typename T>
    T GetOriginal(std::size_t idx) const {
        return reinterpret_cast<T>(this->IntGetOriginal(idx));
    }
    template<typename Ret, typename... T>
    Ret CallOriginal(std::size_t idx, T... args) const {
        using Func_t = Ret (*)(T...);
        return this->GetOriginal<Func_t>(idx)(args...);
    }
    template<class T> static Ref Create(T* instance) {
        return IntCreate(reinterpret_cast<VInst*>(instance));
    }
    template<class T> static Ref Get(T* instance) {
        return IntGet(reinterpret_cast<VInst*>(instance));
    }
    template<class T> static bool Contains(T* instance) {
        return IntContains(reinterpret_cast<VInst*>(instance));
    }
};

}
