# Install script for directory: /home/aahartl/6040/openexr-2.5.8/IlmBase/Imath

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/aahartl/6040/build")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath-2_5.so.25.0.7"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath-2_5.so.25"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "\$ORIGIN/../lib:/home/aahartl/6040/build/lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Imath/libImath-2_5.so.25.0.7"
    "/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Imath/libImath-2_5.so.25"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath-2_5.so.25.0.7"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath-2_5.so.25"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Half:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/IexMath:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Iex:"
           NEW_RPATH "\$ORIGIN/../lib:/home/aahartl/6040/build/lib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath-2_5.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath-2_5.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath-2_5.so"
         RPATH "\$ORIGIN/../lib:/home/aahartl/6040/build/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Imath/libImath-2_5.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath-2_5.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath-2_5.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath-2_5.so"
         OLD_RPATH "/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Half:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/IexMath:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Iex:"
         NEW_RPATH "\$ORIGIN/../lib:/home/aahartl/6040/build/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath-2_5.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathBoxAlgo.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathBox.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathColorAlgo.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathColor.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathEuler.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathExc.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathExport.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathForward.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathFrame.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathFrustum.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathFrustumTest.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathFun.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathGL.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathGLU.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathHalfLimits.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathInt64.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathInterval.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathLimits.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathLineAlgo.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathLine.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathMath.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathMatrixAlgo.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathMatrix.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathNamespace.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathPlane.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathPlatform.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathQuat.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathRandom.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathRoots.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathShear.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathSphere.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathVecAlgo.h"
    "/home/aahartl/6040/openexr-2.5.8/IlmBase/Imath/ImathVec.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND /usr/bin/cmake -E chdir "$ENV{DESTDIR}/home/aahartl/6040/build/lib" /usr/bin/cmake -E create_symlink libImath-2_5.so libImath.so)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  message("-- Creating symlink in /home/aahartl/6040/build/lib libImath.so -> libImath-2_5.so")
endif()

