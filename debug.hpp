#ifndef GUARDDEBUG
#define GUARDDEBUG

#include<iostream>

#include "src/constants.hpp"
#include "src/termcolor/termcolor.hpp"

namespace SHCL = termcolor;

#define SH_ERR       (SHCL::red)
#define SH_OK         (SHCL::green)
#define SH_INFO     (SHCL::cyan)
#define SH_WARN  (SHCL::yellow)
#define SH_SPE       (SHCL::magenta)
#define SH_DEF      (SHCL::white)

#ifdef DEV_MODE
    #define DebugLog(color, msg)  std::cout << __FILE__ << ":" << __LINE__ << ": " << color << msg << SHCL::reset << std::endl
    #define DebugWLog(color, msg) std::wcout << __FILE__ << L":" << __LINE__ << L": " << color << msg << SHCL::reset << std::endl
#else
    #define DebugLog(color, msg) if (false) {}
#endif

#endif // GUARDDEBUG
