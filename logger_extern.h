
extern int32_t start_log_thread();

extern int32_t stop_log_thread();

extern int32_t set_log_dir(const char *szLogDir);

extern void write_debug_log(const char *szFileName, const char *nLineNo, const char *szLoggerName, const char *szFormat, ...);

#define WRITE_DEBUG_LOG(LOGGERNAME, FORMAT, ...)		write_debug_log(__FILE__, __LINE__, LOGGERNAME, FORMAT,  ##__VA_ARGS__)
