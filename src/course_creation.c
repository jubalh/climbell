/*
 * vim: noexpandtab:ts=4:sts=4:sw=4
 *
 * Copyright (C) 2019 Michael Vetter <jubalh@iodoru.org>
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gmodule.h>
#include <ncurses.h>

#define AF_MAX 2
enum ActiveField {
	AF_BL = 0,
	AF_TL
};

struct s_lp_field {
	int x, y;
	char *content;
	int index;
};

struct s_course_creation_win {
	WINDOW *window;
	struct s_lp_field base;
	struct s_lp_field target;
	enum ActiveField af;
	int lp_length;
};

static struct s_course_creation_win cc_win;
static GList *orig_list;
static GList *lp_list;

static void next_af(enum ActiveField *af) {
	*af += 1;
	if (*af >= AF_MAX)
		*af = 0;
}

int init_create_course(GList *lplist)
{
	if (lplist == NULL) {
		goto err;
	}

	cc_win.lp_length = g_list_length(lplist);
	if (cc_win.lp_length < 2) {
		goto err;
	}

	orig_list = lplist;
	lp_list = lplist;

	cc_win.window = newwin(30, 30, 0, 5);
	cc_win.af = AF_BL;
	cc_win.base.content = g_list_nth_data(lplist, 0);
	cc_win.target.content = g_list_nth_data(lplist, 1);

	return 0;

err:
	fprintf (stderr, "You need to have at least two language packs installed!\n");
	return 1;
}

void draw_create_course()
{
	wmove(cc_win.window, 0, 0);
	wprintw(cc_win.window, "climbell\n\n");
	wprintw(cc_win.window, "Create a Course\n");

	wprintw(cc_win.window, "Base language: ");
	getyx(cc_win.window, cc_win.base.y, cc_win.base.x);
	wprintw(cc_win.window, "\nTarget language: ");
	getyx(cc_win.window, cc_win.target.y, cc_win.target.x);

	wmove(cc_win.window, cc_win.base.y, cc_win.base.x);
	wprintw(cc_win.window, "%s", cc_win.base.content);

	wmove(cc_win.window, cc_win.target.y, cc_win.target.x);
	wprintw(cc_win.window, "%s", cc_win.target.content);

	if (cc_win.af == AF_BL) {
		wmove(cc_win.window, cc_win.base.y, cc_win.base.x);
	} else if (cc_win.af == AF_TL) {
		wmove(cc_win.window, cc_win.target.y, cc_win.target.x);
	}

	refresh();
	wrefresh(cc_win.window);
}

void handle_create_course(int key)
{
	if (key == KEY_RIGHT) {
		if (cc_win.af == AF_BL) {
			cc_win.base.index += 1;
			if (cc_win.base.index >= cc_win.lp_length) {
				cc_win.base.index = 0;
			}

			cc_win.base.content = g_list_nth_data(lp_list, cc_win.base.index);
		} else if (cc_win.af == AF_TL) {
			cc_win.target.index += 1;
			if (cc_win.target.index >= cc_win.lp_length) {
				cc_win.target.index = 0;
			}

			cc_win.target.content = g_list_nth_data(lp_list, cc_win.target.index);
		}
	} else if (key == '\t') {
		next_af(&cc_win.af);
	}
}
