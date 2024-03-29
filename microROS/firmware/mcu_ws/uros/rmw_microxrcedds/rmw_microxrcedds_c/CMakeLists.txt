# Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0057 NEW)

set(PACKAGE_VERSION "0.1.0")
project(rmw_microxrcedds
  VERSION
    ${PACKAGE_VERSION}
  LANGUAGES
    C
)

find_package(ament_cmake_ros REQUIRED)
find_package(rcutils REQUIRED)
find_package(rosidl_runtime_c REQUIRED)

find_package(microxrcedds_client REQUIRED)
find_package(microcdr REQUIRED)

find_package(rmw REQUIRED)

# Build options
option(BUILD_DOCUMENTATION "Use doxygen to create product documentation" OFF)
option(RMW_UXRCE_GRAPH "Allows to perform graph-related operations to the user" OFF)

if(RMW_UXRCE_GRAPH)
  find_package(micro_ros_msgs REQUIRED)
endif()

# --- RMW CONFIGURATION ---
set(RMW_UXRCE_TRANSPORT "udp" CACHE STRING "Sets Micro XRCE-DDS transport to use. (udp | serial | custom)")
set(RMW_UXRCE_IPV "ipv4" CACHE STRING "Sets Micro XRCE-DDS IP version to use. (ipv4 | ipv6)")
set(RMW_UXRCE_CREATION_MODE "xml" CACHE STRING "Sets creation mode in Micro XRCE-DDS. (xml | refs)")
set(RMW_UXRCE_MAX_HISTORY "8" CACHE STRING "This value sets the number of history slots available for RMW subscriptions, requests and replies")
set(RMW_UXRCE_MAX_SESSIONS "1" CACHE STRING "This value sets the maximum number of Micro XRCE-DDS sessions.")
set(RMW_UXRCE_MAX_NODES "4" CACHE STRING "This value sets the maximum number of nodes.")
set(RMW_UXRCE_MAX_PUBLISHERS "4" CACHE STRING "This value sets the maximum number of publishers for an application.")
set(RMW_UXRCE_MAX_SUBSCRIPTIONS "4" CACHE STRING "This value sets the maximum number of subscriptions for an application.")
set(RMW_UXRCE_MAX_SERVICES "4" CACHE STRING "This value sets the maximum number of services for an application.")
set(RMW_UXRCE_MAX_CLIENTS "4" CACHE STRING "This value sets the maximum number of clients for an application.")
set(RMW_UXRCE_MAX_TOPICS "-1" CACHE STRING "This value sets the maximum number of topics for an application. If set to -1 RMW_UXRCE_MAX_TOPICS = RMW_UXRCE_MAX_PUBLISHERS + RMW_UXRCE_MAX_SUBSCRIPTIONS + RMW_UXRCE_MAX_NODES.")
option(RMW_UXRCE_ALLOW_DYNAMIC_ALLOCATIONS "Enables increasing static pools with dynamic allocation when needed." OFF)
set(RMW_UXRCE_NODE_NAME_MAX_LENGTH "128" CACHE STRING "This value sets the maximum number of characters for a node name.")
set(RMW_UXRCE_TOPIC_NAME_MAX_LENGTH "100" CACHE STRING "This value sets the maximum number of characters for a topic name.")
set(RMW_UXRCE_TYPE_NAME_MAX_LENGTH "128" CACHE STRING "This value sets the maximum number of characters for a type name.")
set(RMW_UXRCE_REF_BUFFER_LENGTH "100" CACHE STRING "This value sets the maximum number of characters for a reference buffer.")
set(RMW_UXRCE_ENTITY_CREATION_DESTROY_TIMEOUT "1000" CACHE STRING
  "This value sets the maximum time to wait for an XRCE entity creation and destroy in milliseconds.
  If set to 0 best effort is used.")
set(RMW_UXRCE_PUBLISH_RELIABLE_TIMEOUT "1000" CACHE STRING
  "This value sets the default time to wait for a publication in a reliable mode in milliseconds.")

set(RMW_UXRCE_STREAM_HISTORY "4" CACHE STRING "This value sets the number of MTUs to buffer, both input and output. Micro XRCE-DDS client configuration provides their size")
set(RMW_UXRCE_STREAM_HISTORY_INPUT "" CACHE STRING "This value sets the number of MTUs to input buffer. It will be ignored if RMW_UXRCE_STREAM_HISTORY_OUTPUT is blank. Micro XRCE-DDS client configuration provides their size")
set(RMW_UXRCE_STREAM_HISTORY_OUTPUT "" CACHE STRING "This value sets the number of MTUs to output buffer. It will be ignored if RMW_UXRCE_STREAM_HISTORY_INPUT is blank. Micro XRCE-DDS client configuration provides their size")

if(RMW_UXRCE_STREAM_HISTORY_INPUT STREQUAL "" OR RMW_UXRCE_STREAM_HISTORY_OUTPUT STREQUAL "")
  unset(RMW_UXRCE_STREAM_HISTORY_INPUT)
  unset(RMW_UXRCE_STREAM_HISTORY_OUTPUT)
else()
  unset(RMW_UXRCE_STREAM_HISTORY)
endif()

# Transport handle define macros.
set(RMW_UXRCE_TRANSPORT_SERIAL OFF)
set(RMW_UXRCE_TRANSPORT_UDP OFF)
set(RMW_UXRCE_TRANSPORT_IPV4 OFF)
set(RMW_UXRCE_TRANSPORT_IPV6 OFF)
set(RMW_UXRCE_TRANSPORT_CUSTOM OFF)
if(${RMW_UXRCE_TRANSPORT} STREQUAL "serial")
  set(RMW_UXRCE_TRANSPORT_SERIAL ON)
  set(RMW_UXRCE_DEFAULT_SERIAL_DEVICE "/dev/ttyAMA0" CACHE STRING "Sets the agent serial port.")
elseif(${RMW_UXRCE_TRANSPORT} STREQUAL "udp")
  set(RMW_UXRCE_TRANSPORT_UDP ON)
  if(${RMW_UXRCE_IPV} STREQUAL "ipv4")
    set(RMW_UXRCE_TRANSPORT_IPV4 ON)
    set(RMW_UXRCE_DEFAULT_UDP_IP "127.0.0.1" CACHE STRING "Sets the agent IP address.")
  elseif(${RMW_UXRCE_IPV} STREQUAL "ipv6")
    set(RMW_UXRCE_TRANSPORT_IPV6 ON)
    set(RMW_UXRCE_DEFAULT_UDP_IP "::1" CACHE STRING "Sets the agent IP address.")
  endif()
  set(RMW_UXRCE_DEFAULT_UDP_PORT "8888" CACHE STRING "Sets the agent IP port.")
elseif(${RMW_UXRCE_TRANSPORT} STREQUAL "custom")
  set(RMW_UXRCE_TRANSPORT_CUSTOM ON)
else()
  message(FATAL_ERROR "Transport not supported. Use \"serial\", \"udp\", \"custom\"")
endif()

# Create entities type define macros.
set(RMW_UXRCE_USE_REFS OFF)
if(${RMW_UXRCE_CREATION_MODE} STREQUAL "refs")
  set(RMW_UXRCE_USE_REFS ON)
endif()

# Create source files with the define
configure_file(${PROJECT_SOURCE_DIR}/src/config.h.in
  ${PROJECT_BINARY_DIR}/include/rmw_microxrcedds_c/config.h)

# Set install directories
if(WIN32)
    set(DOC_DIR "doc")
else()
    set(DOC_DIR "${DATA_INSTALL_DIR}/doc")
endif()

set(DOC_INSTALL_DIR ${DOC_DIR} CACHE PATH "Installation directory for documentation")

set(SRCS
  src/identifiers.c
  src/memory.c
  src/rmw_client.c
  src/rmw_compare_gids_equal.c
  src/rmw_count.c
  src/rmw_event.c
  src/rmw_get_gid_for_publisher.c
  src/rmw_get_implementation_identifier.c
  src/rmw_get_serialization_format.c
  src/rmw_get_topic_endpoint_info.c
  src/rmw_guard_condition.c
  src/rmw_init.c
  src/rmw_logging.c
  src/rmw_microxrcedds_topic.c
  src/rmw_node.c
  src/rmw_node_info_and_types.c
  src/rmw_node_names.c
  src/rmw_publish.c
  src/rmw_publisher.c
  src/rmw_request.c
  src/rmw_response.c
  src/rmw_serialize.c
  src/rmw_service.c
  src/rmw_service_names_and_types.c
  src/rmw_service_server_is_available.c
  src/rmw_subscription.c
  src/rmw_take.c
  src/rmw_topic_names_and_types.c
  src/rmw_trigger_guard_condition.c
  src/rmw_wait.c
  src/rmw_wait_set.c
  src/types.c
  src/utils.c
  src/callbacks.c
  src/rmw_uxrce_transports.c
  src/rmw_microros/continous_serialization.c
  src/rmw_microros/init_options.c
  src/rmw_microros/time_sync.c
  src/rmw_microros/ping.c
  src/rmw_microros/timing.c
  $<$<BOOL:${RMW_UXRCE_TRANSPORT_UDP}>:src/rmw_microros/discovery.c>
  $<$<BOOL:${RMW_UXRCE_TRANSPORT_CUSTOM}>:src/rmw_microros/custom_transport.c>
  $<$<BOOL:${RMW_UXRCE_GRAPH}>:src/rmw_graph.c>
)

add_library(${PROJECT_NAME}
  ${SRCS})

if(RMW_UXRCE_GRAPH)
  add_library(micro_ros_msgs_lib SHARED IMPORTED)

  set_property(
    TARGET
      micro_ros_msgs_lib
    PROPERTY
      IMPORTED_LOCATION ${micro_ros_msgs_LIBRARIES__rosidl_typesupport_microxrcedds_c})

  ament_target_dependencies(${PROJECT_NAME}
    "micro_ros_msgs"
)
endif()

target_link_libraries(${PROJECT_NAME}
  microcdr
  microxrcedds_client
  $<$<BOOL:${RMW_UXRCE_GRAPH}>:micro_ros_msgs_lib>
)

# Type support lock-up mechanism
find_package(rosidl_typesupport_microxrcedds_c)
if(rosidl_typesupport_microxrcedds_c_FOUND)
  ament_target_dependencies(${PROJECT_NAME}
    "rosidl_typesupport_microxrcedds_c"
)
  target_compile_definitions(${PROJECT_NAME}
    PUBLIC
      HAVE_C_TYPESUPPORT
)
else()
  find_package(rosidl_typesupport_microxrcedds_cpp REQUIRED)
endif()
if(rosidl_typesupport_microxrcedds_cpp_FOUND)
  ament_target_dependencies(${PROJECT_NAME}
    "rosidl_typesupport_microxrcedds_cpp"
)
  target_compile_definitions(${PROJECT_NAME}
    PUBLIC
      HAVE_CPP_TYPESUPPORT
)
endif()

ament_target_dependencies(${PROJECT_NAME}
  "rcutils"
  "rmw"
  "microxrcedds_client"
)

configure_rmw_library(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
  PRIVATE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  C_STANDARD
    99
  C_STANDARD_REQUIRED
    YES
)

target_compile_options(${PROJECT_NAME}
  PRIVATE
    $<$<C_COMPILER_ID:GNU>:-Wall>
    $<$<C_COMPILER_ID:GNU>:-Wextra>
    $<$<C_COMPILER_ID:GNU>:-pedantic>
    $<$<C_COMPILER_ID:GNU>:-Werror-implicit-function-declaration>
    $<$<C_COMPILER_ID:GNU>:$<$<VERSION_GREATER:$<C_COMPILER_VERSION>,8.1>:-Wcast-align=strict>>
    $<$<C_COMPILER_ID:GNU>:-Wvla>
    $<$<C_COMPILER_ID:MSVC>:/Wall>
)

file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include)

