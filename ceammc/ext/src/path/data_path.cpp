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
#include "data_path.h"
#include "ceammc_factory.h"
#include "datatype_dict.h"
#include "fmt/core.h"

DataPath::DataPath(const PdArgs& args)
    : BaseTclObject<DataPathBase>(args, gensym("::ceammc::patheditor::open"))
{
    path_ = new PathProperty("@path", {});
    path_->setArgIndex(0);
    addProperty(path_);

    norm_ = new BoolProperty("@norm", true);
    addProperty(norm_);

    async_ = new BoolProperty("@async", true);
    addProperty(async_);

    createInlet();
    createOutlet();
}

void DataPath::initDone()
{
    DataPathBase::initDone();

    path_->setSuccessFn([this](Property*) {
        if (norm_->value())
            path_->value().normalize();
    });

    path_->callSuccessFn();
}

void DataPath::onBang()
{
    atomTo(0, path_->asDataAtom());
}

void DataPath::onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
    char buf[MAXPDSTRING];
    auto pbuf = fmt::format_to(buf, "data.path {}x{}+{}+{} {}",
        400, 200, (int)xpos, (int)ypos,
        path_->value().toListStringContent());
    *pbuf = '\0';
    tclCallRaw(buf);
}

void DataPath::appendRequest(DataPathOpCode op)
{
    if (async_->value()) {
        addRequest(DataPathRequest { path_->value(), op });
    } else {
        DataPathResult res;
        switch (op) {
        case DATA_PATH_OP_EXISTS:
            res = exists(path_->value());
            break;
        case DATA_PATH_OP_TYPE:
            res = filetype(path_->value());
            break;
        case DATA_PATH_OP_SIZE:
            res = filesize(path_->value());
            break;
        case DATA_PATH_OP_PERMISSIONS:
            res = permissions(path_->value());
            break;
        case DATA_PATH_OP_ALL_INFO:
            res = info(path_->value());
            break;
        default:
            return;
        }

        processResult(res);
    }
}

void DataPath::m_exists(t_symbol* s, const AtomListView& lv)
{
    appendRequest(DATA_PATH_OP_EXISTS);
}

void DataPath::m_filename(t_symbol* s, const AtomListView& lv)
{
    anyTo(0, gensym("filename"), PathAtom(path_->value().filename()));
}

void DataPath::m_filesize(t_symbol* s, const AtomListView& lv)
{
    appendRequest(DATA_PATH_OP_SIZE);
}

void DataPath::m_filetype(t_symbol* s, const AtomListView& lv)
{
    appendRequest(DATA_PATH_OP_TYPE);
}

void DataPath::m_info(t_symbol* s, const AtomListView& lv)
{
    appendRequest(DATA_PATH_OP_ALL_INFO);
}

void DataPath::m_permissions(t_symbol* s, const AtomListView& lv)
{
    appendRequest(DATA_PATH_OP_PERMISSIONS);
}

Either<DataPathResult> DataPath::processRequest(const DataPathRequest& req)
{
    switch (req.op_code) {
    case DATA_PATH_OP_EXISTS:
        return exists(path::DataTypePath(req.path));
    case DATA_PATH_OP_SIZE:
        return filesize(path::DataTypePath(req.path));
    case DATA_PATH_OP_TYPE:
        return filetype(path::DataTypePath(req.path));
    case DATA_PATH_OP_PERMISSIONS:
        return permissions(path::DataTypePath(req.path));
    case DATA_PATH_OP_ALL_INFO:
        return info(path::DataTypePath(req.path));
    default:
        return Either<DataPathResult>::makeError("unknown opcode");
    }
}

void DataPath::onTclResponse(t_symbol* s, const AtomListView& lv)
{
    OBJ_DBG << lv;
    path_->value() = path::DataTypePath(lv);
}

void DataPath::resultAllInfo(const DataPathResult& data)
{
    DictAtom a;
    a->insert("type", gensym(to_string(data.type)));
    a->insert("size", data.filesize);
    a->insert("permissions", data.permissions);
    anyTo(0, gensym("info"), a);
}

