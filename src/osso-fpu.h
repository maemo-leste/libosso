/* ========================================================================= *
 * File: osso-fpu.h
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

#ifndef OSSO_FPU_H
#define OSSO_FPU_H

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================= *
 * Definitions.
 * ========================================================================= */

/* ------------------------------------------------------------------------- *
 * FPU mode, could be IEEE compliant or fast. In fast mode some simplifications
 * are made by hardware about corner modes but it minor incompatibility will
 * allow us to run faster (30% for ARM).
 *
 * Application can switch on and off modes in runtime.
 * ------------------------------------------------------------------------- */

typedef enum
{
  OSSO_FPU_IEEE,    /* Usual processor mode, slow and accurate */
  OSSO_FPU_FAST     /* Fast but a bit non-accurate mode        */
} OSSO_FPU_MODE;


/* ========================================================================= *
 * Methods.
 * ========================================================================= */

/* ------------------------------------------------------------------------- *
 * osso_fpu_get_mode -- returns current fpu mode.
 * parameters:
 *    nothing.
 * returns:
 *    see OSSO_FPU_MODE.
 * ------------------------------------------------------------------------- */
OSSO_FPU_MODE osso_fpu_get_mode(void);

/* ------------------------------------------------------------------------- *
 * osso_fpu_set_mode -- sets current fpu mode (if it differs).
 * parameters:
 *    new mode to be set.
 * returns:
 *    nothing.
 * ------------------------------------------------------------------------- */
void osso_fpu_set_mode(OSSO_FPU_MODE mode);

#ifdef __cplusplus
}
#endif

#endif /* OSSO_MEM_H */
