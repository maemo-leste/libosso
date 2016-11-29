/**
 * This file is part of libosso
 *
 * Copyright (C) 2005-2006 Nokia Corporation. All rights reserved.
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

#include <libosso.h>
#include <stdio.h>
#include <unistd.h>

#define APP_NAME "test_hw"
#define APP_VER "0.0.1"
#define TESTFILE "/tmp/hwsignal"

void hw_cb(osso_hw_state_t *state, gpointer data);

int main(int argc, char **argv)
{
    GMainLoop *loop;
    osso_context_t *osso;

    loop = g_main_loop_new(NULL, FALSE);

    osso = osso_initialize(APP_NAME, APP_VER, TRUE, NULL);
    if(osso == NULL) {
	return 1;
    }

    osso_hw_set_event_cb(osso, NULL, hw_cb, loop);

    g_main_loop_run(loop);

    osso_hw_unset_event_cb(osso, NULL);
    osso_deinitialize(osso);
    return 0;
}


void hw_cb(osso_hw_state_t *state, gpointer data)
{
    GMainLoop *loop;
    FILE *f;
    
    loop = (GMainLoop *)data;

    f = fopen(TESTFILE, "w");
    if(f == NULL) {
	g_debug("unable to open file %s", TESTFILE);
    }
    else {
	if(state->shutdown_ind) {
	    fprintf(f,"shutdown_ind\n");
	    g_debug("shutdown_ind");
	}
	if(state->memory_low_ind) {
	    fprintf(f,"memlow\n");
	    g_debug("memlow");
	}
	if(state->save_unsaved_data_ind) {
	    fprintf(f,"save_unsaved_data_ind\n");
	    g_debug("save_unsaved_data_ind");
	}
	if(state->system_inactivity_ind) {
	    fprintf(f,"minact\n");
	    g_debug("minact");
	}
	if(state->sig_device_mode_ind) {
	    fprintf(f,"device_mode_ind\n");
	    g_debug("device_mode_ind");
	}
	fclose(f);
	sync();
    }
}
