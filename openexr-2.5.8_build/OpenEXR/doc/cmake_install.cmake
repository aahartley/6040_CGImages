# Install script for directory: /home/aahartl/6040/openexr-2.5.8/OpenEXR/doc

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/OpenEXR" TYPE FILE FILES
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/doc/TechnicalIntroduction.pdf"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/doc/ReadingAndWritingImageFiles.pdf"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/doc/OpenEXRFileLayout.pdf"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/doc/MultiViewOpenEXR.pdf"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/doc/InterpretingDeepPixels.pdf"
    "/home/aahartl/6040/openexr-2.5.8/OpenEXR/doc/TheoryDeepPixels.pdf"
    )
endif()

