#!/bin/bash
function imprime_msg()
{
  echo "A minha primeira funcao"
  date 
  hostname
  echo "$USER"
  return 0
}
imprime_msg
