
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

#include <cstring>

#include "memory.hpp"

#include "sharedlibrary.hpp"

namespace neko::hack {

struct Passthrough {
    std::string name;
    fs::path path;
    void* begin;
    std::size_t size;
};

SharedLibrary::SharedLibrary(){}
SharedLibrary::SharedLibrary(std::string_view _name) : name(_name) {
    try {
        this->Init(_name);
    } catch(...){}
}

void SharedLibrary::Init(std::string_view _name){
    if (!init_flag) {
        this->name = _name;
        if (this->name.empty())
            throw std::logic_error("SharedLibrary: no name avaiable.");
        Passthrough pass;
        pass.name = '/' + std::string(_name) + '.';
        if (!dl_iterate_phdr([](struct dl_phdr_info* info, size_t, void* _pass) {
            auto* pass = reinterpret_cast<Passthrough*>(_pass);
            if (!strstr(info->dlpi_name, pass->name.c_str()))
                return 0;

            pass->path = info->dlpi_name;
            pass->begin = reinterpret_cast<void*>(info->dlpi_addr + info->dlpi_phdr[0].p_vaddr);
            pass->size = info->dlpi_phdr[0].p_memsz;
            return 1;
        }, reinterpret_cast<void*>(&pass)))
            throw std::runtime_error("SharedLibrary: Unable to find loaded library");

        this->lmap = static_cast<LMap>(
                dlopen(path.c_str(), RTLD_NOLOAD | RTLD_NOW | RTLD_LOCAL));
        if (const char* error = dlerror())
            throw std::runtime_error("SharedLibrary recieved dlerror: + error");

        this->path = pass.path;
        this->_begin = pass.begin;
        this->_end = mem::Offset<void*>(pass.begin, pass.size);
        this->_size = pass.size;
        this->init_flag = true;
    }
}
void SharedLibrary::ForceInit(std::string_view _name) {
    while(!this->init_flag) {
        this->Init();
    }
}
std::string_view SharedLibrary::GetName() {
    return name;
}
void* SharedLibrary::begin() {
    this->Init();
    return this->_begin;
}
void* SharedLibrary::end() {
    this->Init();
    return this->_end;
}
std::size_t SharedLibrary::size() {
    this->Init();
    return this->_size;
}
LMap SharedLibrary::GetLMap() {
    this->Init();
    return this->lmap;
}
void SharedLibrary::Clear(){
    this->init_flag = false;
    this->name = std::string_view();
}
void* SharedLibrary::GetSymInternal(SymStr name) {
    #if defined(__linux__)
        return dlsym(this->GetLMap(), name);
    #elif defined(_WIN32)
        return GetProcAddress(this->GetLMap(), name);
    #endif
}

}
