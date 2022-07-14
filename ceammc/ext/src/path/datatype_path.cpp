/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "datatype_path.h"
#include "ceammc_atomlist.h"
#include "ceammc_datastorage.h"
#include "ceammc_format.h"

#include "filesystem.hpp"

constexpr const char* TYPE_NAME = "Path";

namespace ceammc {
namespace path {

    namespace fs = ghc::filesystem;

    const DataTypeId DataTypePath::dataType = DataStorage::instance().registerNewType(TYPE_NAME,
        [](const AtomListView& lv) { return Atom(new DataTypePath(ceammc::to_string(lv))); });

    DataTypePath::DataTypePath()
    {
    }

    DataTypePath::~DataTypePath()
    {
        // for std::unique_ptr
    }

    DataTypePath::DataTypePath(DataTypePath&& path)
        : path_(std::move(path.path_))
    {
    }

    DataTypePath::DataTypePath(const DataTypePath& path)
        : path_(path.path_)
    {
    }

    DataTypePath::DataTypePath(const char* path)
        : path_(path)
    {
    }

    DataTypePath::DataTypePath(const std::string& path)
        : path_(path)
    {
    }

    DataTypePath::DataTypePath(Path&& path)
        : path_(std::move(path))
    {
    }

    DataTypePath::DataTypePath(const Path& path)
        : path_(path)
    {
    }

    DataTypePath& DataTypePath::operator=(const DataTypePath& path)
    {
        path_ = path.path_;
        return *this;
    }

    DataTypePath& DataTypePath::operator=(DataTypePath&& path)
    {
        path_ = std::move(path.path_);
        return *this;
    }

    DataTypePath* DataTypePath::clone() const
    {
        return new DataTypePath(*this);
    }

    DataTypeId DataTypePath::type() const noexcept
    {
        return dataType;
    }

    std::string DataTypePath::toJsonString() const
    {
        return fmt::format("\"{}\"", string::escape_for_json(path_.string()));
    }

    bool DataTypePath::isEqual(const AbstractData* d) const noexcept
    {
        auto path = d->as<DataTypePath>();

        // not a dict
        if (!path)
            return false;

        // self check
        if (path == this)
            return true;

        return path->path_ == path_;
    }

    bool DataTypePath::operator==(const DataTypePath& path) const noexcept
    {
        return path_ == path.path_;
    }

    bool DataTypePath::is_absolute() const
    {
        return path_.is_absolute();
    }

    bool DataTypePath::is_relative() const
    {
        return path_.is_relative();
    }

    bool DataTypePath::is_directory() const
    {
        return fs::is_directory(path_);
    }

    bool DataTypePath::is_regular_file() const
    {
        return fs::is_regular_file(path_);
    }

    bool DataTypePath::exists() const
    {
        return fs::exists(path_);
    }

    std::string DataTypePath::root_name() const
    {
        return path_.root_name().string();
    }

    std::string DataTypePath::root_directory() const
    {
        return path_.root_directory().string();
    }

    std::string DataTypePath::root_path() const
    {
        return path_.root_path().string();
    }

    std::string DataTypePath::relative_path() const
    {
        return path_.relative_path().string();
    }

    DataTypePath DataTypePath::parent_path() const
    {
        return DataTypePath(path_.parent_path());
    }

    std::string DataTypePath::filename() const
    {
        return path_.filename().string();
    }

    std::string DataTypePath::extension() const
    {
        return path_.extension().string();
    }

    std::uintmax_t DataTypePath::file_size() const
    {
        return fs::file_size(path_);
    }

    DataTypePath DataTypePath::lexically_normal() const
    {
        return DataTypePath(path_.lexically_normal());
    }

    DataTypePath& DataTypePath::replace_extension(const std::string& ext)
    {
        path_.replace_extension(ext);
        return *this;
    }

    DataTypePath& DataTypePath::clear()
    {
        path_.clear();
        return *this;
    }

    bool DataTypePath::empty() const
    {
        return path_.empty();
    }

    DataTypePath DataTypePath::find(const DataTypePath& dir, const std::string& filename, int max_iters)
    {
        if (!dir.is_directory())
            return {};

        auto fname = fs::path(filename).filename();

        for (auto it = fs::directory_iterator(dir.path_); it != fs::directory_iterator(); ++it) {
            if (max_iters-- == 0)
                break;

            if (!fs::is_regular_file(*it))
                continue;

            if (it->path().filename() == fname)
                return DataTypePath(it->path());
        }

        return {};
    }

}
}
