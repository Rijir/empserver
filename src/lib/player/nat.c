/*
 *  Empire - A multi-player, client/server Internet based war game.
 *  Copyright (C) 1986-2004, Dave Pare, Jeff Bailey, Thomas Ruschak,
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
 *  See the "LEGAL", "LICENSE", "CREDITS" and "README" files for all the
 *  related information and legal notices. It is expected that any future
 *  projects/authors will amend these files as needed.
 *
 *  ---
 *
 *  nat.c: Get nation stuff
 * 
 *  Known contributors to this file:
 *     Dave Pare, 1994
 */

#include "prototypes.h"
#include "misc.h"
#include "nat.h"
#include "file.h"
#include "player.h"
#include <fcntl.h>

int
natbyname(s_char *name, natid *result)
{
    struct natstr *np;
    int i;

    for (i = 0; NULL != (np = getnatp(i)); i++) {
	if (strcmp(np->nat_cnam, name) == 0) {
	    *result = i;
	    return 0;
	}
    }
    *result = 255;
    return -1;
}

int
natpass(int cn, s_char *pass)
{
    struct natstr *np;

    np = getnatp((natid)cn);
    if (np->nat_stat == VIS)
	return 1;
    if (strcmp(np->nat_pnam, pass) == 0)
	return 1;
    return 0;
}
