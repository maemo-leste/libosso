/* ========================================================================= *
 * File: osso-fpu.c
 *
 * This file is part of libosso
 *
 * Copyright (C) 2009 Nokia Corporation. All rights reserved.
 *
 * Contacts: Tatu Mannisto <tatu.mannisto@nokia.com>
 *           Leonid Moiseichuk <leonid.moiseichuk@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

/* ========================================================================= *
 * Includes
 * ========================================================================= */

#include "osso-fpu.h"

/* ========================================================================= *
 * Local data.
 * ========================================================================= */

/* Current mode, IEEE by default, not thread-safe fully */
static volatile OSSO_FPU_MODE fpu_mode = OSSO_FPU_IEEE;

/* ========================================================================= *
 * Public methods.
 * ========================================================================= */

/* ------------------------------------------------------------------------- *
 * osso_fpu_get_mode -- returns current fpu mode.
 * parameters:
 *    nothing.
 * returns:
 *    see OSSO_FPU_MODE.
 * ------------------------------------------------------------------------- */
OSSO_FPU_MODE osso_fpu_get_mode(void)
{
  return fpu_mode;
} /* osso_fpu_get_mode */

/* ------------------------------------------------------------------------- *
 * osso_fpu_set_mode -- sets current fpu mode (if it differs).
 * parameters:
 *    new mode to be set.
 * returns:
 *    nothing.
 * ------------------------------------------------------------------------- */
void osso_fpu_set_mode(OSSO_FPU_MODE mode)
{
#if  defined(__arm__) && defined(__VFP_FP__) && !defined(__SOFTFP__)
  if (mode == fpu_mode)
    return;

  if (OSSO_FPU_FAST == mode)
  {
    int tmp;
    __asm__ volatile(
        "fmrx       %[tmp], fpscr\n"
        "orr        %[tmp], %[tmp], #(1 << 24)\n" /* flush-to-zero */
        "orr        %[tmp], %[tmp], #(1 << 25)\n" /* default NaN */
        "bic        %[tmp], %[tmp], #((1 << 15) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8))\n" /* clear exception bits */
        "fmxr       fpscr, %[tmp]\n"
        : [tmp] "=r" (tmp)
      );
  }
  else
  {
    int tmp;
    __asm__ volatile(
        "fmrx       %[tmp], fpscr\n"
        "bic        %[tmp], %[tmp], #(1 << 24)\n" /* flush-to-zero */
        "bic        %[tmp], %[tmp], #(1 << 25)\n" /* default NaN */
        "fmxr       fpscr, %[tmp]\n"
        : [tmp] "=r" (tmp)
      );
  }
#endif /* if __arm__ */
  fpu_mode = mode;
} /* osso_fpu_set_mode */


/* ========================================================================= *
 * main function, just for testing purposes.
 * ========================================================================= */
#ifdef UNIT_TEST

#include <stdio.h>
#include <time.h>

static float test_scalars(void)
{
  float a = 1;
  float b = 2;
  float c = 1.0 * time(NULL);
  int   i;

  for (i = 0; i < 500 * 1000 * 1000; i++)
  {
    a += c;
    b -= c;
    c = a * b;
    c /= 123;
  }

  return c;
} /* test_scalars */


static float test_vectors(void)
{
  typedef float vector_t[4];

  #define V_SZ  4
  static const vector_t v[V_SZ] =
  {
    { 1, 2, 3, 4 },
    { 5, 6, 7, 8 },
    { 1, 6, 3, 8 },
    { 5, 2, 7, 4 },
  }; /* vectors */

  float c = 1.0 * time(NULL);
  int   i;
  int   a = 0;
  int   b = 1;

  for (i = 0; i < 100 * 1000 * 1000; i++)
  {
    c += v[a][0] * v[b][0] + v[a][1] * v[b][1] + v[a][2] * v[b][2] + v[a][3] * v[b][3];
    a = (a + 1) % V_SZ;
    b = (b + 1) % V_SZ;
  }

  return c;
} /* test_vectors */


int main(const int argc, const char* argv[])
{
  time_t tm;
  int loop;

  printf ("* run scalars testing in default mode\n");
  tm = time(NULL);
  test_scalars();
  tm = time(NULL) - tm;
  printf ("=> %u seconds\n", (unsigned)tm);

  printf ("* run vectors testing in default mode\n");
  tm = time(NULL);
  test_vectors();
  tm = time(NULL) - tm;
  printf ("=> %u seconds\n", (unsigned)tm);

  for (loop = 0; loop < 3; loop++)
  {
    printf ("* run scalars testing in ieee mode\n");
    osso_fpu_set_mode(OSSO_FPU_IEEE);
    tm = time(NULL);
    test_scalars();
    tm = time(NULL) - tm;
    printf ("=> %u seconds\n", (unsigned)tm);

    printf ("* run vectors testing in ieee mode\n");
    osso_fpu_set_mode(OSSO_FPU_IEEE);
    tm = time(NULL);
    test_vectors();
    tm = time(NULL) - tm;
    printf ("=> %u seconds\n", (unsigned)tm);

    printf ("* run scalars testing in fast mode\n");
    osso_fpu_set_mode(OSSO_FPU_FAST);
    tm = time(NULL);
    test_scalars();
    tm = time(NULL) - tm;
    printf ("=> %u seconds\n", (unsigned)tm);

    printf ("* run vectors testing in fast mode\n");
    osso_fpu_set_mode(OSSO_FPU_FAST);
    tm = time(NULL);
    test_vectors();
    tm = time(NULL) - tm;
    printf ("=> %u seconds\n", (unsigned)tm);
  }

  /* That is all */
  return 0;
} /* main */

#endif /* UNIT_TEST */

