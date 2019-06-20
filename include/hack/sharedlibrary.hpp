
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

#pragma once

#if defined(__linux__)
#include <link.h>  // link maps
using LMap = link_map*;
using SymStr = const char*;
#elif defined(_WIN32)
#include <windows.h>  // loadlibrary
using LMap = HMODULE;
using SymStr = LPCSTR;
#endif

#include <string_view>
#include <filesystem>
namespace fs = std::filesystem;

namespace neko::hack {

class SharedLibrary {
public:
    SharedLibrary();
    SharedLibrary(std::string_view _name);
    void Init() { this->Init(this->name); }
    void ForceInit() { this->ForceInit(this->name); }
    void Init(std::string_view _name);
    void ForceInit(std::string_view _name);
    std::string_view GetName();
    void* begin();
    void* end();
    std::size_t size();
    void Clear();
    LMap GetLMap();
    template<typename T>
    T GetSym(SymStr s) { return reinterpret_cast<T>(this->GetSymInternal(s)); }
private:
    void* GetSymInternal(SymStr);
    bool init_flag = false;
    std::string_view name;
    fs::path path;
    void* _begin;
    void* _end;
    std::size_t _size;
    LMap lmap;
};

}
