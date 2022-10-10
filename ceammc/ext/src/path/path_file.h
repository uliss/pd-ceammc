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

    void m_create(t_symbol* s, const AtomListView& args);
    void m_append_string(t_symbol* s, const AtomListView& args);
    void m_append_line(t_symbol* s, const AtomListView& args);
    void m_append_bytes(t_symbol* s, const AtomListView& args);

private:
    bool updateFullPath(const AtomListView& args);
    bool checkExists() const;
    bool checkOpen(std::ios::openmode mode);
};

void setup_path_file();

#endif // CEAMMC_PATH_FILE_H
