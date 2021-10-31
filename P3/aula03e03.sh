#!/bin/bash
# This script checks the existence of a file
c=0
for arg in "$@"
do
  if [[ -f  $arg ]];  then
    echo ""
    echo "Name: $arg"
    echo "Status:"
    echo "-Valid"

    if [[ -d $arg ]];  then
      echo "-Directory"
    fi

    if [[ -r $arg ]];  then
      echo "-Readable"
    fi

    if [[ -w $arg ]];  then
      echo "-Writable"
    fi 

    if [[ -x $arg ]]; then
      echo "-Executable"
    fi

    c=$(($c+1))
  else
    echo ""
    echo "Name: $arg"
    echo "Status:"
    echo "-Invalid"
  fi
done

echo ""
echo "Number of valid Files: $c"











