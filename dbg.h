#include <stdio.h>

#define ERR(fmt, args...) fprintf(stderr, fmt, ## args)
#define DBG2(fmt, args...) fprintf(stderr, "%s:%-5d: " fmt, __FUNCTION__, __LINE__, ## args)
#define ENTER2          do { fprintf(stderr, "%s:%-5d: ENTER\n", __FUNCTION__, __LINE__); } while(0)
#define RET2(args...)   do { fprintf(stderr, "%s:%-5d: RETURN\n", __FUNCTION__, __LINE__);\
return args; } while(0)
#define DBG
#ifdef DBG

#define ENTER          do { fprintf(stderr, "%s:%-5d: ENTER\n", __FUNCTION__, __LINE__); } while(0)
#define RET(args...)   do { fprintf(stderr, "%s:%-5d: RETURN\n", __FUNCTION__, __LINE__);\
return args; } while(0)
#define DBG(fmt, args...) fprintf(stderr, "%s:%-5d: " fmt, __FUNCTION__, __LINE__, ## args)

#else


#define ENTER         do {  } while(0)
#define RET(args...)   return args; 
#define DBG(fmt, args...)   do {  } while(0)
 
#endif

