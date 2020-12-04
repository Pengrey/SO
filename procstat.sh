#!/bin/bash
# ls -alh /proc | awk '{ printf  "%s %d %s\n",$6,$7,$8 } ' date of the processe

# ls -alh /proc | awk '{ printf  "%s\n",$3 } ' date of the processe

#awk '/VmSize|VmRSS/{printf $0"\n"}' /proc/11052/status
#awk '/rchar|wchar/{printf $0"\n"}' /proc/11052/io
#sleep 2
#awk '/rchar|wchar/{printf $0"\n"}' /proc/11052/io
#ola=( $(cat /proc/$pid_s/comm) $(ls -alh /proc/ | grep "$pid_s" | awk ' { printf "%s\n",$3 }') $pid_s $(cat /proc/$pid_s/status | awk '/VmSize|VmRSS/ { printf $2"\n" }') $(cat /proc/$pid_s/io | awk '/rchar|wchar/ { printf $2"\n" }') $(ls -alh /proc/ | grep $pid_s | awk '{ printf  "\"%s %d %s\n\"",$6,$7,$8 }') ) &&  echo $ola
function get_Info () {
  pid_s=$1
  ola+=( $(cat /proc/$pid_s/comm ; ls -alh /proc/ | grep $pid_s | awk ' {  printf "\t%s\n",$3 }'; printf "\t$pid_s"; awk '/VmSize|VmRSS/ { printf "\t"$2"\n" }' /proc/$pid_s/status  ;r1=$(awk '/rchar/ { printf "\t"$2"\n" }') /proc/$pid_s/io && echo $r1;w1=$(awk '/wchar/ { printf "\t"$2"\n" }') /proc/$pid_s/io && echo $w1  ; sleep $2 & proc=$!;wait $proc  ls -alh /proc/ | grep $pid_s | awk '{ printf  "\t%s %d %s\n",$6,$7,$8 }' | sed "s/\"//g") )
}

function usage () {
  echo "specify the number of seconds"
  exit 1
}

input=("$@")
for i in ${input[@]};do
  [[ "$i" =~ ^[0-9]+$ ]] && secsW=$i
done
[[ -v secsW ]] || usage

while getopts ":c:s:e:u:p:mtdwr:" name ;do
  echo "fck"
  case $name in
    [c]) # regular expression to get pid to analyse
      echo "ola"
      pids=$(pgrep "$OPTARG")
      for p in ${pids[@]};do
        get_Info $p $secsW
      done;
      echo ${ola[@]}
      ;;
    [s]) # data minima do inicio do processo
      echo "nice"
      echo "$OPTARG";;
    [e]) # data maxima do inicio do processo
      echo "nice"
      echo "$OPTARG";;
    [u]) # nome do user
      echo "nice"
      echo "$OPTARG";;
    [p]) # numero processos a analisar
      echo "nice"
      echo "$OPTARG";;
    [m]) # sort on mem
      echo "nice"
      echo "$OPTARG";;
    [t]) # sort on RSS
      echo "nice"
      echo "$OPTARG";;
    [d]) # sort on Rater
      echo "nice"
      echo "$OPTARG";;
    [w]) # sort on ratew
      echo "nice"
      echo "$OPTARG";;
    [r]) # revert order
      echo "$name"
      echo "$OPTARG";;
  esac
done
