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
	
	$file =~ /(?:.+\/)?(.+\..+)/;
	my $tmpl_name = $1 ? $1 : $file;
	$name = $names{ $tmpl_name };
	
	open(IN, "< $file");	
	while( <IN> ) {
		my $line = $_;
		chomp($line);
		
		
		$line =~ s/"/\\"/g;
		$line =~ s/\s{2,}|\t/ /g; #replace spaces and tabs by space
		
		$html .= $line;
	}
	close(INT);
	
	
	print "Found $name\n" if ($name);
	return ($name, $html);
}

&read_names;
my $code = "";
foreach my $f (@ARGV) {
	my ($name, $html) = &extract_data( $f );
	
	if ($name) {
		$code .= "\tm_templateMap[ i18n(\"$name\") ] = \"$html\";\n"
	}
}


open(OUT, "> template-init.cpp");
print OUT "void CDisplayTemplateMgr::init() {\n$code\n}\n";
close(OUT);
