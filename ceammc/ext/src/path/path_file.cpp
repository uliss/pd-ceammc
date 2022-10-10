#include "path_file.h"
#include "args/argcheck2.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_platform.h"
#include "filesystem.hpp"
#include "fmt/core.h"
#include "parser_bytes.h"

namespace fs = ghc::filesystem;

namespace {
std::ios::seekdir to_seek_orgin(t_symbol* s)
{
    switch (s->s_name[0]) {
    case 'e':
        return std::ios::end;
    case 'c':
        return std::ios::cur;
    default:
    case 'b':
        return std::ios::beg;
    }
}

const char* mode_to_string(std::ios::openmode m)
{
    using namespace std;
    switch (m) {
    case ios::in:
        return "r";
    case ios::out | ios::trunc:
        return "w";
    case ios::out | ios::app:
        return "a";
    case ios::in | ios::out:
        return "r+";
    case ios::in | ios::out | ios::trunc:
        return "w+";
    case ios::in | ios::out | ios::app:
        return "a+";
    default:
        return "?";
    }
}
}

PathFile::PathFile(const PdArgs& args)
    : BaseObject(args)
    , fname_(nullptr)
{
    fname_ = new SymbolProperty("@path", &s_);
    addProperty(fname_);

    createOutlet();
    createOutlet();
}

void PathFile::m_open(t_symbol* s, const AtomListView& lv)
{
    static args::ArgChecker chk("FNAME:s MODE:s?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    if (!updateFullPath(lv)) {
        METHOD_ERR(s) << fmt::format("can't create full path: ") << lv;
        return;
    }

    std::ios::openmode mode = parse_mode(lv.symbolAt(1, &s_)->s_name);
    if (!mode) {
        METHOD_ERR(s) << fmt::format("invalid open mode: '{}', expected r, w, a, r+, w+ or a+",
            lv.symbolAt(1, &s_)->s_name);
        return;
    }

    if (!checkOpen(mode)) {
        METHOD_ERR(s) << fmt::format("can't open file '{}' with mode: {}",
            fname_->value()->s_name, mode_to_string(mode));
        return;
    }

    OBJ_LOG << fmt::format("file opened with mode '{}': '{}'", mode_to_string(mode), fname_->value()->s_name);
}

void PathFile::m_write_string(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() < 1) {
        METHOD_ERR(s) << fmt::format("ARGS... expected, got: ") << lv;
        return;
    }

    if (!fs_.is_open()) {
        METHOD_ERR(s) << fmt::format("file not opened: '{}'", fname_->value()->s_name);
        return;
    }

    fs_ << to_string(lv);
    if (!fs_) {
        METHOD_ERR(s) << fmt::format("can't write to file: '{}'", fname_->value()->s_name);
    } else {
        fs_.sync();
        OBJ_LOG << fmt::format("write to file: {}", to_string(lv));
    }
}

void PathFile::m_write_line(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() < 1) {
        METHOD_ERR(s) << fmt::format("ARGS... expected, got: ") << lv;
        return;
    }

    if (!fs_.is_open()) {
        METHOD_ERR(s) << fmt::format("file not opened: '{}'", fname_->value()->s_name);
        return;
    }

    fs_ << to_string(lv) << std::endl;
    if (!fs_) {
        METHOD_ERR(s) << fmt::format("can't write to file: '{}'", fname_->value()->s_name);
    } else {
        fs_.sync();
        OBJ_LOG << fmt::format("write line to file: {}", to_string(lv));
    }
}

void PathFile::m_write_bytes(t_symbol* s, const AtomListView& lv)
{
    if (lv.size() < 1) {
        METHOD_ERR(s) << fmt::format("ARGS... expected, got: ") << lv;
        return;
    }

    if (!fs_.is_open()) {
        METHOD_ERR(s) << fmt::format("file not opened: '{}'", fname_->value()->s_name);
        return;
    }

    std::vector<char> out;
    auto res = parse_bytes(lv, out);
    if (res.first < 0) {
        METHOD_ERR(s) << parser_byte_error(lv, res);
        return;
    }

    fs_.write(out.data(), out.size());

    if (!fs_) {
        METHOD_ERR(s) << fmt::format("can't write to file: '{}'", fname_->value()->s_name);
    } else {
        fs_.sync();
        OBJ_LOG << fmt::format("write bytes to file: {}", to_string(lv));
    }
}

