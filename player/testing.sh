# !/bin/bash
#
# testing.sh -- testing file for player
#
# usage: bash -v testing.sh &> testing.out 
#
# Team 12, CS50, Fall 2022

# Make dealer output file
rm -rf dealer.out ; touch dealer.out

# Testing with no arguments
./player

# Testing with one arguments
./player Team12

# Testing with two arguments
./player Team12 Ipaddress

# Testing with too many arguments
./player Team12 Ipaddress bad bad

# Testing with bad port
./player whatevername whateverip 0

# Testing with dealer
cd ../dealer ; ./dealer 50 8092 > dealer.out 2>&1 & 

IPADDR=$(curl ifconfig.me)

echo $IPADDR

cd ../player ; ./player Name $IPADDR 8092

# test with provided dealer
cd ../dealer ; ./dealer-given 1 50 8092 > ../player/dealer.out 2>&1 & 

cd ../player ; ./player Name $IPADDR 8092
