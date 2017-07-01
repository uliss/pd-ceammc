#include "path_listdir.h"
#include "../string/datatype_string.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_platform.h"
#include "config.h"

#include <cerrno>
#include <cstring>

#ifdef HAVE_DIRENT_H
#include <dirent.h>
#else
#error <dirent.h> is missing...
#endif

PathListDir::PathListDir(const PdArgs& a)
    : BaseObject(a)
    , match_(0)
    , cnv_(canvas_getcurrent())
{
    createOutlet();

    createProperty(new PointerProperty<t_symbol*>("@match", &match_, false));

    path_ = to_string(positionalArguments());
    readDirList();
}

void PathListDir::onBang()
{
    listTo(0, ls_.toList());
}

void PathListDir::onSymbol(t_symbol* path)
{
    path_ = path->s_name;
    readDirList();
    onBang();
}

void PathListDir::onDataT(const DataTypeString& s)
{
    path_ = s.str();
    readDirList();
    onBang();
}

void PathListDir::m_match(t_symbol*, const AtomList& lst)
{
    if (lst.empty()) {
        match_ = 0;
        return;
    }

    if (lst[0].isSymbol())
        match_ = lst[0].asSymbol();
}

void PathListDir::readDirList()
{
    if (path_.empty())
        return;

    ls_.clear();

    std::string path = path_;

    if (!sys_isabsolutepath(path.c_str())) {
        if (cnv_) {
            t_symbol* canvas_dir = canvas_getdir(cnv_);
            if (canvas_dir)
                path = std::string(canvas_dir->s_name) + "/" + path;
        }
    }

    DIR* dir = opendir(path.c_str());
    if (dir == NULL) {
        OBJ_ERR << "can't read directory: '" << path << "'. Error: " << strerror(errno);
        return;
    }

    struct dirent* p_dirent;
    while ((p_dirent = readdir(dir)) != NULL) {
        // skip hidden files on UNIX
        if (p_dirent->d_name[0] == '.')
            continue;

        if (match_ != 0) {
            if (platform::fnmatch(match_->s_name, p_dirent->d_name)) {
                DataPtr d(new DataTypeString(p_dirent->d_name));
                ls_.append(d.asAtom());
            }
        } else {
            DataPtr d(new DataTypeString(p_dirent->d_name));
            ls_.append(d.asAtom());
        }
    }

    closedir(dir);
}

extern "C" void setup_path0x2elsdir()
{
    ObjectFactory<PathListDir> obj("path.lsdir");
    obj.addAlias("path.ls");
    obj.addMethod("match", &PathListDir::m_match);
    obj.processData<DataTypeString>();
}
