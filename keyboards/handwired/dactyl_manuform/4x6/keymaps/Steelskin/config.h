/*
This is the c configuration file for the keymap

Copyright 2021 Fabrice 'Steelskin' de Gans <steelskin@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// Layers.
enum _layer {
  _BASE_WIN,
  _BASE_MAC,
  _BASE,
  _NUMS,
  _NUMPAD,
  _FKEYS
};

#define COMBO_ONLY_FROM_LAYER _BASE

/* Use I2C or Serial, not both */
#define USE_SERIAL
// #define USE_I2C

/* Select hand configuration */
#define MASTER_LEFT
// #define MASTER_RIGHT

// #define EE_HANDS
