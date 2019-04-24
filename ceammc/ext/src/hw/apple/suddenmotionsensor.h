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
#ifndef SUDDENMOTIONSENSOR_H
#define SUDDENMOTIONSENSOR_H

#include <IOKit/IOTypes.h>
#include <array>
#include <string>
#include <tuple>

namespace apple_sms {

typedef std::tuple<float, float, float> AccelData;

// Represents a single axis of a type of sensor
struct AxisStruct {
    bool enabled; // if axis is valid in this sensor
    int index; // Location in struct of first byte
    int nbytes;
    float zero_gravity;
    float one_gravity; // Change in value meaning "increase of one g"
        // (can be negative if axis sensor reversed)
};

// Represents the configuration of a type of sensor
struct SensorSpec {
    unsigned int kernel_fn; // Kernel function index
    int record_size; // Size of record to be sent/received
    AxisStruct axes[3]; // Description of three axes (X, Y, Z)
};

class SuddenMotionSensor {
    // model name, like MacBookPro8,1
    std::string model_name_;
    io_connect_t connection_;
    SensorSpec spec_;
    typedef std::array<char, 60> Record;
    Record data_in_;
    Record data_out_;

public:
    SuddenMotionSensor();
    ~SuddenMotionSensor();

    const std::string& modelName() const { return model_name_; }

    AccelData getRawAccel() const;
    AccelData getNormalizedAccel() const;
    bool isConnected() const;

private:
    void init();
    bool connectTo(const std::string& serviceName);
    static std::string getModelName();
};

}

#endif // SUDDENMOTIONSENSOR_H
