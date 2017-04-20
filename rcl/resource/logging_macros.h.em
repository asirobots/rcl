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

#ifndef RCL__LOGGING_MACROS_H_
#define RCL__LOGGING_MACROS_H_

#include "rcl/logging.h"

#include <stdio.h>
#include <stdlib.h>

#if __cplusplus
extern "C"
{
#endif

/**
 * \def RCL_LOG_MIN_SEVERITY
 * Define RCL_LOG_MIN_SEVERITY=RCL_LOG_SEVERITY_[DEBUG|INFO|WARN|ERROR|FATAL]
 * in your build options to compile out anything below that severity.
 */
#ifndef RCL_LOG_MIN_SEVERITY
#define RCL_LOG_MIN_SEVERITY RCL_LOG_SEVERITY_DEBUG
#endif

// Provide the compiler with branch prediction information
#ifndef WIN32
/**
 * \def RCL_LIKELY
 * Instruct the compiler to optimize for the case where the argument equals 1.
 */
# define RCL_LIKELY(x) __builtin_expect((x), 1)
/**
 * \def RCL_LIKELY
 * Instruct the compiler to optimize for the case where the argument equals 0.
 */
# define RCL_UNLIKELY(x) __builtin_expect((x), 0)
#else
/**
 * \def RCL_LIKELY
 * No op since Windows dosn't support providing branch prediction information.
 */
# define RCL_LIKELY(x) (x)
/**
 * \def RCL_LIKELY
 * No op since Windows dosn't support providing branch prediction information.
 */
# define RCL_UNLIKELY(x) (x)
#endif

/**
 * \def RCL_LOGGING_AUTOINIT
 * \brief Initialize the rcl logging library.
 * Usually it is unnecessary to call the macro directly.
 * All logging macros ensure that this has been called once.
 */
#define RCL_LOGGING_AUTOINIT \
  if (RCL_UNLIKELY(!g_rcl_logging_initialized)) \
  { \
    rcl_logging_initialize(); \
  }

///@@{
/**
 * \def RCL_LOG_CONDITION_EMPTY
 * An empty macro which can be used as a placeholder for `condition_before`
 * and `condition_after` which doesn't affect the logging call.
 */
#define RCL_LOG_CONDITION_EMPTY
///@@}

/** @@name Macros for the `once` condition which ignores all subsequent log
 * calls except the first one.
 */
///@@{
/**
 * \def RCL_LOG_CONDITION_ONCE_BEFORE
 * A macro initializing and checking the `once` condition.
 */
#define RCL_LOG_CONDITION_ONCE_BEFORE { \
    static int _once = 0; \
    if (RCL_UNLIKELY(0 == _once)) { \
      _once = 1;
/**
 * \def RCL_LOG_CONDITION_ONCE_AFTER
 * A macro finalizing the `once` condition.
 */
#define RCL_LOG_CONDITION_ONCE_AFTER } \
  }
///@@}

/** @@name Macros for the `skipfirst` condition which ignores the first log
 * call but processes all subsequent calls.
 */
///@@{
/**
 * \def RCL_LOG_CONDITION_SKIPFIRST_BEFORE
 * A macro initializing and checking the `skipfirst` condition.
 */
#define RCL_LOG_CONDITION_SKIPFIRST_BEFORE { \
    static bool _first = true; \
    if (RCL_UNLIKELY(true == _first)) { \
      _first = false; \
    } else {
/**
 * \def RCL_LOG_CONDITION_SKIPFIRST_AFTER
 * A macro finalizing the `skipfirst` condition.
 */
#define RCL_LOG_CONDITION_SKIPFIRST_AFTER } \
  }
///@@}

/** @@name Macros for the `throttle` condition which ignores log calls (from
 * the same code line and macro) if the last logged message is not longer ago
 * than the specified duration.
 */
///@@{
/**
 * \def RCL_LOG_CONDITION_THROTTLE_BEFORE
 * A macro initializing and checking the `throttle` condition.
 */
#define RCL_LOG_CONDITION_THROTTLE_BEFORE(time_source_type, duration) { \
  static rcl_time_source_t _time_source; \
  static rcl_duration_value_t _duration = RCL_MS_TO_NS((rcl_duration_value_t)duration); \
  \
  static bool _init_time_source_called = false; \
  if (RCL_UNLIKELY(!_init_time_source_called)) { \
    _init_time_source_called = true; \
    if (rcl_init_time_source(time_source_type, &_time_source)) { \
      rcl_log(RCL_LOG_SEVERITY_ERROR, "", "%s:%d initialization of time source type [%d] failed\n", __FILE__, __LINE__, time_source_type); \
    } \
  } \
  \
  static rcl_time_point_value_t _last_logged = 0; \
  rcl_time_point_value_t _now = 0; \
  bool _condition = true; \
  if (RCL_LIKELY(_time_source.get_now != NULL)) { \
    if (_time_source.get_now(_time_source.data, &_now)) { \
      rcl_log(RCL_LOG_SEVERITY_ERROR, "", "%s:%d getting current time from time source type [%d] failed\n", __FILE__, __LINE__, time_source_type); \
    } else { \
      _condition = _now >= _last_logged + _duration; \
    }\
  } \
  \
  if (RCL_LIKELY(_condition)) { \
    _last_logged = _now;

/**
 * \def RCL_LOG_CONDITION_THROTTLE_AFTER
 * A macro finalizing the `throttle` condition.
 */
#define RCL_LOG_CONDITION_THROTTLE_AFTER } \
  }
///@@}

/**
 * \def RCL_LOG_COND_NAMED
 * The logging macro all other logging macros call directly or indirectly.
 */
#define RCL_LOG_COND_NAMED(level, condition_before, condition_after, name, format, ...) \
  RCL_LOGGING_AUTOINIT \
  condition_before \
  rcl_log(level, name, format, ##__VA_ARGS__); \
  condition_after

@{
severities = ('DEBUG', 'INFO', 'WARN', 'ERROR', 'FATAL')

from collections import OrderedDict
# parameter name: argument value
default_args = OrderedDict((
  ('condition_before', 'RCL_LOG_CONDITION_EMPTY'),
  ('condition_after', 'RCL_LOG_CONDITION_EMPTY'),
  ('name', '""'),
))

name_params = ('name',)
name_args = {'name': 'name'}

# once_params = ()
once_args = {
  'condition_before': 'RCL_LOG_CONDITION_ONCE_BEFORE',
  'condition_after': 'RCL_LOG_CONDITION_ONCE_AFTER'}

# skipfirst_params = ()
skipfirst_args = {
  'condition_before': 'RCL_LOG_CONDITION_SKIPFIRST_BEFORE',
  'condition_after': 'RCL_LOG_CONDITION_SKIPFIRST_AFTER'}

throttle_params = ('time_source_type', 'duration')
throttle_args = {
  'condition_before': 'RCL_LOG_CONDITION_THROTTLE_BEFORE(time_source_type, duration)',
  'condition_after': 'RCL_LOG_CONDITION_THROTTLE_AFTER'}

# name only evaluated once (caller should not use changing variables)
# format always evaluated (otherwise it can easily be evaluated by the caller)
# n-th (easily implementable by caller, or using a condition)

# TODO condition expression evaluating to a boolean
# TODO filter function ptr
# TODO filter class (C++ only)
# TODO stream (C++ only)

class Feature(object):
  __slots__ = ('params', 'args')
  def __init__(self, *, params=None, args=None):
    if params is None:
      params = []
    self.params = params
    if args is None:
      args = {}
    self.args = args

feature_combinations = OrderedDict((
  ('', Feature()),
  ('_NAMED', Feature(
    params=name_params, args=name_args)),
  ('_ONCE', Feature(
    params=None, args=once_args)),
  ('_ONCE_NAMED', Feature(
    params=name_params, args={**once_args, **name_args})),
  ('_SKIPFIRST', Feature(
    params=None, args=skipfirst_args)),
  ('_SKIPFIRST_NAMED', Feature(
    params=name_params, args={**skipfirst_args, **name_args})),
  ('_THROTTLE', Feature(
    params=throttle_params, args=throttle_args)),
  ('_SKIPFIRST_THROTTLE', Feature(
    params=throttle_params, args={
      'condition_before': throttle_args['condition_before'] + ' ' + skipfirst_args['condition_before'],
      'condition_after': throttle_args['condition_after'] + ' ' + skipfirst_args['condition_after'],
    })),
  ('_THROTTLE_NAMED', Feature(
    params=throttle_params + name_params, args={**throttle_args, **name_args})),
  ('_SKIPFIRST_THROTTLE_NAMED', Feature(
    params=throttle_params + name_params, args={
      **{
        'condition_before': throttle_args['condition_before'] + ' ' + skipfirst_args['condition_before'],
        'condition_after': throttle_args['condition_after'] + ' ' + skipfirst_args['condition_after'],
      }, **name_args
    })),
))

def get_macro_parameters(suffix):
  return ''.join([p + ', ' for p in feature_combinations[suffix].params])

def get_macro_arguments(suffix):
  args = []
  for k, default_value in default_args.items():
    value = feature_combinations[suffix].args.get(k, default_value)
    args.append(value)
  return ''.join([a + ', ' for a in args])
}@
@[for severity in severities]@
/** @@name Logging macros for severity @(severity).
 */
///@@{
#if (RCL_LOG_MIN_SEVERITY > RCL_LOG_SEVERITY_@(severity))
// empty logging macros for severity @(severity) when being disabled at compile time
@[for suffix in feature_combinations]@
/// Empty logging macro due to the preprocessor definition of RCL_LOG_MIN_SEVERITY.
# define RCL_LOG_@(severity)@(suffix)(@(get_macro_parameters(suffix))format, ...)
@[end for]@

#else
@[for suffix in feature_combinations]@
/// Log a message with ... TODO generate something nicer here...
# define RCL_LOG_@(severity)@(suffix)(@(get_macro_parameters(suffix))format, ...) \
  RCL_LOG_COND_NAMED( \
    RCL_LOG_SEVERITY_@(severity), \
    @(get_macro_arguments(suffix))\
    format, ##__VA_ARGS__)
@[end for]@
#endif
///@@}

@[end for]@
#if __cplusplus
}
#endif

#endif  // RCL__LOGGING_MACROS_H_
