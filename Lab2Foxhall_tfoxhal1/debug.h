// Macs don't like the VAR_ARGS thing apparently
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

#define log_err(message, ...) fprintf(stderr, "ERROR[%s:%s:%d]: " message "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)
