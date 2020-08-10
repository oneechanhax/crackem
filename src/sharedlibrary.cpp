
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
#include <cstring>
#include <dlfcn.h>

#include "memory.hpp"

#include "sharedlibrary.hpp"

namespace crackem {

SharedLibrary::SharedLibrary() : loaded(false) {}
SharedLibrary::SharedLibrary(std::string_view _name) { this->Load(_name); }
SharedLibrary::SharedLibrary(const SharedLibrary& v) { *this = v; }
SharedLibrary::SharedLibrary(SharedLibrary&& v) { *this = v; }
SharedLibrary& SharedLibrary::operator=(SharedLibrary&& v) {
    this->loaded = false;
    this->name = v.name;
    if (v.loaded) {
        v.loaded = false;    
        this->path = std::move(v.path);
        this->_begin = v._begin;
        this->_end = v._end;
        this->_size = v._size;
        this->lmap = v.lmap;
        this->loaded = true;
    }
    return *this;
}
SharedLibrary& SharedLibrary::operator=(const SharedLibrary& v) {
    this->loaded = false;
    this->name = v.name;
    if (v.loaded) {
        this->path = v.path;
        this->_begin = v._begin;
        this->_end = v._end;
        this->_size = v._size;
        this->lmap = v.lmap;
        this->loaded = true;
    }
    return *this;
}
void SharedLibrary::Load(std::string_view _name) {
    this->name = _name;
    this->Load();
}
void SharedLibrary::Load() { 
    assert(!this->loaded && !this->name.empty());
    this->loaded = false;
    
    if (!dl_iterate_phdr([](struct dl_phdr_info* info, size_t, void* _pass) {
        auto* _this = reinterpret_cast<SharedLibrary*>(_pass);
        _this->path = info->dlpi_name;
        if (_this->path.stem() != _this->name/* || _this->path.extention() ==*/)
            return 0;

        _this->_begin = reinterpret_cast<void*>(info->dlpi_addr + info->dlpi_phdr[0].p_vaddr);
        _this->_size = info->dlpi_phdr[0].p_memsz;
        return 1;
    }, reinterpret_cast<void*>(this)))
        return;
    
    this->loaded = true;
    this->_end = mem::Offset<void*>(this->_begin, this->_size);
    this->lmap = static_cast<Handle>(dlopen(this->path.c_str(), RTLD_NOLOAD));
    if (const char* error = dlerror())
        throw std::runtime_error("SharedLibrary recieved dlerror(dlopen): " + std::string(error));
}
bool SharedLibrary::IsLoaded() const {
    return this->loaded;
}
std::string_view SharedLibrary::GetName() const {
    assert(this->loaded);
    return this->name;
}
const fs::path& SharedLibrary::GetPath() const {
    assert(this->loaded);
    return this->path;
}
void* SharedLibrary::begin() const {
    assert(this->loaded);
    return this->_begin;
}
void* SharedLibrary::end() const {
    assert(this->loaded);
    return this->_end;
}
std::size_t SharedLibrary::size() const {
    assert(this->loaded);
    return this->_size;
}
Handle SharedLibrary::GetHandle() const {
    assert(this->loaded);
    return this->lmap;
}
void* SharedLibrary::GetSymInternal(SymStr name) const {
    #if defined(__linux__)
        void* symbol = dlsym(this->GetHandle(), name);
        if (const char* error = dlerror())
            throw std::runtime_error("SharedLibrary recieved dlerror(dlsym): " + std::string(error));
        return symbol;
    #elif defined(_WIN32)
        return GetProcAddress(this->GetLMap(), name);
    #else
        assert(false);
    #endif
}

}
