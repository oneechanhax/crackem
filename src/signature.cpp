
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
#include <algorithm>
#include <vector>
#include <string>

#include "signature.hpp"

class Pattern {
public:
    Pattern(std::string_view input) {
        while (!input.empty()) {
            // Discard whitespace
            std::size_t non_white_start = input.find_first_not_of(' ');
            if (non_white_start == std::string_view::npos) break;
            input.remove_prefix(non_white_start);

            // act on delim
            // Wildcard
            if (input.front() == '?') {
                this->wilds.push_back(true);
                std::size_t next = input.find_first_not_of('?');
                if (next == std::string_view::npos)
                    break;
                input.remove_prefix(next);
            } else {
                this->wilds.push_back(false);
                if (input.size() < 2)
                    break;

                std::string hex = {input[0], input[1]};
                uint8_t nibble = std::stoul(hex, nullptr, 16);
                this->pat.push_back(*reinterpret_cast<std::byte*>(nibble));
                input.remove_prefix(2);
            }
        }
    }
    inline std::byte Get(std::size_t i) {
        assert(!this->IsWild(i));
        return pat[i];
    }
    inline bool IsWild(std::size_t i) {
        auto find = std::find(wilds.begin(), wilds.end(), i);
        return find != wilds.end();
    }
    inline std::size_t size(){
        return this->pat.size();
    }
private:
std::vector<std::byte> pat;
std::vector<bool> wilds;
};

void* SearchN(void* _begin, void* _end, std::string_view _pat){
    return nullptr;
    /*Pattern pat(_pat);
    std::byte* begin = reinterpret_cast<std::byte*>(_begin);
    std::byte* end = reinterpret_cast<std::byte*>(_end);
    auto find = std::find_if(begin, end, [&](std::byte* s) {
        // Pattern equality
        for (int )
    });*/
}
