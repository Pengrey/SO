#! /bin/bash
#For all the filer in a folder, show their propeties
if [[ -d $1 && $# -eq 1 ]]; then
  echo "Doing it"
  for f in $1/*; do
    file"$f"
  done
  else
    echo "Wrong m8!"
fi
