/**
 * ManagedPrintf::printf(const char *format, ...) returns a ManagedString
 * !! NOTE: DO NOT use double formats  %f %F %e %E %g %G %a %A 
 * The underlying mbed compiler does not support them
 * 
 * @param format,  printf format 
 * @param ... ,  extra args for printf
 * @return ManagedString
 * 
 * Example:
 *    ManagedString mStr = ManagedPrintf::printf("loop count:%lu\n", counter);
 *    uBit.serial.send(mStr);
 * OR
 *    uBit.serial.send(ManagedPrintf::printf("loop count:%lu\n", counter));
 * 
 * (c)2016 Forward Computing and Control Pty. Ltd.
 * This code may be freely used for both private and commercial use.
 * Provide this copyright is maintained.
 */

#include "ManagedPrintf.h"

ManagedString ManagedPrintf::printf(const char *format, ...) {
    ManagedString rtn;
    std::va_list arg;
    va_start(arg, format);
    // find how long the temp_str needs to be
    int len = vsnprintf(NULL, 0, format, arg);
    if (len < MANAGED_PRINT_MAX_ON_STACK) {
        char temp_str[MANAGED_PRINT_MAX_ON_STACK];
        vsprintf(temp_str, format, arg);
        rtn = ManagedString(temp_str);
    } else {
        char *temp_str = new char[len + 1];
        vsprintf(temp_str, format, arg);
        rtn = ManagedString(temp_str);
        delete[] temp_str;
    }
    va_end(arg);
    return rtn;
}

ManagedString ManagedPrintf::getAddress(void *obj) {
    if (obj == NULL) {
        return ManagedString("NULL");
    } else {
        return printf("%p", obj);
    }
}