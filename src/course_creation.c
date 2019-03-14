/*
 * vim: noexpandtab:ts=4:sts=4:sw=4
 *
 * Copyright (C) 2019 Michael Vetter <jubalh@iodoru.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gmodule.h>
#include <ncurses.h>

struct s_course_creation_win {
	WINDOW *window;
	int base_lang_x, base_lang_y;
};

static struct s_course_creation_win cc_win;
static GList *orig_list;
static GList *lp_list;

void init_create_course(GList *lplist)
{
	orig_list= lplist;
	lp_list = lplist;

	cc_win.window = newwin(20, 20, 0, 5);
}

void draw_create_course()
{
	wmove(cc_win.window, 0, 0);
	wprintw(cc_win.window, "climbell\n\n");
	wprintw(cc_win.window, "Create a Course\n");
	wprintw(cc_win.window, "Base language:");
	getyx(cc_win.window, cc_win.base_lang_y, cc_win.base_lang_x);
	wprintw(cc_win.window, "\nTarget language:\n");
	wmove(cc_win.window, cc_win.base_lang_y, cc_win.base_lang_x + 1);
	wprintw(cc_win.window, "%s", (char*)lp_list->data);
	wmove(cc_win.window, cc_win.base_lang_y, cc_win.base_lang_x + 1);

	refresh();
	wrefresh(cc_win.window);
}

void handle_create_course(int key)
{
	if (key == KEY_UP) {
		lp_list = g_list_next(lp_list);
		if (lp_list == NULL) {
			lp_list = orig_list;
		}
	}
}