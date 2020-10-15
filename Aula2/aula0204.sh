Whitespace ???  ps = 1 da erro

$ indicacao de variavel

'texto' texto literal nao serao feita substituicoes por variaveis de ambiente nem do ambiente de execucao(variaveis declaradas no ficheiro)

"texto" texto que ainda vai ser processado

# comentario   ....  indicativo de root
echo "sleep 2" ; sleep 2 ; echo "sleep acabou" comandos ocorrem sucessivamente um apos o outro sem validar se o ultimo comando teve ou nao sucesso

\ (Escape) usado para fazer o escape de carateres especiais * qualquer coisa . uma qualquer coisa

<   > redirects

| pipe ligar stdout a stdin

[[ expressoes que de resultado booleano ¿¿¿¿¿¿¿¿¿¿¿¿ ]]
{ lista de comandos; }
'comando' $(comando) quando usado na atribuicao de uma variavel ou como output para texto e substituido pelo que o comando escreve no output ( stdout ? )
(comando) o mesmo ?
(( expressao numerica para substituicao ))
$(( o mesmo ))
