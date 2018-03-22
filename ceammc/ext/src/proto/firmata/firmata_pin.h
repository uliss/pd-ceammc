#ifndef FIRMATA_PIN_H
#define FIRMATA_PIN_H

#include <iostream>
#include <stdint.h>
#include <vector>

#include "firmata_proto.h"

namespace firmata {

typedef uint8_t PinN;
typedef uint8_t AnalogPinN;
typedef std::vector<FirmataPinCapability> PinCapabilityList;

class Pin {
    int n_;
    int analog_n_;
    FirmataPinMode mode_;
    PinCapabilityList caps_;
    int value_;
    bool pullup_;
    long state_;

public:
    Pin(PinN n = 0);

    /**
     * Returns pin number
     */
    PinN number() const;

    /**
     * Set pin number
     */
    void setNumber(PinN n);

    /**
     * Returns true if analog pin
     */
    bool isAnalog() const;
    /**
     * Returns pin analog number
     * @return -1 if not analog pin
     */
    int analogNumber() const;

    /**
     * Sets pin analog number
     */
    void setAnalogNumber(AnalogPinN n);

    /**
     * Returns pin capabilities: mode and resolution
     */
    PinCapabilityList& capabilities();
    const PinCapabilityList& capabilities() const;

    void addCapability(FirmataPinCapability c);

    /**
     * Return pin value
     */
    int value() const;

    /**
     * Sets pin value
     */
    void setValue(int v);

    /**
     * Returns pin mode
     */
    FirmataPinMode mode() const;

    /**
     * Sets pin mode
     * @param m - new pin mode
     * @return true on success, false is mode in not supported
     */
    bool setMode(FirmataPinMode m);

    /**
     * Checks if given mode is supported
     * @param m
     */
    bool isSupportedMode(FirmataPinMode m) const;

    /**
     * Returns pin bit resolution
     * @return 0 on error, otherwise value in range 1-N
     */
    int bitResolution() const;

    long state() const;
    void setState(long v);

    /**
     * status of the pull-up resistor for digital inputs
     * @return true if enabled, false if disabled.
     */
    bool pullUp() const;
    void setPullUp(bool v);
};

std::ostream& operator<<(std::ostream& os, const Pin& p);

} // namespace firmata

#endif // FIRMATA_PIN_H
