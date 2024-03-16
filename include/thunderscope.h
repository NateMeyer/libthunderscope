/* SPDX-License-Identifier: BSD-2-Clause
 *
 * Thunderscope library
 *
 * Copyright (C) 2024 / Nate Meyer  / nate.devel@gmail.com
 *
 */
#ifndef _THUNDERSCOPE_H_
#define _THUNDERSCOPE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


#include "channel.h"
#include "adc.h"
#include "afe.h"


int32_t thunderscopeInit(void);
int32_t thunderscopeOpen(void);

#ifdef __cplusplus
}
#endif
#endif