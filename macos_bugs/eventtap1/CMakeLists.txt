cmake_minimum_required (VERSION 3.9)

############################################################
# common

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories(SYSTEM /opt/local/include)
include_directories(SYSTEM /usr/local/include)
include_directories(SYSTEM ${CMAKE_CURRENT_LIST_DIR}/vendor)
include_directories(${CMAKE_CURRENT_LIST_DIR}/share)

add_compile_options(-Wall)
add_compile_options(-Werror)
add_compile_options(-O2)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=c++14>)
add_compile_options($<$<COMPILE_LANGUAGE:C>:-fobjc-arc>)
add_compile_options($<$<COMPILE_LANGUAGE:C>:-fmodules>)

# Variables to avoid string replacement in Info.plist

set(PLIST_EXECUTABLE_NAME "$\{EXECUTABLE_NAME\}")
set(PLIST_MACOSX_DEPLOYMENT_TARGET "$\{MACOSX_DEPLOYMENT_TARGET\}")
set(PLIST_PRODUCT_NAME "$\{PRODUCT_NAME\}")

############################################################

project (EventTapTest)

add_executable(
  EventTapTest
  MACOSX_BUNDLE
  main.m
)

set_target_properties(
  EventTapTest
  PROPERTIES
  MACOSX_BUNDLE_INFO_PLIST
  ${CMAKE_CURRENT_LIST_DIR}/plist.in
)

target_link_libraries(
  EventTapTest
  "-framework Carbon"
)
