#ifndef __dbh_h__
#define __dbh_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define debug(...)
#else
#define debug(...) fprintf(stderr, "[DEBUG] "); fprintf(stderr, "at %s:%d ", __FILE__, __LINE__); fprintf(stderr, "function: %s ", __func__); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(...) fprintf(stderr, "[ERROR] "); fprintf(stderr, "at %s:%d ", __FILE__, __LINE__); fprintf(stderr, "function: %s ", __func__); fprintf(stderr, "errno: %s ", clean_errno()); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");

#define log_warn(...) fprintf(stderr, "[WARN] "); fprintf(stderr, "at %s:%d ", __FILE__, __LINE__); fprintf(stderr, "function: %s ", __func__); fprintf(stderr, "errno %s ", clean_errno()); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");

#define log_info(...) fprintf(stderr, "[INFO] "); fprintf(stderr, "at %s:%d ", __FILE__, __LINE__); fprintf(stderr, "function: %s ", __func__); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");

#define check(A, ...) if(!(A)) { log_err(__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory%s", "!")

#define check_debug(A, ...) if(!(A)) { debug(__VA_ARGS__); errno=0; goto error; }

#define sentinel(...)  { log_err(__VA_ARGS__); errno=0; goto error; }

#endif
