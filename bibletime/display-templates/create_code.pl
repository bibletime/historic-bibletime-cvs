#!/usr/bin/perl

use strict;

my %names;

sub read_names() {
	open(IN, "< names.conf");
	
	while (<IN>) {
		my $line = $_;
		chomp $line;
		
		my ($filename, $description) = split("\t", $line);
		$names{ $filename } = $description;
	}	
	
	close(IN);
}

sub extract_data {
	my $file = shift || die;
	
	my $name;
	my $html;
	
	$name = $names{ $file };
	
	open(IN, "< $file");	
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

&read_names;
my $code = "";
foreach my $f (@ARGV) {
	my ($name, $html) = &extract_data( $f );
	
	$code .= "\tm_templateMap[ i18n(\"$name\") ] = \"$html\";"
}


open(OUT, "> template-init.cpp");
print OUT "void CDisplayTemplateMgr::init() {\n$code\n}\n";
close(OUT);