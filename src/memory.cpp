

/*void Patch::Remove() {
    if (this->original_location) {
        Patch::Write(this->original_location, this->original_data.data(), this->original_data.size());
        this->original_location = nullptr;
    }
    original_data.clear();
}
void Patch::Write(void* where, void* with, std::size_t len) {
    // Get align info
    int page_size = sysconf(_SC_PAGE_SIZE);
    if (page_size == -1)
        throw std::runtime_error("Patch::Write: couldnt retrieve page size");
    // Align
    void* align = where - page_size;
    if (std::align(page_size, page_size, align, page_size * 4) == nullptr)
        throw std::logic_error("Patch::Write: couldnt properly align");
    // Align size
    std::size_t page_len = page_size;
    if (where + len > align + page_size)
        page_len += page_size;
    // Unprotect and write
    if (mprotect(align, page_len, PROT_READ | PROT_WRITE | PROT_EXEC) != 0)
        throw std::logic_error("Patch::Write: Unable to unprotect memory!")
    memcpy(addr, , size);
    if (mprotect(align, page_len, PROT_EXEC) != 0)
        throw std::logic_error("Patch::Write: Unable to reprotect memory!")
}*/
