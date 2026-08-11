/*
 * Copyright 2021 Google LLC
 * SPDX-License-Identifier: MIT
 */

#include "render_common.h"
#ifdef ENABLE_SAME_PROCESS_RENDER_SERVER
#include "virgl_util.h"
#endif

#include <stdarg.h>
#include <stdio.h>
#ifndef _WIN32
#include <syslog.h>
#endif

void
render_log_init(void)
{
#ifndef _WIN32
   openlog(NULL, LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
#endif
}

void
render_log(const char *fmt, ...)
{
   va_list va;

   va_start(va, fmt);
#ifdef ENABLE_SAME_PROCESS_RENDER_SERVER
   virgl_prefixed_logv("server", VIRGL_LOG_LEVEL_INFO, fmt, va);
#else
#ifndef _WIN32
   vsyslog(LOG_DEBUG, fmt, va);
#else
   vfprintf(stderr, fmt, va);
#endif
#endif
   va_end(va);
}
