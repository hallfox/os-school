#ifndef _DBG_H
#define _DBG_H

#ifdef NDEBUG
#define log_dbg(message, ...)
#else
#define log_dbg(message, ...) fprintf(stderr, "DEBUG[%s:%s:%d]: " message "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#endif

#define log_err(message, ...) fprintf(stderr, "ERROR[%s:%s:%d]: " message "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#endif
