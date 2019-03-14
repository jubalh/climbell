/*
 * vim: noexpandtab:ts=4:sts=4:sw=4
 *
 * Copyright (C) 2019 Michael Vetter <jubalh@iodoru.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <ncurses.h>
#include <stdlib.h>

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
