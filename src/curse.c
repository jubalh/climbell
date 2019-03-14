/*
 * vim: noexpandtab:ts=4:sts=4:sw=4
 *
 * Copyright (C) 2019 Michael Vetter <jubalh@iodoru.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <ncurses.h>
#include <stdlib.h>
#include <gmodule.h>

static GList *orig_list;
static GList *lp_list;
static int base_lang_x, base_lang_y;
static WINDOW *win;

static void cleanup(void)
{
	endwin();
}

void init_curses()
{
	initscr();
	clear();
	atexit(cleanup);
	cbreak();
	noecho();

	keypad(stdscr, true);

	refresh();
}

void init_create_course(GList *lplist)
{
	orig_list= lplist;
	lp_list = lplist;

	win = newwin(20, 20, 0, 5);
}

void draw_create_course()
{
	wmove(win, 0, 0);
	wprintw(win, "climbell\n\n");
	wprintw(win, "Create a Course\n");
	wprintw(win, "Base language:");
	getyx(win, base_lang_y, base_lang_x);
	wprintw(win, "\nTarget language:\n");
	wmove(win, base_lang_y, base_lang_x + 1);
	wprintw(win, "%s", (char*)lp_list->data);
	wmove(win, base_lang_y, base_lang_x + 1);

	refresh();
	wrefresh(win);
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
