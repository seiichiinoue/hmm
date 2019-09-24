#!/usr/bin/perl

use strict;
use List::Util qw(sum min max);
binmode STDIN, ":utf8";
binmode STDOUT, ":utf8";
binmode STDERR, ":utf8";

if(@ARGV < 2 or @ARGV > 3) {
    print STDERR "Usage: grade-bayes-hmm.pl REF_FILE TEST_FILE [OUT_FILE]\n";
    exit 1;
}

open REF, "<:utf8", $ARGV[0] or die $!;
open TEST, "<:utf8", $ARGV[1] or die $!;
my ($ref,$test, %mapping);
while($ref = <REF> and $test = <TEST>) {
    chomp $ref; chomp $test;
    $ref =~ s/[^ ]*\///g; $test =~ s/[^ ]*\///g;
    my @rarr = split(/ /,$ref);
    my @tarr = split(/ /,$test);
    @rarr == @tarr or die "mismatched arrays:\n@rarr\n@tarr";
    for(0 .. $#rarr) {
        $mapping{$tarr[$_]} = {} if not $mapping{$tarr[$_]};
        $mapping{$tarr[$_]}->{$rarr[$_]}++;
    }
}
close REF;
close TEST;

my ($correct,$total,%id2sym);
foreach my $idname (sort keys %mapping) {
    my %taghash = %{$mapping{$idname}};
    my @arr = sort { $taghash{$b} <=> $taghash{$a} } keys %taghash;
    my $best = $arr[0];
    $id2sym{$idname} = $best;
    $correct += $taghash{$arr[0]};
    $total += sum(values %taghash);
    print "$idname -> $best\n";
    for(@arr) {
        print " $_=$taghash{$_}";
    }
    print "\n";
}

print sprintf("Accuracy: %.2f%% (%i/%i)\n", $correct/$total*100, $correct,$total);
