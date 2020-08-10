
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
using Handle = link_map*;
using SymStr = const char*;
#elif defined(_WIN32)
#include <windows.h>  // loadlibrary
using Handle = HMODULE;
using SymStr = LPCSTR;
#endif

#include <string_view>
#include <filesystem>

namespace crackem {
namespace fs = std::filesystem;

class SharedLibrary {
public:
    SharedLibrary();
    SharedLibrary(std::string_view _name);
    SharedLibrary(const SharedLibrary&);
    SharedLibrary(SharedLibrary&&);
    void Load();
    void Load(std::string_view _name);
    
    SharedLibrary& operator=(SharedLibrary&& v);
    SharedLibrary& operator=(const SharedLibrary& v);

    bool IsLoaded() const;
    std::string_view GetName() const;
    const fs::path& GetPath() const;
    void* begin() const;
    void* end() const;
    std::size_t size() const;
    Handle GetHandle() const;

    void Clear() const;
    template<typename T>
    T GetSym(SymStr s) { return reinterpret_cast<T>(this->GetSymInternal(s)); }
private:
    void* GetSymInternal(SymStr) const;

    bool loaded;
    std::string_view name;
    fs::path path;
    void* _begin;
    void* _end;
    std::size_t _size;
    Handle lmap;
};

}
