/*
 *  Empire - A multi-player, client/server Internet based war game.
 *  Copyright (C) 1986-2005, Dave Pare, Jeff Bailey, Thomas Ruschak,
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
 *  file.h: Describes Empire tables (`files' for historical reasons)
 * 
 *  Known contributors to this file:
 *     Markus Armbruster, 2005
 */

#ifndef _FILE_H_
#define _FILE_H_

#include <stddef.h>

struct empfile {
    int ef_uid;			/* Table ID */
    char *name;			/* Empire name (e.g., "treaty") */
    char *file;			/* if backed by file, file name relative to
				   data directory */
    int flags;			/* EFF_XY, ... */
    int size;			/* size of a table entry */
    void (*init)(int, char *);	/* called after entry creation, unless null */
    int (*postread)(int, char *); /* called after read, unless null */
    int (*prewrite)(int, char *); /* called before write, unless null */
    int fd;			/* file descriptor, -1 if not open */
    int baseid;			/* id of first entry in cache */
    int cids;			/* # entries in cache */
    int csize;			/* cache size, in entries */
    char *cache;		/* pointer to cache */
    int fids;			/* # entries in table */
    struct castr *cadef;	/* table column selectors (column meta-data) */
};

/*
 * struct empfile flags
 */
/* Immutable flags */
/*
 * EFF_XY / EFF_OWNER / EFF_GROUP assert that coordinates / owner /
 * group of such a table's entries can be safely obtained by
 * dereferencing entry address cast to struct genitem *.
 */
#define EFF_XY		bit(0)
#define EFF_OWNER	bit(1)
#define EFF_GROUP	bit(2)
/* Table is allocated statically */
#define EFF_STATIC	bit(3)
/* Flags set when table contents is mapped */
/* Table is entirely in memory */
#define EFF_MEM		bit(4)
/* Table is read-only */
#define EFF_RDONLY	bit(5)
/* Transient flags, just to control ef_open() */
/* Create table file, clobbering any existing file */
#define EFF_CREATE	bit(6)

/* Flags that may be passed to ef_open() */
#define EFF_OPEN	(EFF_MEM | EFF_RDONLY | EFF_CREATE)

/*
 * Empire `file types'
 * These are really table IDs.  Some tables are backed by files, some
 * are compiled into the server.
 */
/* Error value */
#define EF_BAD		-1
/* Dynamic game data tables */
#define EF_SECTOR	0
#define EF_SHIP		1
#define EF_PLANE	2
#define	EF_LAND		3
#define EF_NUKE		4
#define EF_NEWS		5
#define EF_TREATY	6
#define EF_TRADE	7
#define EF_POWER	8
#define EF_NATION	9
#define EF_LOAN		10
#define	EF_MAP		11
#define EF_BMAP		12
#define EF_COMM         13
#define EF_LOST         14
/* Static game data (configuration) */
#define EF_SECTOR_CHR	15
#define EF_SHIP_CHR	16
#define EF_PLANE_CHR	17
#define EF_LAND_CHR	18
#define EF_NUKE_CHR	19
#if 0 /* doesn't exist yet */
#define EF_NEWS_CHR
#endif
#define EF_TREATY_CHR	20
#define EF_ITEM		21
#define EF_INFRASTRUCTURE   22
#define EF_PRODUCT	23
#define EF_TABLE	24
#define EF_SHIP_CHR_FLAGS	25
#define EF_PLANE_CHR_FLAGS	26
#define EF_LAND_CHR_FLAGS	27
#define EF_NUKE_CHR_FLAGS	28
#define EF_META		29
#define EF_META_TYPE	30
#define EF_META_FLAGS	31
#define EF_MAX		32

#define EF_IS_GAME_STATE(type) (EF_SECTOR <= (type) && (type) <= EF_LOST)

struct fileinit {
    int ef_type;
    void (*init) (int, char *);
    int (*postread) (int, char *);
    int (*prewrite) (int, char *);
};

extern struct castr *ef_cadef(int);
extern int ef_read(int, int, void *);
extern void *ef_ptr(int, int);
extern char *ef_nameof(int);
extern time_t ef_mtime(int);
extern int ef_open(int, int);
extern int ef_check(int);
extern int ef_close(int);
extern int ef_flush(int);
extern int ef_write(int, int, void *);
extern int ef_extend(int, int);
extern int ef_ensure_space(int, int, int);
extern int ef_nelem(int);
extern int ef_flags(int);
extern int ef_byname(char *);
extern int ef_byname_from(char *, int *);

extern struct empfile empfile[EF_MAX + 1];

#endif /* _FILE_H_ */
