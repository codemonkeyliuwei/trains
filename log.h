#ifndef _TRAINS_LOG_H_
#define _TRAINS_LOG_H_

#include <stdio.h>

#define LogError(format, ...)   printf(format"\n", ##__VA_ARGS__)
#define LogWarn(format, ...)   printf(format"\n", ##__VA_ARGS__)
#define LogInfo(format, ...)   printf(format"\n", ##__VA_ARGS__)
#define LogDebug(format, ...)   printf(format"\n", ##__VA_ARGS__)

#endif