#!/bin/sh

#kr   	= Kernigham & Ritchie
# Identically to 
#	-nbad -bap -bbo -nbc -br -brs -c33 -cd33 -ncdb -ce -ci4 -cli0
#      	-cp33 -cs -d0 -di1 -nfc1 -nfca -hnl -i4 -ip0 -l75 -lp -npcs
#      	-nprs -npsl -saf -sai -saw -nsc -nsob -nss
#nce  	= 
#bbo	=

#indent -kr  -fc1 -nce -cdb -cli2 -cbi2 -ts4 -sc -l120 -ncs -cdw $@

for f in $(find . -name \*.cpp -or -name \*.h); do
	echo "	$f";
	astyle --style=kr -N --convert-tabs $f;
done
