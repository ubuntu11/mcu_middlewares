// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from chassis_interfaces:srv/GetString.idl
// generated code does not contain a copyright notice
#include "chassis_interfaces/srv/detail/get_string__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool
chassis_interfaces__srv__GetString_Request__init(chassis_interfaces__srv__GetString_Request * msg)
{
  if (!msg) {
    return false;
  }
  // structure_needs_at_least_one_member
  return true;
}

void
chassis_interfaces__srv__GetString_Request__fini(chassis_interfaces__srv__GetString_Request * msg)
{
  if (!msg) {
    return;
  }
  // structure_needs_at_least_one_member
}

chassis_interfaces__srv__GetString_Request *
chassis_interfaces__srv__GetString_Request__create()
{
  chassis_interfaces__srv__GetString_Request * msg = (chassis_interfaces__srv__GetString_Request *)malloc(sizeof(chassis_interfaces__srv__GetString_Request));
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(chassis_interfaces__srv__GetString_Request));
  bool success = chassis_interfaces__srv__GetString_Request__init(msg);
  if (!success) {
    free(msg);
    return NULL;
  }
  return msg;
}

void
chassis_interfaces__srv__GetString_Request__destroy(chassis_interfaces__srv__GetString_Request * msg)
{
  if (msg) {
    chassis_interfaces__srv__GetString_Request__fini(msg);
  }
  free(msg);
}


bool
chassis_interfaces__srv__GetString_Request__Sequence__init(chassis_interfaces__srv__GetString_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  chassis_interfaces__srv__GetString_Request * data = NULL;
  if (size) {
    data = (chassis_interfaces__srv__GetString_Request *)calloc(size, sizeof(chassis_interfaces__srv__GetString_Request));
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = chassis_interfaces__srv__GetString_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        chassis_interfaces__srv__GetString_Request__fini(&data[i - 1]);
      }
      free(data);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
chassis_interfaces__srv__GetString_Request__Sequence__fini(chassis_interfaces__srv__GetString_Request__Sequence * array)
{
  if (!array) {
    return;
  }
  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      chassis_interfaces__srv__GetString_Request__fini(&array->data[i]);
    }
    free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

chassis_interfaces__srv__GetString_Request__Sequence *
chassis_interfaces__srv__GetString_Request__Sequence__create(size_t size)
{
  chassis_interfaces__srv__GetString_Request__Sequence * array = (chassis_interfaces__srv__GetString_Request__Sequence *)malloc(sizeof(chassis_interfaces__srv__GetString_Request__Sequence));
  if (!array) {
    return NULL;
  }
  bool success = chassis_interfaces__srv__GetString_Request__Sequence__init(array, size);
  if (!success) {
    free(array);
    return NULL;
  }
  return array;
}

void
chassis_interfaces__srv__GetString_Request__Sequence__destroy(chassis_interfaces__srv__GetString_Request__Sequence * array)
{
  if (array) {
    chassis_interfaces__srv__GetString_Request__Sequence__fini(array);
  }
  free(array);
}


// Include directives for member types
// Member `data`
#include "rosidl_runtime_c/string_functions.h"

bool
chassis_interfaces__srv__GetString_Response__init(chassis_interfaces__srv__GetString_Response * msg)
{
  if (!msg) {
    return false;
  }
  // data
  if (!rosidl_runtime_c__String__init(&msg->data)) {
    chassis_interfaces__srv__GetString_Response__fini(msg);
    return false;
  }
  return true;
}

void
chassis_interfaces__srv__GetString_Response__fini(chassis_interfaces__srv__GetString_Response * msg)
{
  if (!msg) {
    return;
  }
  // data
  rosidl_runtime_c__String__fini(&msg->data);
}

chassis_interfaces__srv__GetString_Response *
chassis_interfaces__srv__GetString_Response__create()
{
  chassis_interfaces__srv__GetString_Response * msg = (chassis_interfaces__srv__GetString_Response *)malloc(sizeof(chassis_interfaces__srv__GetString_Response));
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(chassis_interfaces__srv__GetString_Response));
  bool success = chassis_interfaces__srv__GetString_Response__init(msg);
  if (!success) {
    free(msg);
    return NULL;
  }
  return msg;
}

void
chassis_interfaces__srv__GetString_Response__destroy(chassis_interfaces__srv__GetString_Response * msg)
{
  if (msg) {
    chassis_interfaces__srv__GetString_Response__fini(msg);
  }
  free(msg);
}


bool
chassis_interfaces__srv__GetString_Response__Sequence__init(chassis_interfaces__srv__GetString_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  chassis_interfaces__srv__GetString_Response * data = NULL;
  if (size) {
    data = (chassis_interfaces__srv__GetString_Response *)calloc(size, sizeof(chassis_interfaces__srv__GetString_Response));
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = chassis_interfaces__srv__GetString_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        chassis_interfaces__srv__GetString_Response__fini(&data[i - 1]);
      }
      free(data);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
chassis_interfaces__srv__GetString_Response__Sequence__fini(chassis_interfaces__srv__GetString_Response__Sequence * array)
{
  if (!array) {
    return;
  }
  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      chassis_interfaces__srv__GetString_Response__fini(&array->data[i]);
    }
    free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

chassis_interfaces__srv__GetString_Response__Sequence *
chassis_interfaces__srv__GetString_Response__Sequence__create(size_t size)
{
  chassis_interfaces__srv__GetString_Response__Sequence * array = (chassis_interfaces__srv__GetString_Response__Sequence *)malloc(sizeof(chassis_interfaces__srv__GetString_Response__Sequence));
  if (!array) {
    return NULL;
  }
  bool success = chassis_interfaces__srv__GetString_Response__Sequence__init(array, size);
  if (!success) {
    free(array);
    return NULL;
  }
  return array;
}

void
chassis_interfaces__srv__GetString_Response__Sequence__destroy(chassis_interfaces__srv__GetString_Response__Sequence * array)
{
  if (array) {
    chassis_interfaces__srv__GetString_Response__Sequence__fini(array);
  }
  free(array);
}