ament_export_include_directories(${CMAKE_INSTALL_PREFIX}/include)
ament_export_include_directories(${microxrcedds_client_INCLUDE_DIR})
ament_export_include_directories(${microcdr_INCLUDE_DIR})
ament_export_libraries(${PROJECT_NAME})

ament_export_dependencies(
  rcutils
  rmw
  microxrcedds_client
)

if(rosidl_typesupport_microxrcedds_c_FOUND)
  ament_export_dependencies(rosidl_typesupport_microxrcedds_c)
  set(implementations "c:rosidl_typesupport_c:rosidl_typesupport_microxrcedds_c")
endif()
if(rosidl_typesupport_microxrcedds_cpp_FOUND)
  ament_export_dependencies(rosidl_typesupport_microxrcedds_cpp)
  set(implementations
    ${implementations}
    "cpp:rosidl_typesupport_cpp:rosidl_typesupport_microxrcedds_cpp"
)
endif()

register_rmw_implementation(${implementations})

if(BUILD_TESTING)
  # Pedantic in CI
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
  add_subdirectory(test)
endif()

# Documentation
if(BUILD_DOCUMENTATION)
    find_package(Doxygen)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "doxygen is needed to build the documentation. Please install it correctly")
    endif()
    if(UNIX)
        find_program(DOXYFILE_MAKE make)
        if(DOXYFILE_MAKE)
            message(STATUS "Found Make: ${DOXYFILE_MAKE}")
        else()
            message(FATAL_ERROR "make is needed to build the documentation. Please install it correctly")
        endif()
    elseif(WIN32)
        set(DOXYFILE_MAKE make.bat)
    endif()

    # Target to create documentation directories
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/doc)
    add_custom_target(docdirs
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/doc/api_reference
        COMMENT "Creating documentation directory" VERBATIM)

    ### Doxygen ########################3
    # Configure the template doxyfile for or specific project
    configure_file(doxyfile.in ${PROJECT_BINARY_DIR}/doxyfile @ONLY IMMEDIATE)
    # Add custom target to run doxygen when ever the project is build
    add_custom_target(doxygen
        COMMAND "${DOXYGEN_EXECUTABLE}" "${PROJECT_BINARY_DIR}/doxyfile"
        SOURCES "${PROJECT_BINARY_DIR}/doxyfile"
        COMMENT "Generating API documentation with doxygen" VERBATIM)

    add_dependencies(doxygen docdirs)

    add_custom_target(doc ALL
    COMMENT "Generated project documentation" VERBATIM)

    add_dependencies(doc doxygen)
endif()

ament_package()

# Install includes.
install(
  DIRECTORY
    ${PROJECT_SOURCE_DIR}/include/
  DESTINATION
    include
)

# Install config.h file.
install(
  FILES
    ${PROJECT_BINARY_DIR}/include/rmw_microxrcedds_c/config.h
  DESTINATION
    include/rmw_microxrcedds_c
)

# Install library.
install(
  TARGETS
    ${PROJECT_NAME}
  ARCHIVE DESTINATION
    lib
  LIBRARY DESTINATION
    lib
  RUNTIME DESTINATION
    bin
)

if(BUILD_DOCUMENTATION)
  # Instalation of doxygen files
  install(DIRECTORY ${PROJECT_BINARY_DIR}/doc/api_reference
      DESTINATION ${DOC_INSTALL_DIR}
      COMPONENT documentation)
endif()