#ifndef RANG_DOT_HPP
#define RANG_DOT_HPP

#if defined(__unix__) || defined(__unix) || defined(__linux__)
#define OS_LINUX
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#define OS_WIN
#elif defined(__APPLE__) || defined(__MACH__)
#define OS_MAC
#else
#error Unknown Platform
#endif

#if defined(OS_LINUX) || defined(OS_MAC)
#include <unistd.h>
#include <cstring>
#elif defined(OS_WIN)
#include <windows.h>
#include <io.h>
#include <VersionHelpers.h>
#endif

#include <algorithm>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <iterator>
#include <type_traits>

namespace rang {

enum class style {
	reset     = 0,
	bold      = 1,
	dim       = 2,
	italic    = 3,
	underline = 4,
	blink     = 5,
	rblink    = 6,
	reversed  = 7,
	conceal   = 8,
	crossed   = 9
};

enum class fg {
	black   = 30,
	red     = 31,
	green   = 32,
	yellow  = 33,
	blue    = 34,
	magenta = 35,
	cyan    = 36,
	gray    = 37,
	reset   = 39
};

enum class bg {
	black   = 40,
	red     = 41,
	green   = 42,
	yellow  = 43,
	blue    = 44,
	magenta = 45,
	cyan    = 46,
	gray    = 47,
	reset   = 49
};

enum class fgB {
	black   = 90,
	red     = 91,
	green   = 92,
	yellow  = 93,
	blue    = 94,
	magenta = 95,
	cyan    = 96,
	gray    = 97
};

enum class bgB {
	black   = 100,
	red     = 101,
	green   = 102,
	yellow  = 103,
	blue    = 104,
	magenta = 105,
	cyan    = 106,
	gray    = 107
};

enum class control { autoColor = 0, forceColor = 1 };


namespace rang_implementation {

	inline std::streambuf const *&RANG_coutbuf()
	{
		static std::streambuf const *pOutbuff = std::cout.rdbuf();
		return pOutbuff;
	}

	inline std::streambuf const *&RANG_cerrbuf()
	{
		static std::streambuf const *pErrbuff = std::cerr.rdbuf();
		return pErrbuff;
	}

	inline std::streambuf const *&RANG_clogbuf()
	{
		static std::streambuf const *pLogbuff = std::clog.rdbuf();
		return pLogbuff;
	}

	inline int getIword()
	{
		static int i = std::ios_base::xalloc();
		return i;
	}


	inline bool supportsColor()
	{
#if defined(OS_LINUX) || defined(OS_MAC)
		static constexpr const char* Terms[] = {
			"ansi", "color", "console", "cygwin", "gnome", "konsole", "kterm",
			"linux", "msys", "putty", "rxvt", "screen", "vt100", "xterm"
		};

		const char *env_p = std::getenv("TERM");
		if (env_p == nullptr) {
			return false;
		}

		static const bool result = std::any_of(
		  std::begin(Terms), std::end(Terms), [&](const char* term) {
			  return std::strstr(env_p, term) != nullptr;
		  });

#elif defined(OS_WIN)
		static constexpr bool result = true;
#endif
		return result;
	}


	inline bool isTerminal(const std::streambuf *osbuf)
	{
		if (osbuf == RANG_coutbuf()) {
#if defined(OS_LINUX) || defined(OS_MAC)
			return isatty(fileno(stdout)) ? true : false;
#elif defined(OS_WIN)
			return _isatty(_fileno(stdout)) ? true : false;
#endif
		}

		if (osbuf == RANG_cerrbuf() || osbuf == RANG_clogbuf()) {
#if defined(OS_LINUX) || defined(OS_MAC)
			return isatty(fileno(stderr)) ? true : false;
#elif defined(OS_WIN)
			return _isatty(_fileno(stderr)) ? true : false;
#endif
		}
		return false;
	}


	template <typename T>
	using enableStd =
	  typename std::enable_if<std::is_same<T, rang::style>::value
	      || std::is_same<T, rang::fg>::value
	      || std::is_same<T, rang::bg>::value
	      || std::is_same<T, rang::fgB>::value
	      || std::is_same<T, rang::bgB>::value,
	    std::ostream &>::type;


#ifdef OS_WIN
	HANDLE getVersionDependentHandle()
	{
		if (IsWindowsVersionOrGreater(10, 0, 0)) return nullptr;
		return GetStdHandle(STD_OUTPUT_HANDLE);
	}

	inline HANDLE getConsoleHandle()
	{
		static HANDLE h = getVersionDependentHandle();
		return h;
	}

	inline WORD reverseRGB(WORD rgb)
	{
		static const WORD rev[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };
		return rev[rgb];
	}

	inline void setWinAttribute(rang::bg col, WORD &state)
	{
		state &= 0xFF0F;
		state |= reverseRGB(static_cast<WORD>(col) - 40) << 4;
	}

	inline void setWinAttribute(rang::fg col, WORD &state)
	{
		state &= 0xFFF0;
		state |= reverseRGB(static_cast<WORD>(col) - 30);
	}

	inline void setWinAttribute(rang::bgB col, WORD &state)
	{
		state &= 0xFF0F;
		state |= (0x8 | reverseRGB(static_cast<WORD>(col) - 100)) << 4;
	}

	inline void setWinAttribute(rang::fgB col, WORD &state)
	{
		state &= 0xFFF0;
		state |= (0x8 | reverseRGB(static_cast<WORD>(col) - 90));
	}

	inline void setWinAttribute(rang::style style, WORD &state)
	{
		if (style == rang::style::reset) {
			state = (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		}
	}

	inline WORD &current_state()
	{
		static WORD state
		  = (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		return state;
	}

	template <typename T>
	inline enableStd<T> setColor(std::ostream &os, T const value)
	{
		HANDLE h = getConsoleHandle();
		if (h && isTerminal(os.rdbuf())) {
			setWinAttribute(value, current_state());
			SetConsoleTextAttribute(h, current_state());
			return os;
		}
		return os << "\033[" << static_cast<int>(value) << "m";
	}
#else
	template <typename T>
	inline enableStd<T> setColor(std::ostream &os, T const value)
	{
		return os << "\033[" << static_cast<int>(value) << "m";
	}
#endif

	template <typename T>
	using enableControl =
	  typename std::enable_if<std::is_same<T, rang::control>::value,
	    std::ostream &>::type;
}

inline void init()
{
	rang_implementation::RANG_coutbuf();
	rang_implementation::RANG_cerrbuf();
	rang_implementation::RANG_clogbuf();
}

template <typename T>
inline rang_implementation::enableStd<T> operator<<(
  std::ostream &os, T const value)
{
	std::streambuf const *osbuf = os.rdbuf();
	return (os.iword(rang_implementation::getIword())
	         || ((rang_implementation::supportsColor())
	         && (rang_implementation::isTerminal(osbuf))))
	  ? rang_implementation::setColor(os, value)
	  : os;
}

template <typename T>
inline rang_implementation::enableControl<T> operator<<(
  std::ostream &os, T const value)
{
	if (value == rang::control::forceColor) {
		os.iword(rang_implementation::getIword()) = 1;
	} else if (value == rang::control::autoColor) {
		os.iword(rang_implementation::getIword()) = 0;
	}
	return os;
}
}

#undef OS_LINUX
#undef OS_WIN
#undef OS_MAC

#endif /* ifndef RANG_DOT_HPP */
