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
 *  land.h: Definitions for land units
 * 
 *  Known contributors to this file:
 *     Thomas Rushack, 1992
 *     Ken Stevens, 1995
 *     Steve McClure, 1998
 */

#ifndef _LAND_H_
#define _LAND_H_

#include "sect.h"
#include "ship.h"
#include "queue.h"
#include "nsc.h"
#include "retreat.h"

#define LAND_MINEFF	10
#define LAND_MINFIREEFF 40	/* arty must be this effic to fire */
#define MAXLNDV		14
#define LND_MINMOBCOST  0.200

struct lndstr {
    short ef_type;
    natid lnd_own;		/* owner's country num */
    short lnd_uid;		/* unit id (land unit) */
    coord lnd_x;		/* x location in abs coords */
    coord lnd_y;		/* y location in abs coords */
    s_char lnd_type;		/* ship type */
    s_char lnd_effic;		/* 0% to 100% */
    s_char lnd_mobil;		/* mobility units made int for RS/6000 */
    short lnd_sell;		/* pointer to trade file */
    short lnd_tech;		/* tech level ship was built at */
    s_char lnd_army;		/* group membership */
    coord lnd_opx, lnd_opy;	/* Op sector coords */
    short lnd_mission;		/* mission code */
    short lnd_radius;		/* mission radius */
    s_char lnd_flags;		/* unit flags */
    short lnd_ship;		/* pointer to transporting ship */
    s_char lnd_harden;		/* for missiles */
    short lnd_retreat;		/* retreat percentage */
    u_char lnd_fuel;		/* How much fuel do we have */
    u_char lnd_nxlight;		/* How many xlight planes on board? */
    int lnd_rflags;		/* When do I retreat? */
    s_char lnd_rpath[RET_LEN];	/* retreat path */
    u_char lnd_rad_max;		/* max radius for this unit */
    u_char lnd_scar;		/* how experienced the unit is (not used) */
    s_char lnd_nv;		/* current number of variables */
    u_char lnd_vtype[MAXLNDV];
    u_short lnd_vamt[MAXLNDV];
    short lnd_land;		/* pointer to transporting unit */
    u_char lnd_nland;
    time_t lnd_access;		/* Last time mob was updated (MOB_ACCESS) */
    float lnd_att;		/* attack multiplier */
    float lnd_def;		/* defense multiplier */
    int lnd_vul;		/* vulnerability (0-100) */
    int lnd_spd;		/* speed */
    int lnd_vis;		/* visibility */
    int lnd_spy;		/* Seeing distance */
    int lnd_rad;		/* reaction radius */
    int lnd_frg;		/* firing range */
    int lnd_acc;		/* firing accuracy */
    int lnd_dam;		/* # of guns firing */
    int lnd_ammo;		/* firing ammu used per shot */
    int lnd_aaf;		/* aa fire */
    u_char lnd_fuelc;		/* fuel capacity */
    u_char lnd_fuelu;		/* fuel used per 10 mob */
    u_char lnd_maxlight;	/* maximum number of xlight planes */
    u_char lnd_maxland;		/* maximum number of units */
    time_t lnd_timestamp;	/* Last time this unit was touched */
};

#define LND_NOTANY bit(0)	/* Just a placeholder, not used */

struct lchrstr {
    u_char l_nv;		/* number of variables it can hold */
    u_char l_vtype[MAXCHRNV];
    u_short l_vamt[MAXCHRNV];
    s_char *l_name;		/* full name of type of land unit */
    int l_lcm;			/* units of lcm to build */
    int l_hcm;			/* units of hcm to build */
    int l_mil;			/* how many mil it takes to build */
    int l_gun;			/* how many guns it takes to build */
    int l_shell;		/* how many shells it takes to build */
    int l_tech;			/* tech required to build */
    int l_cost;			/* how much it costs to build */
    float l_att;		/* attack multiplier */
    float l_def;		/* defense multiplier */
    int l_vul;			/* vulnerability (0-100) */
    int l_spd;			/* speed */
    int l_vis;			/* visibility */
    int l_spy;			/* Seeing distance */
    int l_rad;			/* reaction radius */
    int l_frg;			/* firing range */
    int l_acc;			/* firing accuracy */
    int l_dam;			/* # of guns firing */
    int l_ammo;			/* firing ammu used per shot */
    int l_aaf;			/* aa fire */
    u_char l_fuelc;		/* fuel capacity */
    u_char l_fuelu;		/* fuel used per 10 mob */
    u_char l_nxlight;		/* maximum number of xlight planes */
    u_char l_mxland;		/* maximum number of units */
    long l_flags;		/* what special things can this unit do */
};

/* Land unit ability flags */
#define	L_XLIGHT	bit(0)	/* Hold xlight planes */
#define	L_ENGINEER	bit(1)	/* Do engineering things */
#define	L_SUPPLY	bit(2)	/* supply other units/sects */
#define	L_SECURITY	bit(3)	/* anti-terrorist troops */
#define	L_LIGHT		bit(4)	/* can go on ships */
#define	L_MARINE	bit(5)	/* marine units, good at assaulting */
#define	L_RECON		bit(6)	/* recon units, good at spying */
#define L_RADAR		bit(7)	/* radar unit */
#define L_ASSAULT	bit(8)	/* can assault */
#define L_FLAK		bit(9)	/* flak unit */
#define L_SPY           bit(10)	/* spy unit - way cool */
#define L_TRAIN         bit(11)	/* train unit - neato */
#define L_HEAVY         bit(12)	/* heavy unit - can't go on trains */

