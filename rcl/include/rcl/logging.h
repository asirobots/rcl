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

#ifndef RCL__LOGGING_H_
#define RCL__LOGGING_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include "rcl/visibility_control.h"

#if __cplusplus
extern "C"
{
#endif

/// The severity levels of log message.
enum RCL_LOG_SEVERITY
{
  RCL_LOG_SEVERITY_DEBUG = 0,
  RCL_LOG_SEVERITY_INFO = 1,
  RCL_LOG_SEVERITY_WARN = 2,
  RCL_LOG_SEVERITY_ERROR = 3,
  RCL_LOG_SEVERITY_FATAL = 4,
};

/// The flag if the logging system has been initialized.
RCL_PUBLIC
extern bool g_rcl_logging_initialized;

/// The function signature to log messages.
/**
 * TODO document all args
 */
typedef void (* RclLogFunction)(int, const char *, const char *, va_list *);

/// The function pointer of the current output handler.
extern RclLogFunction g_rcl_logging_output_handler;

RCL_PUBLIC
void rcl_logging_console_output_handler(
  int level, const char * name, const char * format, va_list * args);

/// Initialize the logging system.
RCL_PUBLIC
void rcl_logging_initialize();

RCL_PUBLIC
RclLogFunction rcl_logging_get_output_handler();

RCL_PUBLIC
void rcl_logging_set_output_handler(RclLogFunction function);

/// Log a message.
/**
 *
 */
RCL_PUBLIC
void rcl_log(int level, const char * name, const char * format, ...);

/*
struct LogLocation
{
  bool initialized_;
  bool logger_enabled_;
  ::ros::console::Level level_;
  void* logger_;
};

#define ROSCONSOLE_DEFINE_LOCATION(cond, level, name) \
  ROSCONSOLE_AUTOINIT; \
  static ::ros::console::LogLocation __rosconsole_define_location__loc = {false, false, ::ros::console::levels::Count, 0}; \
  if (ROS_UNLIKELY(!__rosconsole_define_location__loc.initialized_)) \
  { \
    initializeLogLocation(&__rosconsole_define_location__loc, name, level); \
  } \
  if (ROS_UNLIKELY(__rosconsole_define_location__loc.level_ != level)) \
  { \
    setLogLocationLevel(&__rosconsole_define_location__loc, level); \
    checkLogLocationEnabled(&__rosconsole_define_location__loc); \
  } \
  bool __rosconsole_define_location__enabled = __rosconsole_define_location__loc.logger_enabled_ && (cond);
*/
/**
 * \brief Log to a given named logger at a given verbosity level, only if a given condition has been met, with printf-style formatting
 *
 * \note The condition will only be evaluated if this logging statement is enabled
 *
 * \param cond Boolean condition to be evaluated
 * \param level One of the levels specified in ::ros::console::levels::Level
 * \param name Name of the logger.  Note that this is the fully qualified name, and does NOT include "ros.<package_name>".  Use ROSCONSOLE_DEFAULT_NAME if you would like to use the default name.
 */
/*#define ROS_LOG_COND(cond, level, name, ...) \
  do \
  { \
    ROSCONSOLE_DEFINE_LOCATION(cond, level, name); \
    \
    if (ROS_UNLIKELY(__rosconsole_define_location__enabled)) \
    { \
      ROSCONSOLE_PRINT_AT_LOCATION(__VA_ARGS__); \
    } \
  } while(0)
*/
/**
 * \brief Log to a given named logger at a given verbosity level, only the first time it is hit when enabled, with printf-style formatting
 *
 * \param level One of the levels specified in ::ros::console::levels::Level
 * \param name Name of the logger.  Note that this is the fully qualified name, and does NOT include "ros.<package_name>".  Use ROSCONSOLE_DEFAULT_NAME if you would like to use the default name.
 */
/*#define ROS_LOG_ONCE(level, name, ...) \
  do \
  { \
    ROSCONSOLE_DEFINE_LOCATION(true, level, name); \
    static bool hit = false; \
    if (ROS_UNLIKELY(__rosconsole_define_location__enabled) && ROS_UNLIKELY(!hit)) \
    { \
      hit = true; \
      ROSCONSOLE_PRINT_AT_LOCATION(__VA_ARGS__); \
    } \
  } while(0)
*/

/*
typedef std::vector<LogLocation*> V_LogLocation;
V_LogLocation g_log_locations;
boost::mutex g_locations_mutex;
void registerLogLocation(LogLocation* loc)
{
  boost::mutex::scoped_lock lock(g_locations_mutex);

  g_log_locations.push_back(loc);
}

void checkLogLocationEnabledNoLock(LogLocation* loc)
{
  loc->logger_enabled_ = ::ros::console::impl::isEnabledFor(loc->logger_, loc->level_);
}

void initializeLogLocation(LogLocation* loc, const std::string& name, Level level)
{
  boost::mutex::scoped_lock lock(g_locations_mutex);

  if (loc->initialized_)
  {
    return;
  }

  loc->logger_ = ::ros::console::impl::getHandle(name);
  loc->level_ = level;

  g_log_locations.push_back(loc);

  checkLogLocationEnabledNoLock(loc);

  loc->initialized_ = true;
}

void setLogLocationLevel(LogLocation* loc, Level level)
{
  boost::mutex::scoped_lock lock(g_locations_mutex);
  loc->level_ = level;
}
*/

/*
typedef struct
{
    const char* loc_file;
    int loc_line;
    const char* loc_function;
    void* loc_data;

} log4c_location_info_t;

#ifdef __GNUC__
#   define LOG4C_LOCATION_INFO_INITIALIZER(user_data) { __FILE__, __LINE__, __FUNCTION__, user_data }
#else
#   define LOG4C_LOCATION_INFO_INITIALIZER(user_data) { __FILE__, __LINE__, "(nil)", user_data }
#endif

#define __log4c_str(n) #n

#ifdef __GNUC__
#   define __log4c_location(n)	__FUNCTION__ "() at " __FILE__ ":" __log4c_str(n)
#else
#   define __log4c_location(n)	__FILE__ ":" __log4c_str(n)
#endif

#define log4c_location __log4c_location(__LINE__)
*/

#if __cplusplus
}
#endif

#endif  // RCL__LOGGING_H_
