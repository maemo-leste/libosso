/**
 * This file is part of libosso
 *
 * Copyright (C) 2009 Nokia Corporation. All rights reserved.
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

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libosso.h>
#include <dbus/dbus.h>

#include "../../src/osso-internal.h"

#define APP_NAME "test_osso_init"
#define APP_VER "0.0.1"
#define TESTFILE "/tmp/"APP_NAME".tmp"

void hw_cb(osso_hw_state_t *state, gpointer data)
{
    printf("%s: entered\n", __FUNCTION__);
}

int main(int nargs, char *argv[])
{
    osso_context_t *osso;
    FILE *f;
    DBusConnection *ses_conn, *sys_conn;
    DBusError err;
    osso_rpc_t retval;

    f = fopen(TESTFILE, "w");

    dbus_error_init(&err);
    ses_conn = dbus_bus_get_private(DBUS_BUS_SESSION, &err);
    if (ses_conn == NULL) {
        printf("couldn't connect to session bus\n");
        return 1;
    }
    dbus_connection_setup_with_g_main(ses_conn, NULL);

    sys_conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (sys_conn == NULL) {
        printf("couldn't connect to system bus\n");
        return 1;
    }
    dbus_connection_setup_with_g_main(sys_conn, NULL);
    
    osso = osso_initialize_with_connections(APP_NAME, APP_VER, sys_conn,
                                            ses_conn);
    fprintf(f, "osso = %p\n",osso);
    fflush(f);
    if(osso == NULL)
	return 1;

    osso_hw_set_event_cb(osso, NULL, hw_cb, NULL);

    osso_rpc_run_system (osso, "com.nokia.mce",
                  "/com/nokia/mce/request",
                  "com.nokia.mce.request", "get_device_mode",
                  &retval, DBUS_TYPE_INVALID, NULL);
    printf("mce returned %s\n", retval.value.s);
 
    fprintf(f, "osso->application = %s\n",osso->application);
    fflush(f);
    if(strcmp(osso->application, APP_NAME)!=0)
	return 1;
    fprintf(f, "osso->application = %s\n",osso->application);
    fflush(f);

    if(strcmp(osso->version, APP_VER)!=0)
	return 1;    
    fprintf(f, "osso->version = %s\n",osso->version);
    fflush(f);

    if(osso->object_path == NULL)
	return 1;
    fprintf(f, "object_path = '%s'\n",osso->object_path);
    
    fflush(f);
    fclose(f);

    /*
    osso_deinitialize(osso);
    */
    g_main_loop_run(g_main_loop_new (NULL, TRUE));
    
    return 0;
}
