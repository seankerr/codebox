/**
 * Copyright (c) 2014 Sean Kerr
 *
 * Please view the LICENSE file for a full description of the license.
 */

#ifndef __CODEBOX_H
#define __CODEBOX_H

#include <stdio.h>

#include "codebox/container/buffer.h"
#include "codebox/container/list.h"
#include "codebox/container/stack.h"
#include "codebox/container/table.h"
#include "codebox/gl.h"
#include "codebox/io.h"
#include "codebox/util.h"

// -------------------------------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------------------------------

#ifndef NDEBUG

#define CB_DEBUG(__msg) fprintf(stdout, "[DEBUG] %s\n", __msg)
#define CB_ERROR(__msg) fprintf(stdout, "[ERROR] %s\n", __msg)
#define CB_INFO(__msg)  fprintf(stdout, "[INFO] %s\n", __msg)

#else

#define CB_DEBUG(__msg)
#define CB_ERROR(__msg)
#define CB_INFO(__msg)

#endif

#endif
