
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

#include <cstdint>
#include <memory>

namespace neko::hack {

// Virtual function table
using VF = void*;
using VFT = VF[];

// Virtual Function Table Hook
class VFTHook {
public:
    VFTHook();
    ~VFTHook();
    template<class TPtr>
    void Init(TPtr instance) {
        this->IntInit(reinterpret_cast<void*>(instance));
    }
    template<class TPtr>
    void Hook(TPtr func, std::size_t idx) {
        this->IntHook(reinterpret_cast<VF>(func), idx);
    }
    void Finish();
    void Undo();
    template <class T = void*>
    inline T GetOriginal(std::size_t idx) const {
        return reinterpret_cast<T>(this->original_data[idx]);
    }
    bool IsHooked();
    std::size_t GetSize(){ return this->size; }
private:
    void IntInit(void* instance);
    void IntHook(VF func_ptr, std::size_t);
    void*** vtable = nullptr;
    void** original_data = nullptr;
    std::unique_ptr<void*[]> replacement;
    std::size_t size;
};

}
