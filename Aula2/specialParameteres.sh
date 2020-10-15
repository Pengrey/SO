#!/bin/bash

echo "--------------------------------_"
echo "\$\*"
# palavras separadas por espacos em branco mesmo estando dentro de " irao ser separadas
for words in $*
do
	echo $words;
	printf "\n"
done
echo "--------------------------------_"
echo "\"\$\*\""
# vai aparecer apenas uma linha que vai conter todos os argumentos inserido como se fosse um argumento gigante
for words in "$*"
do
	echo $words;
	printf "\n"
done
echo "--------------------------------_"
echo "\"\$\@\""
# todas as entradas dentro de quotes ( single or double ) irao ser consevadas como um argumentos simples
for words in "$@"
do
	echo $words;
	printf "\n"
done
echo "--------------------------------_"
echo "\$\@"
# palavras separadas por espacos em branco mesmo estando dentro de " irao ser separadas
for words in $@
do
	echo $words;
	printf "\n"
done
