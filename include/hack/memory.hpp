
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

namespace neko::hack::mem {

template <typename Type, class Ptr>
inline auto Offset(Ptr src, std::ptrdiff_t offset) {
    return reinterpret_cast<Type*>(reinterpret_cast<uint64_t>(src) + offset);
}

/*class Patch {
public:
    Patch();
    void Add(){

    }
    void Remove();
    static void Write(void* where, std::byte* with, std::size_t len);
private:
    void* original_location = nullptr;
    std::vector<std::byte> original_data;
};*/


}
