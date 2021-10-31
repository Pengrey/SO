#!/bin/bash
function isgreater()
{
  read -p "First number: " num1
  read -p "Second number: " num2
  if [ $num1 -gt $num2 ]
  then 
    echo "$num1 is greater then $num2"
  elif [ $num1 -lt $num2 ]
  then
    echo "$num1 is less then $num2"
  else
    echo "$num1 is equal to $num2"
  fi
  return 0
}
isgreater 
