// ============================================================================
// Copyright (c) 2017 Alinshans. All rights reserved.
// Licensed under the MIT License. See LICENSE for details.
// 
// Header File : redbud/platform.h
//
// This file contains some preprocessing macros related to the platform.
// ============================================================================

#ifndef ALINSHANS_REDBUD_PLATFORM_H_
#define ALINSHANS_REDBUD_PLATFORM_H_

// ----------------------------------------------------------------------------
// os

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
  #define REDBUD_WIN 1
#elif defined(__unix__) || defined(__unix) || defined(__linux__)
  #define REDBUD_LINUX 1
#elif defined(__APPLE__) || defined(__MACH__)
  #define REDBUD_OSX 1
#else
  #error "System that is not supported yet."
#endif

// ----------------------------------------------------------------------------
// complier

#if defined(__clang__)
  #define REDBUD_CLANG __clang__
#elif defined(__GNUC__)
  #define REDBUD_GNUC __GNUC__
#elif defined(_MSC_VER)
  #define REDBUD_MSVC _MSC_VER
#else
  #error "Complier that is not supported yet."
#endif

// ----------------------------------------------------------------------------
// stringify

#define REDBUD_TO_STRING(x) #x
#define REDBUD_STRING(x) REDBUD_TO_STRING(x)

// ----------------------------------------------------------------------------
// join

#define REDBUD_DO_JOIN(x ,y) x##y
#define REDBUD_JOIN(x, y) REDBUD_DO_JOIN(x, y)

// ----------------------------------------------------------------------------
// version

#define REDBUD_MAJOR 1
#define REDBUD_MINOR 0
#define REDBUD_PATCH 0
#define REDBUD_VERSION \
  REDBUD_STRING(REDBUD_MAJOR.REDBUD_MINOR.REDBUD_PATCH)

#define _VERSION_CODE(x,y,z) \
  (((x)*100000) + ((y)*100) + (z))

#define GNUC_VERSION \
  _VERSION_CODE(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)

#define CLANG_VERSION \
  _VERSION_CODE(__clang_major__, __clang_minor__, __clang_patchlevel__)

// ----------------------------------------------------------------------------
// redbud API

#ifndef _REDBUD_API
  #define _REDBUD_API ::redbud::
#endif

// ----------------------------------------------------------------------------
// C++11 required

#ifndef REDBUD_HAS_CXX11
  #if defined(REDBUD_MSVC) && (REDBUD_MSVC >= 1900)
    #define REDBUD_HAS_CXX11 1
  #elif defined(REDBUD_GNUC) && (GNUC_VERSION >= _VERSION_CODE(4,8,0)) && \
  defined(__GXX_EXPERIMENTAL_CXX0X__)
    #define REDBUD_HAS_CXX11 1
  #elif defined(REDBUD_CLANG) && (CLANG_VERSION >= _VERSION_CODE(3,3,0))
    #define REDBUD_HAS_CXX11 1
  #else 
    #define REDBUD_HAS_CXX11 0
  #endif
#endif // !REDBUD_HAS_CXX11

#if REDBUD_HAS_CXX11 == 0
  #error "C++11 required."
#endif

// ----------------------------------------------------------------------------
// Undefines macro min and max in MSVC.

namespace redbud
{

#if REDBUD_MSVC
  #define NOMINMAX
#endif

}

#endif // !ALINSHANS_REDBUD_PLATFORM_H_
