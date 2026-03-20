use num_enum::{IntoPrimitive, TryFromPrimitive};

use crate::Channel;
#[repr(u8)]
#[derive(Debug, Copy, Clone, Eq, PartialEq, IntoPrimitive, TryFromPrimitive, PartialOrd, Ord)]
pub enum Register {
    TouchStatus0_7 = 0x00,
    TouchStatus8_11 = 0x01,
    OORStatus = 0x02,
    EleproxOORStatus = 0x03,
    FiltData0MSB = 0x04,
    FiltData0LSB = 0x05,
    FiltData1MSB = 0x06,
    FiltData1LSB = 0x07,
    FiltData2MSB = 0x08,
    FiltData2LSB = 0x09,
    FiltData3MSB = 0x0A,
    FiltData3LSB = 0x0B,
    FiltData4MSB = 0x0C,
    FiltData4LSB = 0x0D,
    FiltData5MSB = 0x0E,
    FiltData5LSB = 0x0F,
    FiltData6MSB = 0x10,
    FiltData6LSB = 0x11,
    FiltData7MSB = 0x12,
    FiltData7LSB = 0x13,
    FiltData8MSB = 0x14,
    FiltData8LSB = 0x15,
    FiltData9MSB = 0x16,
    FiltData9LSB = 0x17,
    FiltData10MSB = 0x18,
    FiltData10LSB = 0x19,
    FiltData11MSB = 0x1A,
    FiltData11LSB = 0x1B,
    // EleproxFiltDataMSB = 0x1C,
    // EleproxFiltDataLSB = 0x1D,
    BaseLine0 = 0x1E,
    BaseLine1 = 0x1F,
    BaseLine2 = 0x20,
    BaseLine3 = 0x21,
    BaseLine4 = 0x22,
    BaseLine5 = 0x23,
    BaseLine6 = 0x24,
    BaseLine7 = 0x25,
    BaseLine8 = 0x26,
    BaseLine9 = 0x27,
    BaseLine10 = 0x28,
    BaseLine11 = 0x29,
    EleproxBaseLine = 0x2A,
    MaximumHalfDeltaRising = 0x2B,
    NoiseHalfDataRising = 0x2C,
    NoiseCountLimitRising = 0x2D,
    FilterDelayCountLimitRising = 0x2E,
    MaximmHalfDeltaFalling = 0x2F,
    NoiseHalfDeltaFalling = 0x30,
    NoiseCountLimitFalling = 0x31,
    FilterDelayCountFalling = 0x32,
    NoiseHalfDeltaTouched = 0x33,
    NoiseCountLimitTouched = 0x34,
    FilterDelayCountLimitTouched = 0x35,
    TouchThreshold0 = 0x41,
    ReleaseThreshold0 = 0x42,
    TouchThreshold1 = 0x43,
    ReleaseThreshold1 = 0x44,
    TouchThreshold2 = 0x45,
    ReleaseThreshold2 = 0x46,
    TouchThreshold3 = 0x47,
    ReleaseThreshold3 = 0x48,
    TouchThreshold4 = 0x49,
    ReleaseThreshold4 = 0x4A,
    TouchThreshold5 = 0x4B,
    ReleaseThreshold5 = 0x4C,
    TouchThreshold6 = 0x4D,
    ReleaseThreshold6 = 0x4E,
    TouchThreshold7 = 0x4F,
    ReleaseThreshold7 = 0x50,
    TouchThreshold8 = 0x51,
    ReleaseThreshold8 = 0x52,
    TouchThreshold9 = 0x53,
    ReleaseThreshold9 = 0x54,
    TouchThreshold10 = 0x55,
    ReleaseThreshold10 = 0x56,
    TouchThreshold11 = 0x57,
    ReleaseThreshold11 = 0x58,
    Debounce = 0x5B,
    GlobalChargeDischargeCurrentConfig = 0x5C,
    GlobalChargeDischargeTimeConfig = 0x5D,
    ChargeCurr0 = 0x5F,
    ChargeTime1 = 0x6C,
    Ecr = 0x5E,
    AutoConfig0 = 0x7B,
    AutoConfig1 = 0x7C,
    UpSideLimit = 0x7D,
    LowSideLimit = 0x7E,
    TargetLevel = 0x7F,
    SoftReset = 0x80,

    // GPIO Registers (0x73-0x7A)
    GpioControl0 = 0x73,
    GpioControl1 = 0x74,
    GpioData = 0x75,
    GpioDirection = 0x76,
    GpioEnable = 0x77,
    GpioDataSet = 0x78,
    GpioDataClear = 0x79,
    GpioDataToggle = 0x7A,
}

