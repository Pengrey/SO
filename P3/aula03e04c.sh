#! /bin/bash
case $1 in  
   [0-9][0-9]*)
    case $2 in
      sec**)
        Message="All OK."
        ;;
      *)
        Message="Wrong m8."
        ;;
    esac
    ;;
  *)
    Message="Wrong m8."
    ;;
esac
echo $Message
