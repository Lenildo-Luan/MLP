# Minimum Latency Problem (MLP)

O MLP se trata de um problema de otimização combinatória da classe NP-difícil por ser uma variante do [TSP](https://github.com/Lenildo-Luan/TSP-GILS-RVND), um problema bastante conhecido e estudado na literatura.

Para entender o problema, imagine um vendedor com um conjunto pré-determinado de clientes onde cada cliente possui um custo de viagem (que pode ser tempo, distância ou qualquer variável relevante) dele para os outros clientes, ou seja, o custo de viagem do cliente N1 para o cliente N2 pode ser 200 enquanto do N1 para o N3 150. A ideia é que o vendedor passe por cada cliente somente uma vez e tenha que voltar ao ponto de partida no fim do percurso. Para reduzir custos, é necessário escolher uma rota, dentre todas as rotas possíveis, onde o somatório da latência dos custos de viajem até cada cliente seja o menor, sendo a latência total, o somatório dos custos de viajem totais do ponto inicial até cada cliente.

## Formas de resolução

Existem dois métodos que podem ser seguidos para resolver problemas como o descrito anteriormente, os exatos ou os heurísticos.

Métodos exatos visam iterar sobre todo o conjunto de soluções de um problema para obter sua resposta. Por conta disso, eles retornam a solução ótima do problema, ou seja, a melhor dentre todas as soluções. Por outro lado, para problemas da classe NP, não é possível encontrar estas soluções em tempo computacional aceitável a partir de certo ponto. Para ilustrar, uma instância do TSP com 60 vértices possui uma quantidade de soluções possíveis semelhante a quantidade de átomos no universo!

Por outro lado, existem os métodos heurísticos, que tentar resolver estes problemas com um tempo computacional aceitável sem garantir sua otimalidade, mas que podem trazer boas soluções, e os meta-heurísticos, que tem a mesma finalidade, porém com o intuito de resolver vários tipos de problemas diferentes com um mesmo algoritmo.

O algorítmio implementado neste repositório para resolver o TSP  se trata de uma meta-heurística denominada [GILS-RVND](https://www.sciencedirect.com/science/article/abs/pii/S037722171200269X).

## Instância do MLP

As instâncias do TSP são criadas a partir de um conjunto de cidades, cada uma com sua localização geográfica. Em posse desses dados, os custos de deslocamento podem ser calculados para cada par de cidades, onde é descrito por uma matriz, podendo esta ser simétrica ou assimétrica.

As instâncias contidas neste repositório são **simétricas**, e estão todas localizadas em `./instances` ou podem ser obtidas no [TSPLIB](http://elib.zib.de/pub/mp-testdata/tsp/tsplib/tsplib.html), porêm, os resultados ótimos de cada instância podem ser obtidos no seguinte [artigo](https://www.sciencedirect.com/science/article/abs/pii/S037722171200269X). Segue um exemplo de matriz simétrica para um problema com 4 vértices.

```
M = | -- 10 20 30 |
    | 10 -- 10 20 |
    | 20 10 -- 10 |
    | 30 20 10 -- |
```

## Compilando e executando

Para compilar o programa, basta utilizar o comando `make` pelo terminal na pasta raiz, gerando o arquivo executável `mlp.cpp`.

Para executar, basta utilizar o seguinte comando, **substituindo INSTÂNCIA por qualquer instância dentro da pasta `./instances`**.

``` bash
./mlp instances/INSTANCIA
```

## Resultados

A presente implementação do algoritmo consegue encontrar todos os resultados ótimos, assim como os novos resultados ótimo, do seguinte [artigo](https://www.sciencedirect.com/science/article/abs/pii/S037722171200269X).

## TODO

* Criar tabela com os resultados computacionais
