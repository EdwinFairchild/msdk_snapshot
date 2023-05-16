#!/usr/bin/env perl

while (<>) {
    chomp;
    if ($_ =~ /^h/) {
	($reg, $dev1, $dev2, $dev3, $crap) = split(' ', $_);
	#print $reg . " " . $dev1 . "\n";
	$reg =~ s/h//g;
	$reg =~ s/_.*//g;
	$addr = hex($reg);
	# Select which column
	$col = hex($dev3);
	if (!exists($info{$addr})) {
	    $info{$addr} = $col;
	} else {
	    $info{$addr} += ($col << 16);
	}
    }
}

for ($i = 0; $i < 0x1000; $i += 4) {
    if (exists($info{$i})) {
	printf("0x%08x, 0x%08x,\n", $i, $info{$i});
    } else {
	printf("0x%08x, 0xffffffff,\n", $i);
    }
	       
}
