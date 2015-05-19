
extern int32_t start_log_thread();

extern int32_t stop_log_thread();

extern int32_t set_log_dir(const char *szLogDir);

extern void write_debug_log(const char *szLoggerName, const char *szFormat, ...);

