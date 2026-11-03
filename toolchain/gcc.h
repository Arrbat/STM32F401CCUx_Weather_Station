#ifndef GCC_H
#define GCC_H

#if defined(__GNUC__)
#define WS_MUST_CHECK __attribute__((warn_unused_result))
#else
#define WS_MUST_CHECK
#endif

#endif /* GCC_H */
