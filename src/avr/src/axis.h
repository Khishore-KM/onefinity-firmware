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

#include "config.h"

#include <stdbool.h>


enum {
  AXIS_X, AXIS_Y, AXIS_Z,
  AXIS_A, AXIS_B, AXIS_C,
};


bool axis_is_enabled(int axis);
int axis_get_id(char axis);
int axis_get_motor(int axis);
bool axis_get_homed(int axis);
float axis_get_soft_limit(int axis, bool min);
void axis_map_motors();

float axis_get_velocity_max(int axis);
float axis_get_accel_max(int axis);
float axis_get_jerk_max(int axis);
