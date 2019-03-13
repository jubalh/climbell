/*
 * vim: noexpandtab:ts=4:sts=4:sw=4
 *
 * Copyright (C) 2019 Michael Vetter <jubalh@iodoru.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <glib.h>
#include <glib/gstdio.h>
#include <gmodule.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <ncurses.h>
#include "curse.h"

#define VERSION "0.0"

static gboolean version = FALSE;
static char *language_pack = NULL;

void show_version()
{
#ifndef GIT_REV
        g_print("climbell, version %s\n", VERSION);
#else
        g_print("climbell, version %s~%s\n", VERSION, GIT_REV);
#endif

        g_print("Copyright (C) 2019 Michael Vetter <jubalh@iodoru.org>.\n");
        g_print("License GPLv3+: GNU GPL version 3 or later <https://www.gnu.org/licenses/gpl.html>\n");
        g_print("\n");
        g_print("This is free software; you are free to change and redistribute it.\n");
        g_print("There is NO WARRANTY, to the extent permitted by law.\n");
}

int install_language_pack(const char *language_pack)
{
	if (g_access(language_pack, R_OK) < 0) {
		g_print("File %s does not exist or not readable\n", language_pack);
		return 1;
	}

	g_print("Installing %s...\n", language_pack);

	gchar *ending;
	ending = g_strstr_len(language_pack, -1, ".gls");

	if (!ending) {
		g_print("File %s is not a valid language pack\n", language_pack);
		return 1;
	}

	*ending = '\0';

	gchar *start = (gchar*)language_pack;
	gchar *sep = g_strrstr(language_pack, "/");
	if (sep) {
		start = sep + 1;
	}

	gchar *lp = g_strdup(start);
	*ending = '.';

	g_print("Language Pack: %s\n", lp);

	const gchar *datadir = g_get_user_data_dir();
	if (!datadir) {
		g_free(lp);
		fprintf (stderr, "Unable to get data dir\n");
		return 1;
	}

	gchar *lp_dir = g_strconcat(datadir, "/climbell", "/language_packs/", lp, NULL);
	g_print("Directory: %s\n", lp_dir);

	// when directory already exists, treat as LP already installed
	if (g_access(lp_dir, R_OK) >= 0) {
		fprintf(stderr, "Aborting: %s already installed\n", lp);
		g_free(lp_dir);
		g_free(lp);
		return 1;
	}

	if (g_mkdir_with_parents(lp_dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) < 0) {
		fprintf (stderr, "Unable to create %s\n", lp_dir);
		g_free(lp_dir);
		g_free(lp);
		return 1;
	}

	// TODO: make sure unzip is available
	gchar *zipcall = g_strconcat("unzip ", language_pack, " -d ", lp_dir, NULL);
	int ret_zip = system(zipcall);
	if (ret_zip != 0) {
		fprintf (stderr, "Unable to unzip %s\n", language_pack);
		g_free(zipcall);
		g_free(lp_dir);
		g_free(lp);
		return 1;
	}
	g_free(zipcall);

	g_free(lp_dir);

	g_free(lp);
	return 0;
}

int main(int argc, char **argv)
{
    static GOptionEntry entries[] =
    {
        { "version", 'v', 0, G_OPTION_ARG_NONE, &version, "Show version information", NULL },
        { "install", 'i', 0, G_OPTION_ARG_STRING, &language_pack, "Install new Language pack (.gls)" },
        { NULL }
    };

    GError *error = NULL;
    GOptionContext *context;

    context = g_option_context_new(NULL);
    g_option_context_add_main_entries(context, entries, NULL);
    if (!g_option_context_parse(context, &argc, &argv, &error)) {
        g_print("%s\n", error->message);
        g_option_context_free(context);
        g_error_free(error);
        return 1;
    }

    g_option_context_free(context);

    if (version == TRUE)
    {
		show_version();
		return 0;
    } else if (language_pack != NULL) {
		return install_language_pack(language_pack);
	}

	init_curses();
	show_create_course();
}
