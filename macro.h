#ifndef _TRAINS_MACRO_H_
#define _TRAINS_MACRO_H_

#define CHECK_ARG_WITH_RET(arg, ret)    \
do {                                    \
    if (NULL == arg)                    \
    {                                   \
        LogError("argument is null");   \
        return ret;                     \
    }                                   \
}while(0)

#endif
