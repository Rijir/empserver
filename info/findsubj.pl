#!/usr/bin/perl
#
#  Empire - A multi-player, client/server Internet based war game.
#  Copyright (C) 1986-2007, Dave Pare, Jeff Bailey, Thomas Ruschak,
#                           Ken Stevens, Steve McClure
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#  ---
#
#  See files README, COPYING and CREDITS in the root of the source
#  tree for related information and legal notices.  It is expected
#  that future projects/authors will amend these files as needed.
#
#  ---
#
#  findsubj.pl: Find info subjects, update subjects.mk
# 
#  Known contributors to this file:
#     Ken Stevens (when it was still info.pl)
#     Markus Armbruster, 2006
#

# Usage: findsubj.pl INFO-FILE...
# Run it at the root of the build tree.  This updates the make include
# file subjects.mk, which guides the remaking of info index files.
#
#       --- Global variables ---
# @Subjects       Existing subjects
# $filename       The name of the current info file
# $chapter{TOPIC} TOPIC's chapter (first arg to .TH)
# $see_also{TOPIC}
#                 TOPIC's SEE ALSO items (.SA argument)
# $sanr{TOPIC}    Line number of TOPIC's .SA request
# $subjfil{SUBJECT}
#                 info files for SUBJECT separated by space
#
#     --- File handles ---
# F               Filehandle for info page sources and makefiles
#
#     --- Functions ---
#
# read_make_var   Read a variable value from a makefile
# parse_file      Read an info file
# parse_see_also  Create %subjfil from %see_also
# error           Print an integrity error to STDERR and exit with code 1.

use strict;
use warnings;

use Errno qw(ENOENT);

our (%chapter, %see_also, %sanr);
our ($filename, %subjfil);

# Get known subjects
our @Subjects = split(' ', read_make_var("subjects", "subjects.mk", ""));
# Get source directory
my $srcdir = read_make_var("srcdir", "GNUmakefile");

# Parse the .t files
for my $f (@ARGV) {
    parse_file("$f");
}

# Create %subjfil from %see_also
for my $t (sort keys %see_also) {
    parse_see_also($t);
}

# Update @Subjects from %subjfil
for my $t (@Subjects) {
    print STDERR "WARNING: The subject $t has been removed.\n"
	unless exists $subjfil{$t};
}
for my $t (keys %subjfil) {
    unless (grep(/^$t$/, @Subjects)) {
	print STDERR "WARNING: $t is a NEW subject\n";
	my $fname = "info/$t.t";
	if (-e $fname) {
	    print STDERR "File $fname exists\n";
	    exit 1;
	}
    }
}
@Subjects = sort keys %subjfil;

# Update subjects.mk
open(F, ">subjects.mk")
    or die "Can't open subjects.mk for writing: $!";
print F "subjects := " . join(' ', @Subjects) . "\n";
for my $t (@Subjects) {
    print F "info/$t.t:$subjfil{$t}\n";
}
close(F);

exit 0;

# Read a variable value from a makefile
sub read_make_var {
    my ($var, $fname, $dflt) = @_;
    my $val;

    unless (open(F, "<$fname")) {
	return $dflt if $! == ENOENT and defined $dflt;
	die "Can't open $fname: $!";
    }
    while (<F>) {
	if (/^[ \t]*\Q$var\E[ \t]:?=*(.*)/) {
	    $val = $1;
	    last;
	}
    }
    close(F);
    $val or die "Can't find $var in $fname";
    return $val;
}

# Read an info file
# Parse .TH into %chapter and .SA into %see_also, %sanr
sub parse_file {
    ($filename) = @_;
    my $topic;

    $topic = $filename;
    $topic =~ s,.*/([^/]*)\.t$,$1,;
    
    open(F, "<$filename")
	or die "Can't open $filename: $!";
  
    $_ = <F>;
    if (/^\.TH (\S+) (\S.+\S)$/) {
	$chapter{$topic} = $1;
    } else {
	error("The first line in the file must be a .TH request");
    }

    while (<F>) {
	last if /^\.SA/;
    }

    if ($_) {
	if (/^\.SA "([^\"]*)"/) {
	    $see_also{$topic} = $1;
	    $sanr{$topic} = $.;
	} else {
	    error("Incorrect .SA Syntax.  Syntax should be '.SA \"item1, item2\"'");
	}

	while (<F>) {
	    error("Multiple .SA requests.  Each file may contain at most one.") if /^\.SA/;
	}
    } else {
	error(".SA request is missing");
    }

    close F;
}

# Create %subjfil from %see_also
sub parse_see_also {
    my ($topic) = @_;
    my @see_also = split(/, /, $see_also{$topic});
    my $wanted = $chapter{$topic};
    my $found;		       # found a subject?

    $wanted = undef if $wanted eq 'Concept' or $wanted eq 'Command';
    $filename = "$srcdir/$topic";

    for (@see_also) {
	if (!exists $see_also{$_}) { # is this entry a subject?
	    $subjfil{$_} .= " info/$topic.t";
	    $found = 1;
	}
	if ($wanted && $_ eq $wanted) {
	    $wanted = undef;
	}
    }

    $. = $sanr{$topic};
    error("No subject listed in .SA") unless $found;
    error("Chapter $wanted not listed in .SA") if $wanted;
}

# Print an integrity error message and exit with code 1
sub error {
    my ($error) = @_;

    print STDERR "findsubj.pl:$filename:$.: $error\n";
    exit 1;
}
