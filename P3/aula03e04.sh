#!/bin/bash
#This script does a very simple test fir checking disk space.
space=$(df -h | awk '{print $5}' | grep % | grep -v Use | sort -n | tail -1 | cut -d "%" -f1 -)
echo "Largest occupied space = $space%"
echo "Largest partition = $nspace %"
case $space in
  [1-6]*)
    Message="All OK."
    ;;
  [7-8]*)
    Message="Cleaning out. There's a partition that is $space % full."
    ;;
  9[0-8]*)
    Message="Better buy a new disk... One partition is $space % full."
    ;;
  99)
    Message="I'm drowning here! There's a partition at $space %!"
    ;;
  *)
    Message="I seem to be running with an nonexistent disk..."
    ;;
esac 
echo $Message
