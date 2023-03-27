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

#include "config.h"
#include "stepper.h"
#include "command.h"
#include "vars.h"
#include "base64.h"
#include "hardware.h"
#include "report.h"
#include "exec.h"

#include <stdio.h>


stat_t command_dwell(char *cmd) {
  float seconds;
  if (!b64_decode_float(cmd + 1, &seconds)) return STAT_BAD_FLOAT;
  command_push(*cmd, &seconds);
  return STAT_OK;
}


static stat_t _dwell_exec() {
  exec_set_cb(0); // Immediately clear the callback
  return STAT_OK;
}


unsigned command_dwell_size() {return sizeof(float);}


void command_dwell_exec(void *seconds) {
  st_prep_dwell(*(float *)seconds);
  exec_set_cb(_dwell_exec); // Command must set an exec callback
}


stat_t command_help(char *cmd) {
  printf_P(PSTR("\n{\"commands\":{"));
  command_print_json();
  printf_P(PSTR("},\"variables\":{"));
  vars_print_json();
  printf_P(PSTR("}}\n"));

  return STAT_OK;
}


stat_t command_reboot(char *cmd) {
  hw_request_hard_reset();
  return STAT_OK;
}


stat_t command_dump(char *cmd) {
  report_request_full();
  return STAT_OK;
}
