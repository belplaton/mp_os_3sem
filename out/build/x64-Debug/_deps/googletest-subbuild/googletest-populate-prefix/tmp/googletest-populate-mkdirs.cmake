# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/googletest-src"
  "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/googletest-build"
  "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/googletest-subbuild/googletest-populate-prefix"
  "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/googletest-subbuild/googletest-populate-prefix/tmp"
  "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
  "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/googletest-subbuild/googletest-populate-prefix/src"
  "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
