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

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "rcl/logging.h"

#ifdef RMW_IMPLEMENTATION
# define CLASSNAME_(NAME, SUFFIX) NAME ## __ ## SUFFIX
# define CLASSNAME(NAME, SUFFIX) CLASSNAME_(NAME, SUFFIX)
#else
# define CLASSNAME(NAME, SUFFIX) NAME
#endif

TEST(CLASSNAME(TestLogging, RMW_IMPLEMENTATION), test_logging_initialization) {
  EXPECT_EQ(g_rcl_logging_initialized, false);
  rcl_logging_initialize();
  EXPECT_EQ(g_rcl_logging_initialized, true);
  rcl_logging_initialize();
  EXPECT_EQ(g_rcl_logging_initialized, true);
  g_rcl_logging_initialized = false;
  EXPECT_EQ(g_rcl_logging_initialized, false);
}

#define MAX_BUFFER_SIZE 1024

size_t g_log_calls = 0;

struct LogEvent
{
  int level;
  std::string name;
  std::string message;
};
LogEvent g_last_log_event;

TEST(CLASSNAME(TestLogging, RMW_IMPLEMENTATION), test_logging) {
  EXPECT_EQ(g_rcl_logging_initialized, false);
  rcl_logging_initialize();
  EXPECT_EQ(g_rcl_logging_initialized, true);

  auto rcl_logging_console_output_handler = [](
    int level, const char * name, const char * format, va_list * args) -> void
    {
      g_log_calls += 1;
      g_last_log_event.level = level;
      g_last_log_event.name = name;
      char buffer[MAX_BUFFER_SIZE];
      vsnprintf(buffer, MAX_BUFFER_SIZE, format, *args);
      g_last_log_event.message = buffer;
    };

  RclLogFunction original_function = rcl_logging_get_output_handler();
  rcl_logging_set_output_handler(rcl_logging_console_output_handler);

  g_log_calls = 0;
  rcl_log(RCL_LOG_SEVERITY_WARN, "name1", "message %d\n", 11);
  EXPECT_EQ(g_log_calls, 1u);
  EXPECT_EQ(g_last_log_event.level, RCL_LOG_SEVERITY_WARN);
  EXPECT_EQ(g_last_log_event.name, "name1");
  EXPECT_EQ(g_last_log_event.message, "message 11\n");

  rcl_log(RCL_LOG_SEVERITY_ERROR, "name2", "message %d\n", 22);
  EXPECT_EQ(g_log_calls, 2u);
  EXPECT_EQ(g_last_log_event.level, RCL_LOG_SEVERITY_ERROR);
  EXPECT_EQ(g_last_log_event.name, "name2");
  EXPECT_EQ(g_last_log_event.message, "message 22\n");

  rcl_logging_set_output_handler(original_function);
  g_rcl_logging_initialized = false;
  EXPECT_EQ(g_rcl_logging_initialized, false);
}
