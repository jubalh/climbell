/*
 * vim: noexpandtab:ts=4:sts=4:sw=4
 *
 * Copyright (C) 2019 Michael Vetter <jubalh@iodoru.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <glib.h>

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
	if (g_access(language_pack, 0x04) < 0) {
		g_print("File %s does not exist or not readable\n", language_pack);
		return 1;
	}

	g_print("Installing %s...\n", language_pack);
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
    } else if (language_pack != NULL) {
		return install_language_pack(language_pack);
	}
}
