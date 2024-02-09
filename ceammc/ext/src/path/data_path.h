/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef DATA_PATH_H
#define DATA_PATH_H

#include "ceammc_object.h"
#include "ceammc_pollthread_spsc.h"
#include "ceammc_property_data.h"
#include "ceammc_tcl.h"
#include "datatype_path.h"

#include <cstdint>

using namespace ceammc;

enum DataPathResultCode : std::uint8_t {
    DATA_PATH_RC_OK,
    DATA_PATH_RC_EMPTY,
    DATA_PATH_RC_NOT_FOUND,
    DATA_PATH_RC_SYSTEM_ERR,
};

enum DataPathOpCode : std::uint8_t {
    DATA_PATH_OP_SIZE,
    DATA_PATH_OP_EXISTS,
    DATA_PATH_OP_TYPE,
    DATA_PATH_OP_PERMISSIONS,
    DATA_PATH_OP_ALL_INFO,
};

struct DataPathResult {
    std::uint64_t filesize;
    std::uint16_t permissions;
    path::FileType type;
    DataPathOpCode op_code;
    DataPathResultCode res_code;
};

struct DataPathRequest {
    path::DataTypePath path;
    DataPathOpCode op_code;
};

using DataPathBase = FixedSPSCObject<DataPathRequest, DataPathResult>;
using PathProperty = DataPropertyT<path::DataTypePath>;

class DataPath : public BaseTclObject<DataPathBase> {
    PathProperty* path_ { nullptr };
    BoolProperty* norm_ { nullptr };
    BoolProperty* async_ { nullptr };

public:
    DataPath(const PdArgs& args);
    void initDone() final;

    void onBang() final;
    void onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt) final;

    void m_exists(t_symbol* s, const AtomListView& lv);
    void m_extension(t_symbol* s, const AtomListView& lv);
    void m_filename(t_symbol* s, const AtomListView& lv);
    void m_filesize(t_symbol* s, const AtomListView& lv);
    void m_filetype(t_symbol* s, const AtomListView& lv);
    void m_info(t_symbol* s, const AtomListView& lv);
    void m_parent_path(t_symbol* s, const AtomListView& lv);
    void m_permissions(t_symbol* s, const AtomListView& lv);

    void processResult(const DataPathResult& data) final;
    Either<DataPathResult> processRequest(const DataPathRequest& req) final;

    void onTclResponse(t_symbol* s, const AtomListView& lv) final;

private:
    void resultAllInfo(const DataPathResult& data);
    void resultError(const DataPathResult& data);
    void resultExists(const DataPathResult& data);
    void resultPermissions(const DataPathResult& data);
    void resultSize(const DataPathResult& data);
    void resultType(const DataPathResult& data);

    void appendRequest(DataPathOpCode op);

private:
    static DataPathResult exists(const path::DataTypePath& path);
    static DataPathResult filesize(const path::DataTypePath& path);
    static DataPathResult filetype(const path::DataTypePath& path);
    static DataPathResult permissions(const path::DataTypePath& path);
    static DataPathResult info(const path::DataTypePath& path);
};

void setup_data_path();

#endif // DATA_PATH_H
