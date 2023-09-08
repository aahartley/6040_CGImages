# Install script for directory: /home/aahartl/6040/openexr-2.5.8/OpenEXR

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/config/cmake_install.cmake")
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/IlmImf/cmake_install.cmake")
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/IlmImfUtil/cmake_install.cmake")
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/IlmImfExamples/cmake_install.cmake")
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/IlmImfTest/cmake_install.cmake")
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/IlmImfUtilTest/cmake_install.cmake")
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/IlmImfFuzzTest/cmake_install.cmake")
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/exr2aces/cmake_install.cmake")
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/exrheader/cmake_install.cmake")
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/exrmaketiled/cmake_install.cmake")
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/exrstdattr/cmake_install.cmake")
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/exrmakepreview/cmake_install.cmake")
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/exrenvmap/cmake_install.cmake")
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/exrmultiview/cmake_install.cmake")
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/exrmultipart/cmake_install.cmake")
  include("/home/aahartl/6040/openexr-2.5.8_build/OpenEXR/doc/cmake_install.cmake")

endif()

