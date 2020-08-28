/******************************************************************************\

                 This file is part of the Buildbotics firmware.

                   Copyright (c) 2015 - 2018, Buildbotics LLC
                              All rights reserved.

      This file ("the software") is free software: you can redistribute it
      and/or modify it under the terms of the GNU General Public License,
       version 2 as published by the Free Software Foundation. You should
       have received a copy of the GNU General Public License, version 2
      along with the software. If not, see <http://www.gnu.org/licenses/>.

      The software is distributed in the hope that it will be useful, but
           WITHOUT ANY WARRANTY; without even the implied warranty of
       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
                Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public
                 License along with the software.  If not, see
                        <http://www.gnu.org/licenses/>.

                 For information regarding this software email:
                   "Joseph Coffland" <joseph@buildbotics.com>

\******************************************************************************/

#pragma once

#include "pgmspace.h"

#include <stdbool.h>


typedef enum {
  STATE_READY,
  STATE_ESTOPPED,
  STATE_RUNNING,
  STATE_JOGGING,
  STATE_STOPPING,
  STATE_HOLDING,
} state_t;


typedef enum {
  HOLD_REASON_USER_PAUSE,
  HOLD_REASON_USER_STOP,
  HOLD_REASON_PROGRAM_PAUSE,
  HOLD_REASON_OPTIONAL_PAUSE,
  HOLD_REASON_SWITCH_FOUND,
} hold_reason_t;


typedef enum {
  PAUSE_USER,
  PAUSE_PROGRAM,
  PAUSE_PROGRAM_OPTIONAL,
} pause_t;


PGM_P state_get_pgmstr(state_t state);
PGM_P state_get_hold_reason_pgmstr(hold_reason_t reason);

state_t state_get();

bool state_is_flushing();
bool state_is_resuming();

void state_seek_hold();
void state_holding();
void state_running();
void state_jogging();
void state_idle();
void state_estop();

void state_callback();
