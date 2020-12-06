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
declare -ga process_info=()
function info_regex (){
  n=0
  secs=$1
  echo $secs
  expression=$2
  process_ids=( $(grep ".*" /proc/*/comm | awk -v pat="^/proc/[0-9]+/comm:$expression" '{  if ($0 ~ pat) { printf $0 "\n" } }  ' | awk -F "/comm:" '{printf $1 "\n" }' ) )
  timer1=$(date "+%s")
  for i in ${!process_ids[@]};do
    if [[ -r "${process_ids[$i]}/io" ]];then
      rchar_i[$i]=$( awk '/rchar/ { printf $2 }' ${process_ids[$i]}/io )
      wchar_i[$i]=$( awk '/wchar/ { printf $2 }' ${process_ids[$i]}/io )
      sleep $secs & procs_ids[$i]=$!
    else
      procs_ids[$i]=0
    fi
  done

  for i in ${!process_ids[@]};do
    if [[ ${procs_ids[$i]} != 0 ]];then
      if [[ -r "/proc/${procs_ids[$i]}/io" ]];then
        wait ${procs_ids[$i]}
      fi
    fi
    if [[ -r "${process_ids[$i]}/io" ]];then
      number_id=$(echo ${process_ids[$i]} | awk -F "/proc/" '{printf $2}')
      comm=$(cat ${process_ids[$i]}/comm | sed "s/ /\"/g")
      user=$(stat -c "%U" ${process_ids[$i]}/comm)
      mem=$(awk '/VmSize/ { printf $2 }' ${process_ids[$i]}/status)
      rss=$(awk '/VmRSS/ { printf $2 }' ${process_ids[$i]}/status)
      date_proc=$(date -r ${process_ids[$i]} "+%b %d %H:%M")

      raterchar_i=$(awk -v secs=$secsW -v inir=${rchar_i[$i]} '/rchar/ { printf "%f",(($2 - inir)/secs) }' ${process_ids[$i]}/io )

      ratewchar_i=$(awk -v secs=$secsW -v iniw=${wchar_i[$i]} '/wchar/ { printf "%f",(($2 - iniw )/secs) }' ${process_ids[$i]}/io )
      if [[ -n $mem && -n $rss ]];then
        string=$(printf "%-22s\t%-11s\t%-5d\t%8d\t%8d\t%10d\t%10d\t%12.2f\t%12.2f\t%-17s\n" $comm $user $number_id $mem $rss ${rchar_i[$i]} ${wchar_i[$i]} $raterchar_i $ratewchar_i "$date_proc")

        process_info[$n]="$string"
      fi

      n=$(( $n + 1 ))
    fi
  done
  timer2=$(date "+%s")
  echo $(( $timer2 - $timer1 ))
}






################################################

function usage () {
  echo "
  procstat.sh [option] secs
      You must specify a valide number of seconds, or use nono overlapping options
      OPTIONS:
      -h to show this help message
      -t sort process in ascending orders of rss values
      -m  sort process in ascending orders of mem values
      -r sort process in reverse order
      -d  sort process in ascending orders of rater values
      -w  sort process in ascending orders of ratew values

      -p [number of process to be show] show only the first n specified values
      -s [minimal date for the process start]
      -e [maximal date for the process start]

      -u [username] filters all process wich files are owned by username
      -c \"regex\" given a regex filters all process whose name correspond to that pattern


      This program tries to replicate ps.
      Every file whose process insn't protected or hasn't dissapeared will be read twice exactly 'secs' after, and the info extracted from them will be prompted to the user.
  "
  exit 1
}

#input=("$@")
#for i in ${input[@]};do
  #if [[ "$i" =~ [[:digit:]]+  ]];then
    #secsW="$i"
    #break
  #fi
#done
secsW="${!#}"
[[ -v secsW ]] || usage
regexex=".*"

while getopts ":c:s:e:u:p:mtdwr" name ;do
  case $name in
    [c]) # regular expression to get pid to analyse
      regexex=$OPTARG
      #info_regex $secsW $OPTARG
      ;;
    [s]) # data minima do inicio do processo
      s_date=$OPTARG
     ;;
    [e]) # data maxima do inicio do processo
      e_date=$OPTARG
      ;;
    [u]) # nome do user Done
      file_owner=$OPTARG
      #IFS=$'\n' process_info=($(awk -v usr=$OPTARG '{if($2==usr) {print $0}}' <<< "${arr[*]}")); unset IFS
      ;;
    [p])
      ##[[ -v process_info ]] || info_regex $secsW ".*"
      number_of_process=$OPTARG
      #process_info=${process_info[@]:0:$OPTARG}
      ;;
    [m]) # sort on mem
      #[[ -v process_info ]] || info_regex $secsW ".*"
      #IFS=$'\n' process_info=($(sort -nk 4 <<<"${process_info[*]}")); unset IFS
      #printf "%s\n" "${process_info[@]}" | sort -nk 4
      if [[ -v sort_by ]] ;then
        echo "only one sorting way can be used"
        usage
        exit 1
      else
        sort_by="mem"
      fi
      ;;
    [t]) # sort on RSS
      #[[ -v process_info ]] else
      #IFS=$'\n' process_info=($(sort -nk 5 <<<"${process_info[*]}")); unset IFS
      #printf "%s\n" "${process_info[@]}" | sort -nk 5
      if [[ -v sort_by ]] ;then
        echo "only one sorting way can be used"
        usage
        exit 1
      else
        sort_by="rss"
      fi
      ;;
    [d]) # sort on Rater
      if [[ -v sort_by ]];then
        echo "only one sorting way can be used"
        usage
        exit 1
      else
          sort_by="rater"
      fi
      #[[ -v process_info ]] || info_regex $secsW ".*"
      #printf "%s\n" "${process_info[@]}" | sort -nk 8
      #IFS=$'\n' process_info=($(sort -nk 8 <<<"${process_info[*]}")); unset IFS
      ;;
    [w]) # sort on ratew
      if [[ -v sort_by ]];then
        echo "only one sorting way can be used"
        usage
        exit 1
      else
          sort_by="ratew"
      fi
      #[[ -v process_info ]] || info_regex $secsW ".*"
      #IFS=$'\n' process_info=($(sort -nk 9 <<<"${process_info[*]}")); unset IFS
      #printf "%s\n" "${process_info[@]}" | sort -nk 9
      ;;
    [r]) # revert order
      sort_i="inverse"
      #[[ -v process_info ]] || info_regex $secsW ".*"
      #IFS="\n";process_info=($(sort -r <<< "${process_info[*]}")); unset IFS
      #printf "%s" "${process_info[@]}"
      #printf "%s\n" "${process_info[@]}" | sort -r
      ;;
  esac
done
[[ -v sort_by ]] || sort_by="alpha"

info_regex $secsW $regexex
#number of process esta definida entao imprime-se os primeiros number_of_process
if [[ -v file_owner  ]];then
  IFS=$'\n'
  process_info=($(printf "%s\n" "${process_info[@]}" | awk -v usr=$file_owner '{if($2==usr) {print $0}}'))
fi

if [[ -v s_date &&  -v e_date  ]];then
  process_info=($(printf "%s\n" "${process_info[@]}" | awk -v max=($(date -d "$s_date" "+%s")) '{if(($(date -d "$10" "+%s"))<max) {print $0}}'))
  process_info=($(printf "%s\n" "${process_info[@]}" | awk -v min=($(date -d "$e_date" "+%s")) '{if(($(date -d "$10" "+%s"))>min) {print $0}}'))
fi
printf "%-11s\t%-12s\t%-7s\t%7s\t%16s\t%8s\t%9s\t%12s\t%12s\t%17s\n" "COMM" "USER" "PID" "MEM" "RSS" "READB" "WRITEB" "RATER" "RATEW" "DATE"

case "$sort_by" in
  "alpha")
                        #sort_i esta definido entao flag -r for adicionada    sort_i nao esta definido
    if [[ -v sort_i ]];then
      process_info=("$(printf "\"%s\"\n" "${process_info[@]}" | sort -nk 9 -r)")
    else
      process_info=("$(printf "%s\n" "${process_info[@]}" | sort -d)")
    fi
    ;;
  "ratew")
                        #sort_i esta definido entao flag -r for adicionada    sort_i nao esta definido
     if [[ -v sort_i ]];then
       process_info=("$(printf "%s\n" "${process_info[@]}" | sort -nk 9 -r)")
     else
      process_info=("$( printf "%s\n" "${process_info[@]}" | sort -nk 9)")
     fi
    ;;
  "rater")
                        #sort_i esta definido entao flag -r for adicionada    sort_i nao esta definido
    if [[ -v sort_i ]];then
       process_info=("$( printf "%s\n" "${process_info[@]}" | sort -nk 8 -r)")
    else
       process_info=("$( printf "%s\n" "${process_info[@]}" | sort -nk 8)")
    fi
    ;;
  "rss")
                        #sort_i esta definido entao flag -r for adicionada    sort_i nao esta definido
    if [[ -v sort_i ]];then
     process_info=("$( printf "%s\n" "${process_info[@]}" | sort -nk 5 -r)")
    else
      process_info=("$(printf "%s\n" "${process_info[@]}" | sort -nk 5)")
    fi
    ;;
  "mem")
                        #sort_i esta definido entao flag -r for adicionada    sort_i nao esta definido
    if [[ -v sort_i ]];then
      process_info=("$(printf "%s\n" "${process_info[@]}" | sort -nk 4 -r)")
    else
     process_info=("$( printf "%s\n" "${process_info[@]}" | sort -nk 4)")
    fi
    ;;
  "inverse")
                      #sort_by nao esta definido apenas foi chamada a flag -r
    [[ -v sort_by ]] || printf "%s\n" "${process_info[@]}" | sort -r
    ;;
esac
n=0
IFS=$'\n'
echo $number_of_process
for val in $process_info;do
  if [[ -v number_of_process ]];then
    if [[ $n -lt $number_of_process ]];then
      printf "%s\n" $val
    fi
  else
      printf "%s\n" $val
  fi
  n=$(( $n + 1 ))
done
unset IFS
