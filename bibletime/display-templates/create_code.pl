#!/usr/bin/perl

use strict;

sub extract_data {
	my $file = shift || die;
	
	my $name;
	my $html;
	
	open(IN, "< $file");
	$name = <IN>;
	chomp($name);
	
	while( <IN> ) {
		my $line = $_;
		chomp($line);
		$line =~ s/"/\\"/g;
		
		$html .= $line;
	}
	
	close(INT);
	
	
	print "Found $name\n";
	return ($name, $html);
}

my $code = "";
foreach my $f (@ARGV) {
	my ($name, $html) = &extract_data( $f );
	
	$code .= "\tm_templateMap[ i18n(\"$name\") ] = \"$html\";"
}


open(OUT, "> template-init.cpp");
print OUT "void CDisplayTemplateMgr::init() {\n$code\n}\n";
close(OUT);