void PathFile::m_read_line(t_symbol* s, const AtomListView& lv)
{
    if (!fs_.is_open()) {
        METHOD_ERR(s) << fmt::format("file is not opened: '{}'", fname_->value()->s_name);
        return;
    }

    if (fs_.eof())
        return;

    std::string line;
    std::getline(fs_, line);

    listTo(0, AtomList::parseString(line.c_str()));

    if (fs_.eof())
        return bangTo(1);
}

void PathFile::m_read_bytes(t_symbol* s, const AtomListView& lv)
{
    static args::ArgChecker chk("NUM:i?");

    if (!chk.check(lv, this)) {
        chk.usage(this, s);
        return;
    }

    if (!fs_.is_open()) {
        METHOD_ERR(s) << fmt::format("file is not opened: '{}'", fname_->value()->s_name);
        return;
    }

    if (fs_.eof())
        return;

    const auto N = lv.intAt(0, 0);
    if (N <= 0) {
        AtomList lst;
        auto it = std::istreambuf_iterator<char>(fs_);
        auto e = std::istreambuf_iterator<char>();
        while (it != e) {
            lst.append(static_cast<int>(*it++));
        }

        listTo(0, lst);
    } else {
        std::unique_ptr<char[]> data(new char[N]);
        fs_.read(data.get(), N);
        AtomList lst;
        lst.reserve(N);
        for (size_t i = 0; i < fs_.gcount(); i++) {
            lst.append(static_cast<int>(data[i]));
        }

        listTo(0, lst);
    }

    if (fs_.eof())
        return bangTo(1);
}

void PathFile::m_seek_read(t_symbol* s, const AtomListView& lv)
{
    static args::ArgChecker chk("POS:i ORIG:s=beg|end|cur?");

    if (!chk.check(lv, this)) {
        chk.usage(this, s);
        return;
    }

    if (!fs_.is_open()) {
        METHOD_ERR(s) << fmt::format("file not opened: '{}'", fname_->value()->s_name);
        return;
    }

    auto off = lv.intAt(0, 0);
    auto orig = to_seek_orgin(lv.symbolAt(1, &s_));
    fs_.seekg(off, orig);

    OBJ_LOG << fmt::format("seek read to {} from {}", off, lv.symbolAt(1, &s_)->s_name);
}

void PathFile::m_seek_write(t_symbol* s, const AtomListView& lv)
{
    static args::ArgChecker chk("POS:i ORIG:s=beg|end|cur?");

    if (!chk.check(lv, this)) {
        chk.usage(this, s);
        return;
    }

    if (!fs_.is_open()) {
        METHOD_ERR(s) << fmt::format("file not opened: '{}'", fname_->value()->s_name);
        return;
    }

    auto off = lv.intAt(0, 0);
    auto orig = to_seek_orgin(lv.symbolAt(1, &s_));
    fs_.seekp(off, orig);

    OBJ_LOG << fmt::format("seek write to {} from {}", off, lv.symbolAt(1, &s_)->s_name);
}

void PathFile::m_close(t_symbol* s, const AtomListView& lv)
{
    if (!fs_.is_open()) {
        METHOD_ERR(s) << fmt::format("file not opened: '{}'", fname_->value()->s_name);
        return;
    }

    fs_.close();
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
    fs_.close();
    fs_.open(path, mode);
    return fs_.is_open();
}

void setup_path_file()
{
    ObjectFactory<PathFile> obj("file");

    obj.setXletsInfo({ "open, close, read, write..." }, { "read output", "bang: on EOF" });

    obj.addMethod("open", &PathFile::m_open);
    obj.addMethod("close", &PathFile::m_close);

    obj.addMethod("read_line", &PathFile::m_read_line);
    obj.addMethod("read_bytes", &PathFile::m_read_bytes);

    obj.addMethod("write", &PathFile::m_write_string);
    obj.addMethod("write_line", &PathFile::m_write_line);
    obj.addMethod("write_bytes", &PathFile::m_write_bytes);

    obj.addMethod("seek_write", &PathFile::m_seek_write);
    obj.addMethod("seek_read", &PathFile::m_seek_read);
}
