#!/bin/bash

echo "A miha shell por omissao $BASH" # texto que ira ser substituido -> a minha shell por omissao bash
echo 'A minha shell por omissao $BASH' # texto literal
echo $(( 5 + 5 )) # 10         a operacao aritmetica 5 + 5 vai ser substituida pelo seu resultado
# as expressoes irao ser avaliadas
# a expressao e avaliada e o primeiro echo sera executado se a expressao e verdadeira
# a expressao e avaliada e o segundo echo sera executado se a expressao e verdadeira
# ; o terceiro echo ira sempre ser executado ; nao espera uma valor positivo ou negativo para executar mas sim espera pela sua vez de execucao
(( 5 > 0 )) && echo "cinco e maior que zero" || echo " 0  e maior que zero"  ; echo "Nao quis saber e imprimi isto"
echo "--------------------------"
(( 5 == 0 )) && echo "cinco e maior que zero" || echo " 5  e nao e igual a zero"  ; echo "Nao quis saber e imprimi isto"
echo "--------------------------"
[[ 5 -le 0  ]] && echo "cinco e maior que zero" || echo " 5  nao e menor que zero"  ; echo "Nao quis saber e imprimi isto"
echo "--------------------------"
today=$(date) ; echo $today
# echo a seguir aos carateres || vai ser executado
# o echo apos && nao sera executado
# o printf "$today\n" ira imprimir uma linha branca
today=$(date -f "+%Y") && echo "sera que apareco" || echo "date nao gosta de -f como flag para formatar"; printf "${today} \n"
