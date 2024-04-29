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
#include "ceammc_datastorage.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "ceammc_string.h"
#include "fmt/core.h"

#include "filesystem.hpp"

constexpr const char* TYPE_NAME = "Path";

namespace ceammc {
namespace path {

    namespace fs = ghc::filesystem;

    const char* to_string(FileType t)
    {
        switch (t) {
        case FILE_TYPE_REGULAR:
            return "regular";
        case FILE_TYPE_DIRECTORY:
            return "dir";
        case FILE_TYPE_SYMLINK:
            return "symlink";
        case FILE_TYPE_BLOCK:
            return "block";
        case FILE_TYPE_CHARACTER:
            return "char";
        case FILE_TYPE_FIFO:
            return "fifo";
        case FILE_TYPE_SOCKET:
            return "socket";
        case FILE_TYPE_IS_UNKNOWN:
            return "?";
        default:
            return "";
        }
    }

    DataTypeId DataTypePath::staticType()
    {
        CEAMMC_REGISTER_DATATYPE(TYPE_NAME, [](const AtomListView& args) -> Atom { return new path::DataTypePath(args); }, {});
    }

    DataTypePath::DataTypePath()
    {
    }

    // for std::unique_ptr
    DataTypePath::~DataTypePath() = default;

    DataTypePath::DataTypePath(DataTypePath&& path)
        : path_(std::move(path.path_))
    {
    }

    DataTypePath::DataTypePath(const DataTypePath& path)
    {
        if (path.path_)
            path_.reset(new Path(*path.path_));
    }

    DataTypePath::DataTypePath(const char* path)
        : path_(new Path(path))
    {
    }

    DataTypePath::DataTypePath(const std::string& path)
        : path_(new Path(path))
    {
    }

    DataTypePath::DataTypePath(PathPtr&& path)
        : path_(std::move(path))
    {
    }

    DataTypePath::DataTypePath(const Path& path)
        : path_(new Path(path))
    {
    }

    DataTypePath::DataTypePath(const AtomListView& args)
        : DataTypePath(to_string(args))
    {
    }

    DataTypePath& DataTypePath::operator=(const DataTypePath& path)
    {
        if (this == &path)
            return *this;

        if (path.path_)
            path_.reset(new Path(*path.path_));

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
        return staticType();
    }

    std::string DataTypePath::toListStringContent() const
    {
        return path_ ? path_->generic_string() : "";
    }

    std::string DataTypePath::toDictStringContent() const
    {
        return "";
    }

    bool DataTypePath::set(const AbstractData* d) noexcept
    {
        if (d->type() != DataTypePath::staticType())
            return false;

        *this = *d->as<DataTypePath>();
        return true;
    }

    std::string DataTypePath::toJsonString() const
    {
        return fmt::format("\"{}\"", string::escape_for_json(path_->string()));
    }

    bool DataTypePath::isEqual(const AbstractData* d) const noexcept
    {
        auto path = d->as<DataTypePath>();

        // not a path
        if (!path)
            return false;

        return this->operator==(*path);
    }

    bool DataTypePath::operator==(const DataTypePath& path) const noexcept
    {
        // self check
        if (this == &path || path_ == path.path_)
            return true;
        else if (path_ && path.path_)
            return *path_ == *path.path_;
        else
            return false;
    }

    bool DataTypePath::is_absolute() const
    {
        return path_ && path_->is_absolute();
    }

    bool DataTypePath::is_relative() const
    {
        return path_ && path_->is_relative();
    }

    bool DataTypePath::is_directory() const noexcept
    {
        std::error_code ec;
        return path_ && fs::is_directory(*path_, ec);
    }

    bool DataTypePath::is_regular_file() const noexcept
    {
        std::error_code ec;
        return path_ && fs::is_regular_file(*path_, ec);
    }

    bool DataTypePath::exists() const noexcept
    {
        std::error_code ec;
        return path_ && fs::exists(*path_, ec);
    }

    std::string DataTypePath::root_name() const
    {
        return path_ ? path_->root_name().string() : "";
    }

    std::string DataTypePath::root_directory() const
    {
        return path_ ? path_->root_directory().string() : "";
    }

    std::string DataTypePath::root_path() const
    {
        return path_ ? path_->root_path().string() : "";
    }

    std::string DataTypePath::relative_path() const
    {
        return path_ ? path_->relative_path().string() : "";
    }

    DataTypePath DataTypePath::parent_path() const
    {
        return path_ ? DataTypePath(path_->parent_path()) : DataTypePath();
    }

    std::string DataTypePath::filename() const
    {
        return path_ ? path_->filename().string() : "";
    }

    std::string DataTypePath::extension() const
    {
        return path_ ? path_->extension().string() : "";
    }

    std::uintmax_t DataTypePath::file_size() const noexcept
    {
        std::error_code ec;
        return path_ ? fs::file_size(*path_, ec) : 0;
    }

    FileType DataTypePath::file_type() const noexcept
    {
        if (!path_)
            return FILE_TYPE_NONE;

        std::error_code ec;
        switch (fs::status(*path_, ec).type()) {
        case fs::file_type::none:
            return FILE_TYPE_NONE;
        case fs::file_type::not_found:
            return FILE_TYPE_NOT_FOUND;
        case fs::file_type::regular:
            return FILE_TYPE_REGULAR;
        case fs::file_type::directory:
            return FILE_TYPE_DIRECTORY;
        case fs::file_type::symlink:
            return FILE_TYPE_SYMLINK;
        case fs::file_type::block:
            return FILE_TYPE_BLOCK;
        case fs::file_type::character:
            return FILE_TYPE_CHARACTER;
        case fs::file_type::fifo:
            return FILE_TYPE_FIFO;
        case fs::file_type::socket:
            return FILE_TYPE_SOCKET;
        default:
            return FILE_TYPE_IS_UNKNOWN;
        }
    }

    std::uint16_t DataTypePath::permissions() const noexcept
    {
        std::error_code ec;
        return path_ ? static_cast<std::uint16_t>(fs::status(*path_, ec).permissions()) : 0;
    }

    DataTypePath DataTypePath::lexically_normal() const
    {
        return path_ ? DataTypePath(path_->lexically_normal()) : DataTypePath();
    }

    DataTypePath& DataTypePath::replace_extension(const std::string& ext)
    {
        if (path_)
            path_->replace_extension(ext);

        return *this;
    }

    DataTypePath& DataTypePath::clear() noexcept
    {
        if (path_)
            path_->clear();

        return *this;
    }

    DataTypePath& DataTypePath::normalize()
    {
        if (path_)
            path_.reset(new Path(path_->lexically_normal()));

        return *this;
    }

    bool DataTypePath::empty() const noexcept
    {
        return path_ ? path_->empty() : true;
    }

    DataTypePath DataTypePath::find(const DataTypePath& dir, const std::string& filename, int max_iters)
    {
        if (!dir.is_directory() || !dir.path_)
            return {};

        auto fname = fs::path(filename).filename();

        for (auto it = fs::directory_iterator(*dir.path_); it != fs::directory_iterator(); ++it) {
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
