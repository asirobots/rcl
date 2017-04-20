// Copyright 2017 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#if __cplusplus
extern "C"
{
#endif

#include "rcl/logging.h"

bool g_rcl_logging_initialized = false;

RclLogFunction g_rcl_logging_output_handler = NULL;

void rcl_logging_console_output_handler(
  int level, const char * name, const char * format, va_list * args)
{
  (void)level;
  (void)name;
  vprintf(format, *args);
}

void rcl_logging_initialize()
{
  printf("rcl_logging_initialize()\n");
  if (!g_rcl_logging_initialized) {
    g_rcl_logging_initialized = true;
    g_rcl_logging_output_handler = &rcl_logging_console_output_handler;
  }
}


RclLogFunction rcl_logging_get_output_handler()
{
  return g_rcl_logging_output_handler;
}

void rcl_logging_set_output_handler(RclLogFunction function)
{
  g_rcl_logging_output_handler = function;
}

/// Log a message.
/**
 *
 */
void rcl_log(int level, const char * name, const char * format, ...)
{
  RclLogFunction output_handler = g_rcl_logging_output_handler;
  if (output_handler) {
    va_list args;
    va_start(args, format);
    (*output_handler)(level, name, format, &args);
    va_end(args);
  }
}

#if __cplusplus
}
#endif
