#!/bin/sh
shell_start=`date +%s`
make clean
clear
git add $1
git commit -m $2
git push origin master:SLAVE
git pull
shell_end=`date +%s`
time=$[shell_end - shell_start]
echo "time = "$time's'
