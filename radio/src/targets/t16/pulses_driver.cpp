/*
 * Copyright (C) JumperTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "opentx.h"

void moduleStop(uint32_t port);
void moduleSerialStart(uint32_t port, uint32_t baudrate, uint32_t period_half_us);
void moduleNoneStart(uint32_t port);

void extmodulePpmStart(void);
void extmodulePxxStart(void);
void extmoduleCrossfireStart(void);

enum PULSES_STATUS : uint8_t {
  DISABLED = 1,
  ACTIVE,
  NO_PULSES
};

volatile uint8_t s_status[NUM_MODULES] = { DISABLED, DISABLED };


//Update timer active
void init_no_pulses(uint32_t port)
{
  s_status[port] = NO_PULSES;
  //In case of shared GPIO it is more complicated.
#if defined(SHARED_MODULE_GPIO)
  //disable module
  if(port == INTERNAL_MODULE) INTERNAL_MODULE_OFF();
  if(port == EXTERNAL_MODULE) EXTERNAL_MODULE_OFF();

  if(s_status[INTERNAL_MODULE] == NO_PULSES && s_status[EXTERNAL_MODULE] == NO_PULSES) {
    moduleNoneStart(port);
  }
#else
  if(port == EXTERNAL_MODULE) moduleNoneStart(port);
#endif
}

void disable_no_pulses(uint32_t port)
{
  s_status[port] = DISABLED;
  //In case of shared GPIO it is more complicated.
#if defined(SHARED_MODULE_GPIO)
  if(port == INTERNAL_MODULE) INTERNAL_MODULE_OFF();
  if(port == EXTERNAL_MODULE) EXTERNAL_MODULE_OFF();
  if(s_status[INTERNAL_MODULE] == DISABLED && s_status[EXTERNAL_MODULE] == DISABLED) {
    moduleNoneStart(port);
  }
#else
  if(port == EXTERNAL_MODULE) moduleStop(port);
#endif
}

void init_ppm(uint32_t port)
{
  if (port == EXTERNAL_MODULE) {
    s_status[port] = ACTIVE;
    extmodulePpmStart();
  }
}

void disable_ppm(uint32_t port)
{
  if (port == EXTERNAL_MODULE) {
    disable_no_pulses(port);
  }
}

void init_pxx(uint32_t port)
{
  if (port == EXTERNAL_MODULE) {
    s_status[port] = ACTIVE;
    extmodulePxxStart();
  }
}

void disable_pxx(uint32_t port)
{
  if (port == EXTERNAL_MODULE) {
    disable_no_pulses(port);
  }
}

void init_serial(uint32_t port, uint32_t baudrate, uint32_t period_half_us)
{
  s_status[port] = ACTIVE;
  moduleSerialStart(port, baudrate, period_half_us);
}

void disable_serial(uint32_t port)
{
  disable_no_pulses(port);
}

void init_crossfire(uint32_t port)
{
  if (port == EXTERNAL_MODULE) {
    s_status[port] = ACTIVE;
    extmoduleCrossfireStart();
  }
}

void disable_crossfire(uint32_t port)
{
  if (port == EXTERNAL_MODULE) {
    disable_no_pulses(port);
  }
}