#define LND_ATTDEF(b, t) (((b) * (1.0 + ((sqrt((double)(t)) / 100.0) * 4.0))) \
						  > 127 ? 127 : \
						  ((b) * (1.0 + ((sqrt((double)(t)) / 100.0) * 4.0))))
#define LND_SPD(b, t) ((b * (1.0 + ((sqrt((double)t) / 100.0) * 2.1))) > 127\
		       ? 127 : (b * (1.0 + ((sqrt((double)t) / 100.0) * 2.1))))
#define LND_VUL(b, t) ((b * (1.0 - ((sqrt((double)t) / 100.0) * 1.1))) < 0\
		       ? 0 : (b * (1.0 - ((sqrt((double)t) / 100.0) * 1.1))))
#define LND_VIS(b, t) (b)
#define LND_SPY(b, t) (b)
#define LND_RAD(b, t) (b)
#define LND_FRG(b, t) ((t) ? \
					   ((b) * (logx((double)(t), (double)35.0) < 1.0 ? 1.0 : \
							   logx((double)(t), (double)35.0))) : (b))
#define LND_DAM(b, t) ((t) ? \
					   ((b) * (logx((double)(t), (double)60.0) < 1.0 ? 1.0 : \
							   logx((double)(t), (double)60.0))) : (b))
#define LND_ACC(b, t) ((b * (1.0 - ((sqrt((double)t) / 100.0) * 1.1))) < 0\
		       ? 0 : (b * (1.0 - ((sqrt((double)t) / 100.0) * 1.1))))
#define LND_AMM(b, d, t) ((b) ? ((LND_DAM((d), (t)) / 2) + 1) : 0)
#define LND_AAF(b, t) ((b * (1.0 + ((sqrt((double)t) / 100.0) * 3.0))) > 127\
		       ? 127 : (b * (1.0 + ((sqrt((double)t) / 100.0) * 3.0))))
#define LND_FC(b, t)  (b)
#define LND_FU(b, t)  (b)
#define LND_XPL(b, t) (b)
#define LND_MXL(b, t) (b)
#define LND_COST(b, t) ((b) * (1.0 + (sqrt((double)(t)) / 100.0)))

/* Chance to detect L_SPY unit (percent) */
#define LND_SPY_DETECT_CHANCE(eff) ((110-(eff))/100.0)

#define getland(n, p) \
	ef_read(EF_LAND, n, (caddr_t)p)
#define putland(n, p) \
	ef_write(EF_LAND, n, (caddr_t)p)
#define getlandp(n) \
	(struct lndstr *) ef_ptr(EF_LAND, n)

extern struct lchrstr lchr[];
extern int lnd_maxno;

struct llist {
    struct emp_qelem queue;	/* list of units */
    coord x, y;			/* x,y it came from */
    struct lchrstr *lcp;	/* pointer to desc of land unit */
    struct lndstr land;		/* struct land unit */
    int eff;
    double mobil;
    int supplied;
};

/* src/lib/subs/lndsub.c */
extern void lnd_sweep(struct emp_qelem *, int, int, natid);
extern int lnd_interdict(struct emp_qelem *, coord, coord, natid);
extern void lnd_sel(struct nstr_item *, struct emp_qelem *);
extern void lnd_mess(s_char *, struct llist *);
extern int lnd_check_mines(struct emp_qelem *);
extern double lnd_mobcost(struct lndstr *, struct sctstr *, int);
extern s_char *lnd_path(int, struct lndstr *, s_char *);

extern int attack_val(int, struct lndstr *);
extern int total_mil(struct lndstr *);
extern int defense_val(struct lndstr *);
extern int lnd_getmil(struct lndstr *);
extern void lnd_print(struct llist *, s_char *);
extern void lnd_delete(struct llist *, s_char *);
extern int lnd_take_casualty(int, struct llist *, int);
extern void lnd_submil(struct lndstr *, int);
extern void lnd_takemob(struct emp_qelem *, double);
extern int lnd_spyval(struct lndstr *);
extern int intelligence_report(int, struct lndstr *, int, s_char *);
extern int count_sect_units(struct sctstr *);
extern void count_units(struct shpstr *);
extern void lnd_count_units(struct lndstr *);
extern void lnd_mar(struct emp_qelem *, double *, double *, int *, natid);
extern void lnd_put(struct emp_qelem *, natid);
extern int lnd_hit_mine(struct lndstr *, struct lchrstr *);
extern void lnd_list(struct emp_qelem *);
extern int lnd_hardtarget(struct lndstr *);
extern int lnd_mar_one_sector(struct emp_qelem *, int, natid, int);
extern int lnd_support(natid, natid, coord, coord);
extern int lnd_can_attack(struct lndstr *);
extern int lnd_fortify (struct lndstr *lp, int hard_amt);

void landdamage();
void lnd_nav();
int lnd_check_nav();
double sqrt();
s_char *prland();

#endif /* _LAND_H_ */
