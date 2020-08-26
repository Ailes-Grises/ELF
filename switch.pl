#!/usr/bin/perl

use strict;
use warnings;

my $file1 = "case";
my $file2 = "hoge";
my $filew = "foo";

open(my $case, $file1) or die "($!)";
open(my $cout, $file2) or die "($!)";
open(my $fout, '>'.$filew) or die "($!)";

my @str;
for(my $i = 0; $i<80; $i++){
	$_ = <$case>;
	chomp($_);
	$str[0] = $_;

	$_ = <$cout>;
	chomp($_);
	$str[1] = $_;

	print $fout "$str[0]\n  $str[1]\n";
}

close($case);
close($cout);
close($fout);
