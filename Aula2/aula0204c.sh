#!/bin/sh

echo "a)"
ls /etc
# lista todos os ficheiros em etc
echo "b)"

# lista todos os ficheiros em etc que comecam em a
ls /etc/a*
ls /etc | grep -i "^a"


echo "c)"

# lista todos os ficheiros em etc que comecao em a e tem mais de 3 letras
ls /etc/a??*
ls /etc | grep -i "^a..*"

echo "d)"

# lista todos os ficheiros em etc com conf no nome
ls /etc | grep -i "conf" | less
ls /etc/*conf*
