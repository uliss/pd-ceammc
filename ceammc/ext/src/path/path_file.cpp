#include "path_file.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_platform.h"
#include "filesystem.hpp"
#include "fmt/core.h"

namespace fs = ghc::filesystem;

namespace {
void to_bytes(const AtomListView& lv, std::string& out)
{
    out.reserve(lv.size());
    for (auto& a : lv) {
        if (a.isInteger()) {
            auto val = a.asT<int>();
            if (val >= 0 && val < 255)
                out.push_back(static_cast<char>(val));
        }
    }
}
}

PathFile::PathFile(const PdArgs& args)
    : BaseObject(args)
    , fname_(nullptr)
{
    fname_ = new SymbolProperty("@name", &s_);
    fname_->setArgIndex(0);
    addProperty(fname_);

    createInlet();
    createOutlet();
}

void PathFile::m_create(t_symbol* s, const AtomListView& args)
{
    if (!checkArgs(args, ARG_SYMBOL)) {
        METHOD_ERR(s) << fmt::format("FILENAME expected, got: ") << args;
        return;
    }

    if (fs_.is_open())
        fs_.close();

    if (!updateFullPath(args)) {
        METHOD_ERR(s) << fmt::format("can't create full path: ") << args;
        return;
    }

    if (checkExists()) {
        METHOD_ERR(s) << fmt::format("file already exists: '{}'", fname_->value()->s_name);
        return;
    }

    if (!checkOpen(std::ios::out))
        return;

    fs_.sync();
    OBJ_LOG << fmt::format("file created: '{}'", fname_->value()->s_name);
}

void PathFile::m_append_string(t_symbol* s, const AtomListView& args)
{
    if (!checkArgs(args, ARG_SYMBOL)) {
        METHOD_ERR(s) << fmt::format("FILENAME expected, got: ") << args;
        return;
    }

    if (!updateFullPath(args)) {
        METHOD_ERR(s) << fmt::format("can't create full path: ") << args;
        return;
    }

    if (!checkExists() || !checkOpen(std::ios::out))
        return;

    fs_ << to_string(args.subView(1));
    if (!fs_)
        METHOD_ERR(s) << fmt::format("can't write to file: '{}'", fname_->value()->s_name);
}

void PathFile::m_append_line(t_symbol* s, const AtomListView& args)
{
    if (!checkArgs(args, ARG_SYMBOL)) {
        METHOD_ERR(s) << fmt::format("FILENAME expected, got: ") << args;
        return;
    }

    if (!updateFullPath(args)) {
        METHOD_ERR(s) << fmt::format("can't create full path: ") << args;
        return;
    }

    if (!checkExists() || !checkOpen(std::ios::out))
        return;

    fs_ << to_string(args.subView(1)) << std::endl;
    if (!fs_)
        METHOD_ERR(s) << fmt::format("can't write to file: '{}'", fname_->value()->s_name);
}

void PathFile::m_append_bytes(t_symbol* s, const AtomListView& args)
{
    if (!checkArgs(args, ARG_SYMBOL)) {
        METHOD_ERR(s) << fmt::format("FILENAME expected, got: ") << args;
        return;
    }

    if (!updateFullPath(args)) {
        METHOD_ERR(s) << fmt::format("can't create full path: ") << args;
        return;
    }

    if (!checkExists() || !checkOpen(std::ios::out))
        return;

    std::string data;
    to_bytes(args.subView(1), data);

    fs_.write(data.c_str(), data.size());
    if (!fs_)
        METHOD_ERR(s) << fmt::format("can't write to file: '{}'", fname_->value()->s_name);
}

bool PathFile::updateFullPath(const AtomListView& args)
{
    if (args.empty() || !canvas()) {
        fname_->setValue(&s_);
        return false;
    }

    fs::path path = platform::expand_tilde_path(args[0].asT<t_symbol*>()->s_name);
    if (path.empty()) {
        fname_->setValue(&s_);
        return false;
    } else if (path.is_relative()) {
        auto dir = canvas_getdir(canvas());
        if (!dir || !dir->s_name) {
            fs::path new_path = platform::current_working_directory();
            new_path /= path;
            fname_->setValue(gensym(new_path.lexically_normal().c_str()));
            return true;
        }

        fs::path new_path = platform::expand_tilde_path(dir->s_name);
        new_path /= path;
        return fname_->setValue(gensym(new_path.lexically_normal().c_str()));
    } else
        return fname_->setValue(gensym(path.lexically_normal().c_str()));
}

bool PathFile::checkExists() const
{
    return fs::is_regular_file(fname_->value()->s_name);
}

bool PathFile::checkOpen(std::ios::openmode mode)
{
    auto path = fname_->value()->s_name;
    fs_.open(path, mode);
    if (!fs_) {
        OBJ_ERR << fmt::format("can't open file: '{}'", path);
        return false;
    } else
        return true;
}

void setup_path_file()
{
    ObjectFactory<PathFile> obj("file");
    obj.addMethod("create", &PathFile::m_create);
    obj.addMethod("append", &PathFile::m_append_string);
    obj.addMethod("append_line", &PathFile::m_append_line);
    obj.addMethod("append_bytes", &PathFile::m_append_bytes);
}