void DataPath::resultSize(const DataPathResult& data)
{
    switch (data.res_code) {
    case DATA_PATH_RC_OK:
        anyTo(0, gensym("size"), data.filesize);
        break;
    default:
        resultError(data);
        break;
    }
}

void DataPath::resultExists(const DataPathResult& data)
{
    anyTo(0, gensym("exists"), data.res_code == DATA_PATH_RC_OK);
}

void DataPath::resultPermissions(const DataPathResult& data)
{
    if (data.res_code == DATA_PATH_RC_OK)
        anyTo(0, gensym("permissions"), data.permissions);
    else
        resultError(data);
}

void DataPath::resultType(const DataPathResult& data)
{
    if (data.res_code == DATA_PATH_RC_OK)
        anyTo(0, gensym("type"), gensym(to_string(data.type)));
    else
        resultError(data);
}

void DataPath::resultError(const DataPathResult& data)
{
    switch (data.res_code) {
    case DATA_PATH_RC_OK:
        return;
    case DATA_PATH_RC_NOT_FOUND:
        OBJ_ERR << "file not found";
        return;
    case DATA_PATH_RC_SYSTEM_ERR:
        OBJ_ERR << "system error";
        return;
    default:
        OBJ_ERR << "unknown error";
    }
}

void DataPath::processResult(const DataPathResult& data)
{
    switch (data.op_code) {
    case DATA_PATH_OP_SIZE:
        resultSize(data);
        break;
    case DATA_PATH_OP_EXISTS:
        resultExists(data);
        break;
    case DATA_PATH_OP_TYPE:
        resultType(data);
        break;
    case DATA_PATH_OP_PERMISSIONS:
        resultPermissions(data);
        break;
    case DATA_PATH_OP_ALL_INFO:
        resultAllInfo(data);
        break;
    default:
        OBJ_ERR << fmt::format("not supported operation: {}", static_cast<int>(data.op_code));
        break;
    }
}

DataPathResult DataPath::exists(const path::DataTypePath& path)
{
    DataPathResult res;
    res.op_code = DATA_PATH_OP_EXISTS;
    res.res_code = path.exists() ? DATA_PATH_RC_OK : DATA_PATH_RC_NOT_FOUND;
    return res;
}

DataPathResult DataPath::filesize(const path::DataTypePath& path)
{
    DataPathResult res;
    res.op_code = DATA_PATH_OP_SIZE;
    res.res_code = path.exists() ? DATA_PATH_RC_OK : DATA_PATH_RC_NOT_FOUND;
    res.filesize = path.file_size();
    return res;
}

DataPathResult DataPath::filetype(const path::DataTypePath& path)
{
    DataPathResult res;
    res.op_code = DATA_PATH_OP_TYPE;
    res.res_code = path.exists() ? DATA_PATH_RC_OK : DATA_PATH_RC_NOT_FOUND;
    res.type = path.file_type();
    return res;
}

DataPathResult DataPath::permissions(const path::DataTypePath& path)
{
    DataPathResult res;
    res.op_code = DATA_PATH_OP_PERMISSIONS;
    res.res_code = path.exists() ? DATA_PATH_RC_OK : DATA_PATH_RC_NOT_FOUND;
    res.permissions = path.permissions();
    return res;
}

DataPathResult DataPath::info(const path::DataTypePath& path)
{
    DataPathResult res;
    res.op_code = DATA_PATH_OP_ALL_INFO;
    res.res_code = path.exists() ? DATA_PATH_RC_OK : DATA_PATH_RC_NOT_FOUND;
    res.permissions = path.permissions();
    res.type = path.file_type();
    res.filesize = path.file_size();
    return res;
}

void setup_data_path()
{
    ObjectFactory<DataPath> obj("data.path");
    obj.addMethod("exists", &DataPath::m_exists);
    obj.addMethod("size", &DataPath::m_filesize);
    obj.addMethod("type", &DataPath::m_filetype);
    obj.addMethod("permissions", &DataPath::m_permissions);
    obj.addMethod("info", &DataPath::m_info);

    obj.useClick();
    DataPath::initTclMethods(obj);

    LIB_LOG << fmt::format("Path datatype id: {:d}", path::DataTypePath::staticType());
}
