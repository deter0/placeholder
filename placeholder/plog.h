#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <placeholder/ff.h>

#define STRINGIZING(x) #x
#define STR(x) STRINGIZING(x)
#define FILE_LINE __FILE__ ":" STR(__LINE__)

// Terminal
private const char *t_RESET     = "\033[0m";
private const char *t_UNDERLINE = "\033[4m";
private const char *t_BLACK     = "\033[30;1m";
private const char *t_RED       = "\033[31;1m";
private const char *t_GREEN     = "\033[32;1m";
private const char *t_YELLOW    = "\033[33;1m";
private const char *t_BLUE      = "\033[34;1m";
private const char *t_MAGENTA   = "\033[35;1m";
private const char *t_CYAN      = "\033[36;1m";
private const char *t_WHITE     = "\033[37;1m";

private const char *t_BR_BLACK   = "\033[30;1m";
private const char *t_BR_RED     = "\033[31;1m";
private const char *t_BR_GREEN   = "\033[32;1m";
private const char *t_BR_YELLOW  = "\033[33;1m";
private const char *t_BR_BLUE    = "\033[34;1m";
private const char *t_BR_MAGENTA = "\033[35;1m";
private const char *t_BR_CYAN    = "\033[36;1m";
private const char *t_BR_WHITE   = "\033[37;1m";

void p_f_debugLog(const char *, const char *, ...);
void p_f_log(const char *, const char *, ...);
void p_f_warn(const char *, const char *, ...);
void p_f_error(const char *, const char *, ...);

#define p_debugLog(fmt, ...) p_f_debugLog(FILE_LINE, fmt, __VA_ARGS__)
#define p_log(fmt, ...)      p_f_log(FILE_LINE, fmt, __VA_ARGS__)
#define p_warn(fmt, ...)     p_f_warn(FILE_LINE, fmt, __VA_ARGS__)
#define p_error(fmt, ...)    p_f_error(FILE_LINE, fmt, __VA_ARGS__)
