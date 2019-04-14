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
#ifndef SMC_H
#define SMC_H

#include <IOKit/IOTypes.h>

#include "ceammc_atomlist.h"
#include "fn_strings.h"

using namespace ceammc;

namespace apple_smc {

//#define OP_NONE 0
//#define OP_LIST 1
//#define OP_READ 2
//#define OP_READ_FAN 3
//#define OP_WRITE 4

enum {
    SMC_CMD_READ_BYTES = 5,
    SMC_CMD_WRITE_BYTES = 6,
    SMC_CMD_READ_INDEX = 8,
    SMC_CMD_READ_KEYINFO = 9,
    SMC_CMD_READ_PLIMIT = 11,
    SMC_CMD_READ_VERS = 12
};

#define DATATYPE_FPE2 "fpe2"
#define DATATYPE_UINT8 "ui8 "
#define DATATYPE_UINT16 "ui16"
#define DATATYPE_UINT32 "ui32"
#define DATATYPE_SP78 "sp78"

// key values
// lots more here:
//   http://discussions.apple.com/thread.jspa?threadID=734247&tstart=0
#define SMC_KEY_CPU_TEMP "TC0D"
#define SMC_KEY_GPU_TEMP "TG0P"
#define SMC_KEY_FAN0_RPM_MIN "F0Mn"
#define SMC_KEY_FAN1_RPM_MIN "F1Mn"
#define SMC_KEY_FAN0_RPM_CUR "F0Ac"
#define SMC_KEY_FAN1_RPM_CUR "F1Ac"

struct SMCKeyDataVersion {
    char major;
    char minor;
    char build;
    char reserved[1];
    UInt16 release;
};

typedef struct {
    UInt16 version;
    UInt16 length;
    UInt32 cpuPLimit;
    UInt32 gpuPLimit;
    UInt32 memPLimit;
} SMCKeyData_pLimitData_t;

struct SMCKeyDataKeyInfo {
    UInt32 dataSize;
    UInt32 dataType;
    char dataAttributes;
};

struct SMCKeyData {
    UInt32 key;
    SMCKeyDataVersion vers;
    SMCKeyData_pLimitData_t pLimitData;
    SMCKeyDataKeyInfo keyInfo;
    char result;
    char status;
    char data8; // command
    UInt32 data32;
    SMCBytes bytes;
};

struct SMCValue {
    UInt32CharKey key;
    UInt32 dataSize;
    UInt32CharKey dataType;
    SMCBytes bytes;
};

class SMC {
    io_connect_t io_connection_;

public:
    SMC();
    ~SMC();

    AtomList readKey(t_symbol* key) const;
    bool isConnected() const;
    AtomList keys() const;
    size_t keyCount() const;

    t_float cpuTemperature() const;
    t_float gpuTemperature() const;

private:
    bool readKey(const std::string& key, SMCValue& val) const;
    bool smcCall(int idx, const SMCKeyData* in_data, SMCKeyData* out_data) const;
};
}

#endif // SMC_H
