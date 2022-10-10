#ifndef CEAMMC_PATH_FILE_H
#define CEAMMC_PATH_FILE_H

#include "ceammc_object.h"
using namespace ceammc;

#include <fstream>

class PathFile : public BaseObject {
    SymbolProperty* fname_;
    std::fstream fs_;

public:
    PathFile(const PdArgs& args);

    void m_create(t_symbol* s, const AtomListView& lv);
    void m_write_string(t_symbol* s, const AtomListView& lv);
    void m_write_line(t_symbol* s, const AtomListView& lv);
    void m_write_bytes(t_symbol* s, const AtomListView& lv);
    void m_seek_write(t_symbol* s, const AtomListView& lv);

private:
    bool updateFullPath(const AtomListView& args);
    bool checkExists() const;
    bool checkOpen(std::ios::openmode mode);
};

void setup_path_file();

#endif // CEAMMC_PATH_FILE_H
