#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "%s:%d: ASSERTION FAILED: %s\n", \
                    __FILE__, __LINE__, msg); \
            abort(); \
        } \
    } while (0)

#endif /* __ASSERT_H__ */
