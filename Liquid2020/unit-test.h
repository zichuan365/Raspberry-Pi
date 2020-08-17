/* tests/unit-test.h.  Generated from unit-test.h.in by configure.  */
/*
 * Copyright © 2008-2011 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_

/* Constants defined by configure.ac */
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#ifdef HAVE_STDINT_H
# ifndef _MSC_VER
# include <stdint.h>
# else
# include "stdint.h"
# endif
#endif

#define SERVER_ID         17
#define INVALID_SERVER_ID 18

//线圈（一个线圈，2个字节0xFF）
const uint16_t UT_BITS_ADDRESS = 0x00;
const uint16_t UT_BITS_NB = 0x04;
const uint8_t UT_BITS_TAB[] = { 0x01, 0x02, 0x03, 0x04 };
//input线圈
const uint16_t UT_INPUT_BITS_ADDRESS = 0x04;
const uint16_t UT_INPUT_BITS_NB = 0x04;
const uint8_t UT_INPUT_BITS_TAB[] = { 0x01, 0x02, 0x03, 0x04};

//寄存器(一个寄存器，4个字节0xFFFF，高位+低位)
const uint16_t UT_REGISTERS_ADDRESS = 0x00;
/* Raise a manual exception when this adress is used for the first byte */
const uint16_t UT_REGISTERS_ADDRESS_SPECIAL = 0x0F;
const uint16_t UT_REGISTERS_NB = 0x04;
const uint16_t UT_REGISTERS_TAB[] = { 0x1111,0x0002, 0x0003, 0x0004 };
/* If the following value is used, a bad response is sent.
   It's better to test with a lower value than
   UT_REGISTERS_NB_POINTS to try to raise a segfault. */
const uint16_t UT_REGISTERS_NB_SPECIAL = 0x2;



//input寄存器
const uint16_t UT_INPUT_REGISTERS_ADDRESS = 0x00;
const uint16_t UT_INPUT_REGISTERS_NB = 0x01;
const uint16_t UT_INPUT_REGISTERS_TAB[] = { 0x000A };

const float UT_REAL = 916.540649;
const uint32_t UT_IREAL = 0x4465229a;

#endif /* _UNIT_TEST_H_ */
