#include "path_listdir.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_platform.h"
#include "datatype_string.h"

#include <cerrno>
#include <cstring>

#include "config.h"
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#else
#error <dirent.h> is missing...
#endif

PathListDir::PathListDir(const PdArgs& a)
    : BaseObject(a)
    , match_(&s_)
{
    createOutlet();

    addProperty(new PointerProperty<t_symbol*>("@match", &match_, PropValueAccess::READWRITE));
    createInlet(&match_);

    createCbListProperty(
        "@path",
        [this]() -> AtomList { return AtomList(gensym(path_.c_str())); },
        [this](const AtomListView& lv) -> bool { path_ = to_string(lv); return true; })
        ->setArgIndex(0);
}

void PathListDir::onBang()
{
    readDirList();
    listTo(0, ls_);
}

void PathListDir::onSymbol(t_symbol* path)
{
    path_ = path->s_name;
    onBang();
}

void PathListDir::onDataT(const StringAtom& dptr)
{
    path_ = dptr->str();
    onBang();
}

void PathListDir::readDirList()
{
    if (path_.empty())
        return;

    ls_.clear();

    std::string path = platform::expand_tilde_path(path_);

    if (!sys_isabsolutepath(path.c_str())) {
        if (canvas()) {
            t_symbol* canvas_dir = canvas_getdir(canvas());
            if (canvas_dir)
                path = std::string(canvas_dir->s_name) + "/" + path;
        }
    }

    std::unique_ptr<DIR, int (*)(DIR*)> dir(opendir(path.c_str()),
        [](DIR* d) -> int { return d ? closedir(d) : 0; });

    if (!dir) {
        OBJ_ERR << "can't read directory: '" << path << "'. Error: " << strerror(errno);
        return;
    }

    struct dirent* p_dirent;
    while ((p_dirent = readdir(dir.get())) != NULL) {
        // skip hidden files on UNIX
        if (p_dirent->d_name[0] == '.')
            continue;

        if (match_->s_name[0] != 0) {
            if (platform::fnmatch(match_->s_name, p_dirent->d_name)) {
                ls_.append(StringAtom(p_dirent->d_name));
            }
        } else {
            ls_.append(StringAtom(p_dirent->d_name));
        }
    }
}

void setup_path_lsdir()
{
    ObjectFactory<PathListDir> obj("path.lsdir");
    obj.addAlias("path.ls");
    obj.processData<DataTypeString>();
    obj.parseArgsMode(PdArgs::PARSE_COPY);
    obj.parsePropsMode(PdArgs::PARSE_COPY);

    obj.setXletsInfo({ "bang: list directory\n"
                       "symbol: set directory path and output",
                         "symbol: set @match property" },
        { "list: list of strings" });
}