impl Register {
    /// Returns the threshold register associated with the channel
    pub fn get_threshold_register(channel: Channel) -> Register {
        match channel {
            Channel::Zero => Register::TouchThreshold0,
            Channel::One => Register::TouchThreshold1,
            Channel::Two => Register::TouchThreshold2,
            Channel::Three => Register::TouchThreshold3,
            Channel::Four => Register::TouchThreshold4,
            Channel::Five => Register::TouchThreshold5,
            Channel::Six => Register::TouchThreshold6,
            Channel::Seven => Register::TouchThreshold7,
            Channel::Eight => Register::TouchThreshold8,
            Channel::Nine => Register::TouchThreshold9,
            Channel::Ten => Register::TouchThreshold10,
            Channel::Eleven => Register::TouchThreshold11,
        }
    }
    /// Returns the release register associated with the channel
    pub fn get_release_register(channel: Channel) -> Register {
        match channel {
            Channel::Zero => Register::ReleaseThreshold0,
            Channel::One => Register::ReleaseThreshold1,
            Channel::Two => Register::ReleaseThreshold2,
            Channel::Three => Register::ReleaseThreshold3,
            Channel::Four => Register::ReleaseThreshold4,
            Channel::Five => Register::ReleaseThreshold5,
            Channel::Six => Register::ReleaseThreshold6,
            Channel::Seven => Register::ReleaseThreshold7,
            Channel::Eight => Register::ReleaseThreshold8,
            Channel::Nine => Register::ReleaseThreshold9,
            Channel::Ten => Register::ReleaseThreshold10,
            Channel::Eleven => Register::ReleaseThreshold11,
        }
    }

    /// Returns the Most Significant Byte [MSB] register associated with the channel
    pub fn get_filtered_data_msb(channel: Channel) -> Register {
        match channel {
            Channel::Zero => Register::FiltData0MSB,
            Channel::One => Register::FiltData1MSB,
            Channel::Two => Register::FiltData2MSB,
            Channel::Three => Register::FiltData3MSB,
            Channel::Four => Register::FiltData4MSB,
            Channel::Five => Register::FiltData5MSB,
            Channel::Six => Register::FiltData6MSB,
            Channel::Seven => Register::FiltData7MSB,
            Channel::Eight => Register::FiltData8MSB,
            Channel::Nine => Register::FiltData9MSB,
            Channel::Ten => Register::FiltData10MSB,
            Channel::Eleven => Register::FiltData11MSB,
        }
    }

    /// Returns the baseline register associated with the channel
    pub fn get_baseline(channel: Channel) -> Register {
        match channel {
            Channel::Zero => Register::BaseLine0,
            Channel::One => Register::BaseLine1,
            Channel::Two => Register::BaseLine2,
            Channel::Three => Register::BaseLine3,
            Channel::Four => Register::BaseLine4,
            Channel::Five => Register::BaseLine5,
            Channel::Six => Register::BaseLine6,
            Channel::Seven => Register::BaseLine7,
            Channel::Eight => Register::BaseLine8,
            Channel::Nine => Register::BaseLine9,
            Channel::Ten => Register::BaseLine10,
            Channel::Eleven => Register::BaseLine11,
        }
    }

    /// Some registers require for the sensor to be in stop mode before they can be accessed
    pub fn require_stop(&self) -> bool {
        !matches!(
            self,
            // These Registers require you to put the device in STOP mode in order to read/write too
            Self::Ecr
                | Self::GpioControl0
                | Self::GpioControl1
                | Self::GpioData
                | Self::GpioDirection
                | Self::GpioEnable
                | Self::GpioDataSet
                | Self::GpioDataClear
                | Self::GpioDataToggle
        )
    }

    /// Returns the default value of the Register
    pub fn get_initial_value(&self) -> u8 {
        match self {
            Self::GlobalChargeDischargeCurrentConfig => 0x10,
            Self::GlobalChargeDischargeTimeConfig => 0x24,
            _ => 0x00,
        }
    }
}

/// Limit Values for the Up and Low Side boundary checking on the electroludes. See Datasheet Page 19 for more details
pub mod limits {
    const VDD_VALUE: f32 = 3.3;
    pub const UP_SIDE: u8 = (((VDD_VALUE - 0.7) / VDD_VALUE) * 256.0) as u8;
    pub const LOW_SIDE: u8 = ((UP_SIDE as f32) * 0.65) as u8;
    pub const TARGET_LEVEL: u8 = ((UP_SIDE as f32) * 0.9) as u8;
}
