// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from chassis_interfaces:msg/StringMap.idl
// generated code does not contain a copyright notice

#ifndef CHASSIS_INTERFACES__MSG__DETAIL__STRING_MAP__STRUCT_H_
#define CHASSIS_INTERFACES__MSG__DETAIL__STRING_MAP__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'key'
// Member 'value'
#include "rosidl_runtime_c/string.h"

// Struct defined in msg/StringMap in the package chassis_interfaces.
typedef struct chassis_interfaces__msg__StringMap
{
  rosidl_runtime_c__String key;
  rosidl_runtime_c__String value;
} chassis_interfaces__msg__StringMap;

// Struct for a sequence of chassis_interfaces__msg__StringMap.
typedef struct chassis_interfaces__msg__StringMap__Sequence
{
  chassis_interfaces__msg__StringMap * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} chassis_interfaces__msg__StringMap__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CHASSIS_INTERFACES__MSG__DETAIL__STRING_MAP__STRUCT_H_
