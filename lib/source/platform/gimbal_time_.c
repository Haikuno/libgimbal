#include <gimbal/platform/gimbal_time_.h>

#ifdef GBL_NO_TIMESPEC_

#if defined(__MINGW32__)
#include <windows.h>
#endif

int timespec_get(struct timespec* pSpec, int base) {
    if(base != TIME_UTC)
        return 0;

#if defined(__MINGW32__)
    FILETIME fileTime;
    ULARGE_INTEGER ticks;
    const ULONGLONG windowsToUnixEpoch100ns = 116444736000000000ULL;

    GetSystemTimeAsFileTime(&fileTime);
    ticks.LowPart = fileTime.dwLowDateTime;
    ticks.HighPart = fileTime.dwHighDateTime;

    if(ticks.QuadPart < windowsToUnixEpoch100ns)
        return 0;

    ticks.QuadPart -= windowsToUnixEpoch100ns;
    pSpec->tv_sec = (time_t)(ticks.QuadPart / 10000000ULL);
    pSpec->tv_nsec = (long)((ticks.QuadPart % 10000000ULL) * 100ULL);
#else
    if(clock_gettime(CLOCK_REALTIME, pSpec) == -1)
        return 0;
#endif

    return TIME_UTC;
}

#endif
