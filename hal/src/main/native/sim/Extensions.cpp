/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2017 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "HAL/Extensions.h"

#include <llvm/SmallString.h>
#include <llvm/StringRef.h>

#include "HAL/HAL.h"

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#if defined(WIN32) || defined(_WIN32)
#define DELIM ';'
#define HTYPE HMODULE
#define PREFIX NULL
#define SUFFIX NULL
#define DLOPEN(a) LoadLibrary(a)
#define DLSYM GetProcAddress
#define DLCLOSE FreeLibrary
#else
#define DELIM ':'
#define HTYPE void*
#define PREFIX "lib"
#if defined(__APPLE__)
#define SUFFIX ".dylib"
#else
#define SUFFIX ".so"
#endif
#define DLOPEN(a) dlopen(a, RTLD_LAZY)
#define DLSYM dlsym
#define DLCLOSE dlclose
#endif

using namespace hal;

extern "C" {

int HAL_LoadOneExtension(const char* library) {
  int rc = 1;  // It is expected and reasonable not to find an extra simulation
  HTYPE handle = DLOPEN(library);
  if (!handle && (PREFIX || SUFFIX)) {
    llvm::SmallString<128> library_name;
    if (PREFIX) library_name += PREFIX;
    library_name += library;
    if (SUFFIX) library_name += SUFFIX;
    handle = DLOPEN(library_name.c_str());
  }
  if (!handle) return rc;

  halsim_extension_init_func_t* init;
  init = reinterpret_cast<halsim_extension_init_func_t*>(
      DLSYM(handle, "HALSIM_InitExtension"));

  if (init) {
    rc = (*init)();
  }

  if (rc != 0) {
    DLCLOSE(handle);
  }
  return rc;
}

/* Load any extra halsim libraries provided in the HALSIM_EXTENSIONS
   environment variable. */
int HAL_LoadExtensions(void) {
  int rc = 1;
  llvm::SmallVector<llvm::StringRef, 2> libraries;
  const char* e = std::getenv("HALSIM_EXTENSIONS");
  if (!e) return rc;
  llvm::StringRef env{e};
  env.split(libraries, DELIM, -1, false);
  for (size_t i = 0; i < libraries.size(); i++) {
    llvm::SmallString<128> library(libraries[i]);
    rc = HAL_LoadOneExtension(library.c_str());
    if (rc < 0) break;
  }
  return rc;
}

}  // extern "C"
