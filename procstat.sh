#!/bin/bash
# ls -alh /proc | awk '{ printf  "%s %d %s\n",$6,$7,$8 } ' date of the processe

# ls -alh /proc | awk '{ printf  "%s\n",$3 } ' date of the processe

#awk '/VmSize|VmRSS/{printf $0"\n"}' /proc/11052/status
#awk '/rchar|wchar/{printf $0"\n"}' /proc/11052/io
#sleep 2
#awk '/rchar|wchar/{printf $0"\n"}' /proc/11052/io
#ola=( $(cat /proc/$pid_s/comm) $(ls -alh /proc/ | grep "$pid_s" | awk ' { printf "%s\n",$3 }') $pid_s $(cat /proc/$pid_s/status | awk '/VmSize|VmRSS/ { printf $2"\n" }') $(cat /proc/$pid_s/io | awk '/rchar|wchar/ { printf $2"\n" }') $(ls -alh /proc/ | grep $pid_s | awk '{ printf  "\"%s %d %s\n\"",$6,$7,$8 }') ) &&  echo $ola

###################################################
##
## get info com uma expresao regex
## Usada para-> filtragem de nomes em comm
## ou "*" para ir buscar todos os processos
## parametros entrada:
##      $1: numero de segundos
##      $2: Expressao a utilizar
##  parametros de saida:
##      array de string com info
###################################################
function info_regex (){
  process_info=( )
  secs=$1
  expression=$2
  files=( $(grep "$expression" /proc/*/comm | awk -F ":" '{printf $1 "\n" }' | awk -F "/comm"  '{printf $1 "\n"}'))
  for file in ${files[@]} ;do
    if [[ -z "$(touch -c $file/io 2>&1 | grep 'Permission denied')" ]]  ;then
      rchar_i[$n]=$( awk '/rchar/ { printf $2 }' $file/io )
      wchar_i[$n]=$( awk '/wchar/ { printf $2 }' $file/io )
      sleep $secs & procs_ids[$n]=$!
      n=$(( n + 1))
    fi
  done

  for pid in $(seq 0 $(( ${#procs_ids[@]} - 1 )));do
# processo esta a correr ? sim espera
#pode nao estar a correr para intervalos curtos
    if [ -n "$(ps -p ${procs_ids[$pid]} -o pid=)" ];then
      wait ${procs_ids[$pid]}
    fi
    number_id=$(echo ${files[$pid]} | awk -F "/proc/" '{printf $2}')
    comm=( $(cat ${files[$pid]}/comm) )
    user=( $(ls -lah /proc | grep $number_id | awk ' { printf "%s" , $3 }') )
    mem=( $(awk '/VmSize/ { printf $2 }' ${files[$pid]}/status) )
    rss=( $(awk '/VmRSS/ { printf $2 }' ${files[$pid]}/status) )
    date_proc=$(date -r ${files[$pid]} "+%Y %b %d %H:%M")
    raterchar_i=$(awk -v secs=2 -v inir=${rchar_i[$pid]} '/rchar/ { printf "%f",(($2 - inir)/secs) }' ${files[$pid]}/io )

    ratewchar_i=$(awk -v secs=2 -v iniw=${wchar_i[$pid]} '/wchar/ { printf "%f",(($2 - iniw )/secs) }' ${files[$pid]}/io )
            #com  usr  pid  mem rss rdb wdb rr    rw    date
    printf "%-11s  %-7s  %-5d  %10d  %8d  %18d  %18d  %12.2f  %12.2f  %-17s\n" $comm $user $number_id $mem $rss ${rchar_i[$pid]} ${wchar_i[$pid]} $raterchar_i $ratewchar_i "$date_proc"
    process_info+=( $(printf "%-11s  %-7s  %-5d  %10d  %8d  %18d  %18d  %12.2f  %12.2f  %-17s\n" $comm $user $number_id $mem $rss ${rchar_i[$pid]} ${wchar_i[$pid]} $raterchar_i $ratewchar_i "$date_proc") )
  done
}






################################################

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
  case $name in
    [c]) # regular expression to get pid to analyse
      info_regex $secsW $OPTARG
      ;;
    [s]) # data minima do inicio do processo
      [[ -v process_info ]] || info_regex $secsW ".*"
      echo "nice"
      echo "$OPTARG";;
    [e]) # data maxima do inicio do processo
      [[ -v process_info ]] || info_regex $secsW ".*"
      echo "nice"
      echo "$OPTARG";;
    [u]) # nome do user Done
      [[ -v process_info ]] || info_regex $secsW ".*"
      IFS=$'\n' process_info=($(awk -v usr=$OPTARG '{if($2==usr) {print $0}}' <<< "${arr[*]}")); unset IFS
    [p]) # numero processos a analisar DONE
      [[ -v process_info ]] || info_regex $secsW ".*"
      process_info=${process_info[@]:0:$OPTARG}
      ;;
    [m]) # sort on mem
      IFS=$'\n' process_info=($(sort -nk 4 <<<"${process_info[*]}")); unset IFS
    [t]) # sort on RSS
      IFS=$'\n' process_info=($(sort -nk 5 <<<"${process_info[*]}")); unset IFS
    [d]) # sort on Rater
      IFS=$'\n' process_info=($(sort -nk 8 <<<"${process_info[*]}")); unset IFS
    [w]) # sort on ratew
      IFS=$'\n' process_info=($(sort -nk 9 <<<"${process_info[*]}")); unset IFS
    [r]) # revert order
      IFS=$'\n' process_info=($(sort -r <<<"${process_info[*]}")); unset IFS
  esac
done
