O jogo está modularizado.

- Screen (São aquelas que interferem no que os jogadores veem);
- Core (Ou núcleo, possui funções que interferem no funcionamento do jogo);
- Intelligence (São aquelas funções relacionadas às jogadas efetuadas
pelo computador)

Sendo assim, as funções estão separadas em 3 arquivos: screen.c, core.c e intelligence.c
Cada uma delas possui seu respectivo cabeçalho: screen.h, core.h e intelligence.h

Todos cabeçalhos estão sendo incluídos na main que está no arquivo main.c

* IMPORTANTE *

Para compilar o código e fazer a linkagem automaticamente de todos os objetos
basta executar 'make' no terminal.

Certifique-se, no entanto, de fazer isso em uma pasta em que não contenha outros
arquivos .c , pois se não o compilador gcc irá notar que existe mais de um código
com uma função main e retornará um erro.
