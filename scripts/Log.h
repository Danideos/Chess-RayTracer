#ifndef MAIN_CPP_LOG_H
#define MAIN_CPP_LOG_H

#include <stdio.h>

#define __DEBUG__
#ifdef __DEBUG__
#define Log(...) printf(__VA_ARGS__); printf("\n");
#else
#define Log(...) ;
#endif

#endif
