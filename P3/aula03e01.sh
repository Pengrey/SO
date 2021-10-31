#!/bin/bash
#Agrupamento de comandos bash
{
  i=0
  while read line; do
    echo $1: $line
    i=$(($i+1))
  done
} < $1
