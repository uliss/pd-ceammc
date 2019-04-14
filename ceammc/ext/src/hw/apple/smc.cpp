/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "smc.h"
#include "ceammc_log.h"

#include <IOKit/IOKitLib.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace ceammc;

namespace apple_smc {

static void _ultostr(char* str, UInt32 val)
{
    str[0] = '\0';
    sprintf(str, "%c%c%c%c",
        (unsigned int)val >> 24,
        (unsigned int)val >> 16,
        (unsigned int)val >> 8,
        (unsigned int)val);
}

static UInt32 _strtoul(const char* str, int size, int base)
{
    UInt32 total = 0;

    for (int i = 0; i < size; i++) {
        if (base == 16)
            total += str[i] << (size - 1 - i) * 8;
        else
            total += (unsigned char)(str[i] << (size - 1 - i) * 8);
    }
    return total;
}

static float _strtof(const char* str, int size, int e)
{
    float total = 0;

    for (int i = 0; i < size; i++) {
        if (i == (size - 1))
            total += (str[i] & 0xff) >> e;
        else
            total += str[i] << (size - 1 - i) * (8 - e);
    }

    return total;
}

SMC::SMC()
    : io_connection_(0)
{
    CFMutableDictionaryRef dict = IOServiceMatching("AppleSMC");
    if (!dict) {
        LIB_LOG << "can't create matching dict for AppleSMC";
        return;
    }

    io_service_t io_service = IOServiceGetMatchingService(kIOMasterPortDefault, dict);
    if (!io_service) {
        LIB_LOG << "AppleSMC is not found";
        return;
    } else {
        LIB_LOG << "AppleSMC is found";
    }

    kern_return_t kernResult = IOServiceOpen(io_service, mach_task_self(), 0, &io_connection_);
    IOObjectRelease(io_service);

    if (kernResult != KERN_SUCCESS)
        LIB_LOG << "can't open AppleSMC service, error code is: " << kernResult;
}

SMC::~SMC()
{
    if (io_connection_)
        IOObjectRelease(io_connection_);
}

AtomList SMC::readKey(t_symbol* key) const
{
    LIB_LOG << "read key: " << key;

    SMCValue val;
    if (!readKey(key->s_name, val)) {
        LIB_LOG << "error while key read: " << key;
        return AtomList();
    }

    if (val.dataSize < 1) {
        LIB_LOG << "invalid key size: " << val.dataSize;
        return AtomList();
    }

    if (strcmp(val.dataType, DATATYPE_SP78) == 0) {
        int intValue = (val.bytes[0] * 256 + val.bytes[1]) >> 2;
        return AtomList(intValue / 64.f);
    } else if ((strcmp(val.dataType, DATATYPE_UINT8) == 0)
        || (strcmp(val.dataType, DATATYPE_UINT16) == 0)
        || (strcmp(val.dataType, DATATYPE_UINT32) == 0)) {
        LIB_LOG << val.dataSize << ": " << _strtoul(val.bytes, val.dataSize, 10);
        //        LIB_LOG << std::string(val.bytes, val.dataSize);
        return AtomList(_strtoul(val.bytes, val.dataSize, 10));
    } else if (strcmp(val.dataType, DATATYPE_FPE2) == 0) {
        return AtomList(_strtof(val.bytes, val.dataSize, 2));
    } else {
        AtomList res;
        for (UInt32 i = 0; i < val.dataSize; i++)
            res.append(t_float(val.bytes[i]));

        return res;
    }
}

bool SMC::isConnected() const
{
    return io_connection_ != 0;
}

AtomList SMC::keys() const
{
    AtomList res;
    //    kern_return_t result;
    SMCKeyData in_data;
    SMCKeyData out_data;

    UInt32Char_t key;
    SMCValue val;

    const auto N = keyCount();
    for (size_t i = 0; i < N; i++) {
        memset(&in_data, 0, sizeof(SMCKeyData));
        memset(&out_data, 0, sizeof(SMCKeyData));
        memset(&val, 0, sizeof(SMCValue));

        in_data.data8 = SMC_CMD_READ_INDEX;
        in_data.data32 = i;

        if (!smcCall(KERNEL_INDEX_SMC, &in_data, &out_data))
            continue;

        _ultostr(key, out_data.key);
        res.append(gensym(key));
    }

    return res;
}

size_t SMC::keyCount() const
{
    SMCValue val;
    readKey("#KEY", val);
    return _strtoul(val.bytes, val.dataSize, 16);
}

bool SMC::readKey(const std::string& key, SMCValue& val) const
{
    SMCKeyData in_data;
    SMCKeyData out_data;

    memset(&in_data, 0, sizeof(SMCKeyData));
    memset(&out_data, 0, sizeof(SMCKeyData));
    memset(&val, 0, sizeof(SMCValue));

    in_data.key = _strtoul(key.c_str(), 4, 16);
    in_data.data8 = SMC_CMD_READ_KEYINFO;

    if (!smcCall(KERNEL_INDEX_SMC, &in_data, &out_data)) {
        LIB_LOG << "can't read key info: " << key;
        return false;
    }

    LIB_LOG << "data type: " << out_data.keyInfo.dataType;

    _ultostr(val.dataType, out_data.keyInfo.dataType);
    val.dataSize = out_data.keyInfo.dataSize;
    in_data.keyInfo.dataSize = val.dataSize;
    in_data.data8 = SMC_CMD_READ_BYTES;

    if (!smcCall(KERNEL_INDEX_SMC, &in_data, &out_data)) {
        LIB_LOG << "can't read bytes: " << key;
        return false;
    }

    memcpy(val.bytes, out_data.bytes, sizeof(out_data.bytes));
    return true;
}

bool SMC::smcCall(int idx, const SMCKeyData* in_data, SMCKeyData* out_data) const
{
    size_t in_size = sizeof(SMCKeyData);
    size_t out_size = sizeof(SMCKeyData);

    kern_return_t res = IOConnectCallStructMethod(
        io_connection_, // an io_connect_t returned from IOServiceOpen().
        idx, // selector of the function to be called via the user client.
        in_data, // pointer to the input struct parameter.
        in_size, // the size of the input structure parameter.
        out_data, // pointer to the output struct parameter.
        &out_size // pointer to the size of the output structure parameter.
    );

    if (res != kIOReturnSuccess) {
        return false;
    } else {
        return true;
    }
}
}
