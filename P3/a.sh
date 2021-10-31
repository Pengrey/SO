#!/bin/bash
for file in $*;do
  echo "$file \n"
  {
  IFS="\w"
  i=0
  while read line; do
    echo $i: $line
    i=$(( $i + 1 ))
  done
  } < $file
done
