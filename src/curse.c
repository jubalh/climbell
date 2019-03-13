/*
 * vim: noexpandtab:ts=4:sts=4:sw=4
 *
 * Copyright (C) 2019 Michael Vetter <jubalh@iodoru.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <ncurses.h>
#include <stdlib.h>
#include <gmodule.h>

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

void show_create_course(GList *lp_list)
{
	int base_lang_x, base_lang_y;

	WINDOW *win = newwin(20, 20, 0, 5);

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

	getch();
}

