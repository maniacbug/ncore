// Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!

#ifndef __RTC_native_H__
#define __RTC_native_H__

#include <RTClib.h>

// RTC based on the native chip connected via SPI and the SPI library
class RTC_native
{
public:
    RTC_native() {}
    uint8_t begin(void);
    void adjust(const DateTime& dt);
    uint8_t isrunning(void);
    DateTime now();
};

#endif // __RTC_native_H__

// vim:ai:cin:sw=4 sts=4 ft=cpp
