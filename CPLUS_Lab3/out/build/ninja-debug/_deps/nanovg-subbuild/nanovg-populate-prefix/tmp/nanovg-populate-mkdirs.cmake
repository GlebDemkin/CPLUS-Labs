# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "D:/Учёба/C++/CPLUS-Labs/CPLUS_Lab3/out/build/ninja-debug/_deps/nanovg-src")
  file(MAKE_DIRECTORY "D:/Учёба/C++/CPLUS-Labs/CPLUS_Lab3/out/build/ninja-debug/_deps/nanovg-src")
endif()
file(MAKE_DIRECTORY
  "D:/Учёба/C++/CPLUS-Labs/CPLUS_Lab3/out/build/ninja-debug/_deps/nanovg-build"
  "D:/Учёба/C++/CPLUS-Labs/CPLUS_Lab3/out/build/ninja-debug/_deps/nanovg-subbuild/nanovg-populate-prefix"
  "D:/Учёба/C++/CPLUS-Labs/CPLUS_Lab3/out/build/ninja-debug/_deps/nanovg-subbuild/nanovg-populate-prefix/tmp"
  "D:/Учёба/C++/CPLUS-Labs/CPLUS_Lab3/out/build/ninja-debug/_deps/nanovg-subbuild/nanovg-populate-prefix/src/nanovg-populate-stamp"
  "D:/Учёба/C++/CPLUS-Labs/CPLUS_Lab3/out/build/ninja-debug/_deps/nanovg-subbuild/nanovg-populate-prefix/src"
  "D:/Учёба/C++/CPLUS-Labs/CPLUS_Lab3/out/build/ninja-debug/_deps/nanovg-subbuild/nanovg-populate-prefix/src/nanovg-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Учёба/C++/CPLUS-Labs/CPLUS_Lab3/out/build/ninja-debug/_deps/nanovg-subbuild/nanovg-populate-prefix/src/nanovg-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Учёба/C++/CPLUS-Labs/CPLUS_Lab3/out/build/ninja-debug/_deps/nanovg-subbuild/nanovg-populate-prefix/src/nanovg-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
