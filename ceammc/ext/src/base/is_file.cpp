#include "is_file.h"
#include "ceammc_factory.h"
#include "datatype_string.h"

#include <fstream>

IsFile::IsFile(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
    createOutlet();
}

void IsFile::onSymbol(t_symbol* s)
{
    bool exists = fileExists(s->s_name);

    if (exists)
        symbolTo(1, s);

    boolTo(0, exists);
}

void IsFile::onDataT(const StringAtom& str)
{
    bool exists = fileExists(str->str());

    if (exists)
        atomTo(1, str);

    boolTo(0, exists);
}

bool IsFile::fileExists(const std::string& p) const
{
    std::string path(p);

    if (!isAbsolutePath(path.c_str()))
        path = findInStdPaths(path.c_str());

    if (path.empty())
        return false;

    std::ifstream f(path.c_str());
    return f.is_open();
}

void setup_is_file()
{
    ObjectFactory<IsFile> obj("is_file");
    obj.processData<DataTypeString>();

    obj.setDescription("checks if file exists and accessible");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({"predicate", "filesystem"});
    obj.setCategory("predicates");
    obj.setSinceVersion(0, 2);
}
