# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/json-src"
  "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/json-build"
  "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/json-subbuild/json-populate-prefix"
  "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/json-subbuild/json-populate-prefix/tmp"
  "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp"
  "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/json-subbuild/json-populate-prefix/src"
  "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/_Platon/_Files/GitHub/mp_os_3sem/out/build/x64-Debug/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
