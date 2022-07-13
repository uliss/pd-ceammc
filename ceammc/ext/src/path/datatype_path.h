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
#ifndef DATATYPE_PATH_H
#define DATATYPE_PATH_H

#include "ceammc_abstractdata.h"
#include "ceammc_data.h"
#include "filesystem.hpp"

#include <memory>

namespace ceammc {
namespace path {

    using Path = ghc::filesystem::path;

    class DataTypePath : public AbstractData {
        Path path_;

    public:
        DataTypePath();
        ~DataTypePath();
        DataTypePath(DataTypePath&& path);
        DataTypePath(const DataTypePath& path);
        explicit DataTypePath(const char* path);
        explicit DataTypePath(const std::string& path);
        explicit DataTypePath(Path&& path);
        explicit DataTypePath(const Path& path);

        DataTypePath& operator=(const DataTypePath& path);
        DataTypePath& operator=(DataTypePath&& path);

        /**
         * Polymorphic copy
         */
        DataTypePath* clone() const override;

        /**
         * Polymorphic data type
         */
        DataTypeId type() const noexcept final;

        std::string toListStringContent() const final;
        std::string toDictStringContent() const final;
        bool set(const AbstractData* d) noexcept final;

        /**
         * Polymorphic string value
         */
        std::string toString() const override;

        std::string valueToJsonString() const override;

        /**
         * Polymorphics equality check
         */
        bool isEqual(const AbstractData* d) const noexcept final;

        /**
         * Equal check
         */
        bool operator==(const DataTypePath& path) const noexcept;

        bool is_absolute() const;
        bool is_relative() const;
        bool is_directory() const;
        bool is_regular_file() const;
        bool exists() const;

        std::string root_name() const;
        std::string root_directory() const;
        std::string root_path() const;
        std::string relative_path() const;
        DataTypePath parent_path() const;
        std::string filename() const;
        std::string extension() const;
        std::uintmax_t file_size() const;

        DataTypePath lexically_normal() const;

        DataTypePath& replace_extension(const std::string& ext = {});
        DataTypePath& clear();

        bool empty() const;

    public:
        static DataTypePath find(const DataTypePath& dir, const std::string& filename, int max_iters = -1);

    public:
        static const DataTypeId dataType;
    };

}

using PathAtom = DataAtom<path::DataTypePath>;

}

#endif // DATATYPE_PATH_H
