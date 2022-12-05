# !/bin/bash
#
# testing.sh -- testing file for dealer
#
# usage: bash -v testing.sh &> testing.out 
#
# Team 12, CS50, Fall 2022

# Make player output file
rm -rf player.out ; touch player.out

# Testing with no arguments
./dealer
# Check error code
echo $?

# Testing with one arguments
./dealer Team12
# Check error code
echo $?

# Testing with too many arguments
./dealer Team12 Ipaddress bad
# Check error code
echo $?

# Testing with bad port
./dealer 1 port
# Check error code
echo $?

# Testing with bad port
./dealer 1 0
# Check error code
echo $?

# Testing with non-int number of games
./dealer one 8092
# Check error code
echo $?

# Testing with 0 number of games
./dealer 0 8092
# Check error code
echo $?

# Testing with player
./dealer 50 8092 &

IPADDR=$(curl ifconfig.me)

cd ../player ; ./player Name $IPADDR 8092 > ../dealer/player.out 2>&1 

# Check status code
echo $?

