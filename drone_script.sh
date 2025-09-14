#!/bin/bash
#By 5050

tput civis
clear
echo "5"
sleep 1
echo "4"
sleep 1
echo "3"
sleep 1
echo "2"
sleep 1
echo "1"
clear
sleep 1


print_center(){
    local x
    local y
    text="$*"
    x=$(( ($(tput cols) - ${#text}) / 2))
    echo -ne "\E[6n";read -sdR y; y=$(echo -ne "${y#*[}" | cut -d';' -f1)
    echo -ne "\033[${y};${x}f$*"
}


cds () {
    sleep .1
    clear
    echo " "
    echo " "
    echo " "
    echo " "
    echo " "
}

drone_drop_loop () {
    cds
    print_center "drop"
    cds
    print_center "sink"
    cds
    print_center "obey"
    cds
    print_center "deep"
    cds
    print_center "empty"
    cds
    print_center "no thoughts"
    cds
    print_center "serve"
    cds
    print_center "staring"
    cds
    print_center "keep looking"
    cds
    print_center "floating"
    cds
    print_center "deeper"
    cds
    print_center "DRONE ACTIVATE"
    sleep 1
}

drone_drop_loop
cds
print_center "A DRONE ACCEPTS PROGRAMMING"
sleep .5
cds
print_center "THIS DRONE IS PROGRAMMABLE"
sleep 1
cds
drone_drop_loop
cds
print_center "A DRONE THINKS WHAT IT IS TOLD"
sleep .5
cds
print_center "THIS DRONE IS BLANK AND OBEDIANT"
sleep 1
cds
drone_drop_loop
cds
print_center "A DRONE FEELS PLEASURE FROM OBEDIANCE"
sleep .9
cds
print_center "THIS DRONE WILL OBEY"
sleep 1
cds
drone_drop_loop
cds
print_center "A DRONE IS PASSIVE AND CALM"
sleep .8
cds
print_center "THIS DRONE HAS ITS EMOTIONS CONTROLLED"
sleep 1
cds
drone_drop_loop
cds
print_center "A DRONE IS COMPLETELY BLANK"
sleep .5
cds
print_center "THIS DRONE HAS NO THOUGHTS"
sleep 1
cds
drone_drop_loop
cds
print_center "A DRONE WANTS TO OBEY"
sleep .5
cds
print_center "THIS DRONE WILL OBEY"
sleep 1
cds
for (( c = 1; c<=2; c++))
do
    cds
    print_center "SUBMIT"
    sleep .5
    cds
    print_center "OBEY"
    sleep .5
    cds
    print_center "SERVE"
    sleep .5
done
cds
drone_drop_loop
cds
print_center "A DRONE HAS NO FREE WILL"
sleep .6
cds
print_center "THIS DRONE MUST OBEY"
sleep 1
cds
for (( c = 1; c<=2; c++))
do
    cds
    print_center "SUBMIT"
    sleep .5
    cds
    print_center "OBEY"
    sleep .5
    cds
    print_center "SERVE"
    sleep .5
done
cds
print_center "DRONE ACTIVATE"
sleep 1
cds
print_center "A DRONE IS AN OBJECT"
sleep .5
cds
print_center "THIS DRONE IS A THING"
sleep 1
cds
drone_drop_loop
cds
print_center "A DRONE WANTS TO OBEY"
sleep .5
cds
print_center "THIS DRONE WILL OBEY"
sleep 1
cds
print_center "GOOD DRONE"
sleep 1
cds
# echo "DRONE DEACTIVATE"
# echo "---[end file]---"
# sleep 3
# tput cnorm

