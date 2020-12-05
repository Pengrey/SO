#!/bin/bash
files=( $(grep "^br" /proc/*/comm | awk -F ":" '{printf $1 "\n" }' | awk -F "/comm"  '{printf $1 "\n"}'))

n=0
for file in ${files[@]} ;do
  rchar_i=$(  awk '/rchar/ { printf $2 "\n" }' $file/io )
  wchar_i=$(  awk '/wchar/ { printf $2 "\n" }' $file/io )
  echo $rchar_i $wchar_i
  sleep 2 & procs_ids[$n]=$!
  n=$(( n + 1))
done

echo "-----------------------"
for pid in $(seq 0 $(( ${#procs_ids[@]} - 1 )));do
  if [ -n "$(ps -p ${procs_ids[$pid]} -o pid=)" ];then
    wait ${procs_ids[$pid]}
  fi
  raterchar_i=$(  awk -v secs=2 -v inir=$rchar_i '/rchar/ { printf "%f\n",(($2 - inir)/2) }' ${files[$pid]}/io )
  ratewchar_i=$(  awk -v secs=2 -v inir=$wchar_i '/wchar/ { printf "%f\n",(($2 - inir )/2) }' ${files[$pid]}/io )
  echo $raterchar_i      $ratewchar_i

done
