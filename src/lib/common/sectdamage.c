/*
 *  Empire - A multi-player, client/server Internet based war game.
 *  Copyright (C) 1986-2000, Dave Pare, Jeff Bailey, Thomas Ruschak,
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
 *  sectdamage.c: Damage a sector
 * 
 *  Known contributors to this file:
 *     Dave Pare, 1989
 *     Steve McClure, 1996
 */

#include "misc.h"
#include "var.h"
#include "sect.h"
#include "ship.h"
#include "land.h"
#include "plane.h"
#include "nuke.h"
#include "xy.h"
#include "nsc.h"
#include "file.h"
#include "nat.h"
#include "optlist.h"
#include "damage.h"
#include "common.h"
#include "gen.h"
#include "subs.h"
#include "lost.h"
#include "combat.h"

int
sect_damage(struct sctstr *sp, int dam, struct emp_qelem *list)
{
    int eff;

    if (dam <= 0)
	return 0;
    if (dam > 100)
	dam = 100;

    sp->sct_effic = damage((int)sp->sct_effic, dam);
    sp->sct_road = damage((int)sp->sct_road, dam);
    sp->sct_rail = damage((int)sp->sct_rail, dam);
    sp->sct_defense = damage((int)sp->sct_defense, dam);
    if (!opt_DEFENSE_INFRA)
	sp->sct_defense = sp->sct_effic;

    eff = dam;

    if (sp->sct_mobil > 0)
	sp->sct_mobil = damage((int)sp->sct_mobil, dam);
    sp->sct_nv = vl_damage(dam,
			   sp->sct_vtype, sp->sct_vamt, (int)sp->sct_nv);
    if (opt_EASY_BRIDGES == 0) {
	if (sp->sct_effic < 20 && sp->sct_type == SCT_BHEAD)
	    bridgefall(sp, list);
    } else {
	if (sp->sct_effic < 20 && sp->sct_type == SCT_BSPAN)
	    knockdown(sp, list);
    }
    putsect(sp);
    return eff;
}

int
sectdamage(struct sctstr *sp, int dam, struct emp_qelem *list)
{
    extern double unit_damage;
    struct nstr_item ni;
    struct lndstr land;
    struct plnstr plane;
    double real_dam;
    int eff;

    /* Some sectors are harder/easier to kill..   */
    /* Average sector has a dstr of 1, so adjust  */
    /* the damage accordingly. Makes forts a pain */

/*	real_dam = (double)dam * (1.0/((((double)(dchr[sp->sct_type].d_dstr - 1))*(sp->sct_effic/100.0)) + 1.0));*/
    real_dam = (double)dam *(1.0 / sector_strength(sp));
    dam = ldround(real_dam, 1);

    eff = sect_damage(sp, PERCENT_DAMAGE(dam), list);

    /* Damage all the land units in the sector */
    /* Units don't take full damage */
    dam = ldround(DPERCENT_DAMAGE(dam * unit_damage), 1);
    if (dam <= 0)
	return eff;

    snxtitem_xy(&ni, EF_LAND, sp->sct_x, sp->sct_y);
    while (nxtitem(&ni, (s_char *)&land)) {
	if (!land.lnd_own)
	    continue;
	landdamage(&land, dam);
	putland(land.lnd_uid, &land);
    }

    dam = dam / 7;
    if (dam <= 0)
	return eff;
    snxtitem_xy(&ni, EF_PLANE, sp->sct_x, sp->sct_y);
    while (nxtitem(&ni, (s_char *)&plane)) {
	if (!plane.pln_own)
	    continue;
	if (plane.pln_flags & PLN_LAUNCHED)
	    continue;
	if (plane.pln_ship >= 0)
	    continue;
	/* Is this plane flying in this list? */
	if (ac_isflying(&plane, list))
	    continue;
	planedamage(&plane, dam);
	putplane(plane.pln_uid, &plane);
    }
    return eff;
}
