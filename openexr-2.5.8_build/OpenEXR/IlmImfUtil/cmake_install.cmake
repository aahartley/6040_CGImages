# Install script for directory: /home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImfUtil-2_5.so.26.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImfUtil-2_5.so.26"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "\$ORIGIN/../lib:/home/aahartl/6040/build/lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/IlmImfUtil/libIlmImfUtil-2_5.so.26.0.0"
    "/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/IlmImfUtil/libIlmImfUtil-2_5.so.26"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImfUtil-2_5.so.26.0.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImfUtil-2_5.so.26"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/IlmImf:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Imath:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Half:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/IexMath:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/IlmThread:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Iex:"
           NEW_RPATH "\$ORIGIN/../lib:/home/aahartl/6040/build/lib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImfUtil-2_5.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImfUtil-2_5.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImfUtil-2_5.so"
         RPATH "\$ORIGIN/../lib:/home/aahartl/6040/build/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/IlmImfUtil/libIlmImfUtil-2_5.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImfUtil-2_5.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImfUtil-2_5.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImfUtil-2_5.so"
         OLD_RPATH "/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/IlmImf:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Imath:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Half:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/IexMath:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/IlmThread:/home/aahartl/6040/openexr-2.5.8_build/IlmBase/Iex:"
         NEW_RPATH "\$ORIGIN/../lib:/home/aahartl/6040/build/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libIlmImfUtil-2_5.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfImageChannel.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfFlatImageChannel.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfDeepImageChannel.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfSampleCountChannel.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfImageLevel.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfFlatImageLevel.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfDeepImageLevel.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfImage.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfFlatImage.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfDeepImage.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfImageIO.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfFlatImageIO.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfDeepImageIO.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfImageDataWindow.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfImageChannelRenaming.h"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/IlmImfUtil/ImfUtilExport.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND /usr/bin/cmake -E chdir "$ENV{DESTDIR}/home/aahartl/6040/build/lib" /usr/bin/cmake -E create_symlink libIlmImfUtil-2_5.so libIlmImfUtil.so)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  message("-- Creating symlink in  libIlmImfUtil.so -> libIlmImfUtil-2_5.so")
endif()

