/*
 *  Empire - A multi-player, client/server Internet based war game.
 *  Copyright (C) 1986-2007, Dave Pare, Jeff Bailey, Thomas Ruschak,
 *                           Ken Stevens, Steve McClure
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  ---
 *
 *  See files README, COPYING and CREDITS in the root of the source
 *  tree for related information and legal notices.  It is expected
 *  that future projects/authors will amend these files as needed.
 *
 *  ---
 *
 *  unit.h: Generalize unit data structures and functions.
 * 
 *  Known contributors to this file:
 *     Ron Koenderink, 2006-2007
 *     Markus Armbruster, 2006
 */
 
#include "empobj.h"

struct ulist {
    struct emp_qelem queue;	/* list of units */
    double mobil;		/* how much mobility the unit has left */
    struct empobj_chr *chrp;	/* pointer to characteristics unit */
    union empobj_storage unit;	/* unit */
    coord x, y;			/* x,y it came from LAND only */
    int eff;			/* LAND only */
    int supplied;		/* LAND only */
};

extern void unit_list(struct emp_qelem *);
extern void unit_put(struct emp_qelem *list, natid actor);
extern char *unit_path(int, struct empobj *, char *);
extern void unit_view(struct emp_qelem *);