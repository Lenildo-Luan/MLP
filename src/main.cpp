  #include "readData.h"
  #include "Util.h"

  #include <string>
  #include <random>
  #include <bits/stdc++.h>
  #include <fstream>
  #include <iostream>
  #include <cmath>
  #include <algorithm>

  using namespace std;

  typedef struct{
    int pos;
    int vertice;
    int custo;
  } tInsercao;

  typedef struct{
    int posVertice;
    int posInsercao;
    int vertice;
  } tReinsercao;

  typedef struct{
    int pos1;
    int vertice1;
    int pos2;
    int vertice2;
  } tSwap;

  // Pega da instâncias
  double ** matrizAdj; // matriz de adjacencia
  int dimension; // quantidade total de vertices

  int qtdMelhorasReinsercao = 0;
  int qtdReinsercoes = 0;
  double tempoTotalReinsercao = 0;
  double tempoReinsercao = 0;

  int qtdMelhorasSwap = 0;
  double tempoTotalSwap = 0;
  double tempoSwap = 0;

  int qtdMelhorasTwoOpt = 0;
  double tempoTotalTwoOpt = 0;
  double tempoTwoOpt = 0;

  int qtdMelhorasDoubleBridge = 0;
  double tempoTotalDoubleBridge = 0;
  double tempoDoubleBridge = 0;

  // Untils
  void printData(); // Mostra matriz de adjacencia
  void printSolucao(vector<int> &solucao, int tamanhoArray); // Mostra a solução inicial gerada pel algorítimo escolhido
  void custoSolucao(int *custoTotal, vector<int> solucao, int tamanhoArray); // Mostra o custo da solução gerada
  void updateSubsequenceMatrix(vector< vector<int> > &subsequenceMatrix, vector< vector<int> > &routeMatrix, vector<int> &solution);
  void printSubsequenceMatrix(vector< vector<int> > &subsequenceMatrix);
  bool compareByCost(const tInsercao &data1, const tInsercao &data2);
  int custoAcumulate(vector<int> &solution);

  //GILS-RVND
  int construtivo(vector<int> &solucao, int coleta, int deposito, float alpha);
  int rvnd(vector<int> &solucao, int custoDaSolucaoAnterior, vector< vector<int> > &subsequenceMatrix, vector< vector<int> > acumulateMatrix);
  int gilsRvnd(vector<int> &solucao, int Imax, int Iils);

  // Vizinhanças
  int reinsertion(vector<int> &solucao, int blocoSize, int custoDaSolucaoAnterior, vector< vector<int> > &subsequenceMatrix, vector< vector<int> > acumulateMatrix);
  int swap(vector<int> &solucao, int custoDaSolucaoAnteriorm, vector< vector<int> > &subsequenceMatrix, vector< vector<int> > acumulateMatrix);
  int twoOptN(vector<int> &solucao, int custoDaSolucaoAnterior, vector< vector<int> > &subsequenceMatrix, vector< vector<int> > acumulateMatrix);

  //Pertubações
  int doubleBridge(vector<int> &solucao, int custoDaSolucaoAnterior);

  //MAIN
  int main(int argc, char** argv){
    //Get instance
    readData(argc, argv, &dimension, &matrizAdj);
    //printData();

    //Initi variables
    vector<int> solucao;
    int custoSoluca, custo, maxIls = (dimension > 150) ? dimension/2 : dimension;

    //Run algoritm
    custoSoluca = gilsRvnd(solucao, 50, maxIls);

    //Show results
    printSolucao(solucao, dimension);
    cout << "Custo: " << custoSoluca << endl;

    return 0;
  }

  //GILS-RVND
  int construtivo(vector<int> &solucao, int coleta, int deposito, float alpha){
    // Inicia variáveis
    vector<int> verticesList; // Lista de vertices faltando
    vector<tInsercao> bestVerticesList; // Lista dos X melhores verticesList

    random_device rd;
    mt19937 mt(rd()); // Gerador de números aleatórios
    tInsercao insercao;

    int rBest;
    int randomVertice1, randomVertice2, randomVertice3;
    int tamanhoSolucao;
    int verticesRestantes;
    int distanciaVertice;
    int custoSolucao = 0;

    //Adiciona coleta ao vector
    solucao.clear();
    solucao.push_back(coleta);

    // Gera lista de vertices faltantes
    for (size_t i = 1; i <= dimension; i++) {
      if(i == coleta || i == deposito) continue;
      verticesList.push_back(i);
    }

    // Escolhe três vertices de forma aleatória
    for (size_t i = 0; i < 3; i++) {
      uniform_int_distribution<int> linear_i(0, verticesList.size()-1); // Distribuição linear de inteiros para escolher vertice inicial
      randomVertice1 = linear_i(mt);

      solucao.push_back(verticesList[randomVertice1]);
      verticesList.erase(verticesList.begin() + randomVertice1);

      custoSolucao += matrizAdj[solucao[i]][solucao[i+1]];
    }

    // Adiciona deposito ao vector
    solucao.push_back(deposito);

    custoSolucao += matrizAdj[solucao[3]][solucao[4]];

    // Gera solução
    while(1) {
      tamanhoSolucao = solucao.size();
      verticesRestantes = verticesList.size();
      distanciaVertice = 0;

      // Gera lista de custo de vertices
      for (size_t i = 0; i < verticesRestantes; i++) { // Itera sobre vértices restantes
        for (size_t j = 1; j < tamanhoSolucao; j++) { // Itera sobre a solução
          insercao.vertice = verticesList[i];
          insercao.pos = j;
          insercao.custo = (matrizAdj[solucao[j-1]][verticesList[i]] + matrizAdj[verticesList[i]][solucao[j]]) - matrizAdj[solucao[j-1]][solucao[j]];
          bestVerticesList.push_back(insercao);
        }
      }

      //Arruma lista pelo valor do custo
      sort(bestVerticesList.begin(), bestVerticesList.end(), compareByCost);

      // Adiciona novo vertice à solução conforme alpha
      uniform_int_distribution<int> linear_i_alpha(0, int((bestVerticesList.size()-1)*alpha));
      rBest = linear_i_alpha(mt);
      solucao.emplace(solucao.begin() + bestVerticesList[rBest].pos, bestVerticesList[rBest].vertice);

      //Calcula custo
      custoSolucao += bestVerticesList[rBest].custo;

      // Reseta Vectors
      for (size_t i = 0; i < verticesRestantes; i++) {
        if(bestVerticesList[rBest].vertice == verticesList[i]) {
          verticesList.erase(verticesList.begin() + i);
          break;
        }
      }
      bestVerticesList.clear();

      // Se não tiverem mais candidatos o loop acaba
      if(verticesRestantes == 1) {
        break;
      }
    }

    return custoSolucao;
  }

  int rvnd(vector<int> &solucao, int custoDaSolucaoAnterior, vector< vector<int> > &subsequenceMatrix, vector< vector<int> > acumulateMatrix){
    vector<int> vizinhanca = {1, 2, 3, 4, 5};
    vector<int> vizinhancaInicial = vizinhanca;
    vector<int> solucaoTeste;

    random_device rd;
    mt19937 mt(rd());

    int solucaoSize = solucao.size();
    int vizinhancaSize = vizinhanca.size();
    int vizinhoRandom = 0;
    int random = 0;
    int custoAnterior = custoDaSolucaoAnterior;
    int novoCusto = custoAnterior;

    solucaoTeste = solucao;

    while(1){
      uniform_int_distribution<int> linear_i(0, vizinhanca.size() - 1);
      random = linear_i(mt);
      vizinhoRandom = vizinhanca[random];

      switch(vizinhoRandom){
        case 1:
          novoCusto = reinsertion(solucaoTeste, 1, custoAnterior, subsequenceMatrix, acumulateMatrix);
          break;
        case 2:
          novoCusto = reinsertion(solucaoTeste, 2, custoAnterior, subsequenceMatrix, acumulateMatrix);
          break;
        case 3:
          novoCusto = reinsertion(solucaoTeste, 3, custoAnterior, subsequenceMatrix, acumulateMatrix);
          break;
        case 4:
          novoCusto = swap(solucaoTeste, custoAnterior, subsequenceMatrix, acumulateMatrix);
          break;
        case 5:
          novoCusto = twoOptN(solucaoTeste, custoAnterior, subsequenceMatrix, acumulateMatrix);
          break;
        default:
          break;
      }

      if(novoCusto < custoAnterior){
        custoAnterior = novoCusto;

        solucao.clear();
        solucao = solucaoTeste;

        vizinhanca.clear();
        vizinhanca = vizinhancaInicial;

      } else {
        vizinhanca.erase(vizinhanca.begin() + random);
      }

      if(vizinhanca.empty()) break;

      updateSubsequenceMatrix(subsequenceMatrix, acumulateMatrix, solucaoTeste);
    }

    return custoAnterior;
  }

  int gilsRvnd(vector<int> &solucaoFinal, int Imax, int Iils){
    //Declara Variáveis
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<float> linear_f(0.0, 0.5); // Distribuição linear de reais para gerar alpha

    vector<int> solucaoParcial;
    vector<int> solucaoMelhor;
    vector< vector<int> > subsequenceMatrix, acumulateMatrix;

    int custoFinal = INT32_MAX;
    int custoParcial = 0;
    int custoMelhor = 0;
    int coleta = 1;
    int deposito = 1;
    int interILS;
    int solucaoSize;
    float alpha = 0.0;

    //Busca Melhor Solução
    for (size_t i = 0; i < Imax; i++) {
      //Gera Alfa e o interador de ILS
      alpha = linear_f(mt);
      interILS = 0;

      //Calcula solução parcial com o método construtivo
      solucaoParcial.clear();
      construtivo(solucaoParcial, coleta, deposito, alpha);
      updateSubsequenceMatrix(subsequenceMatrix, acumulateMatrix, solucaoParcial);
      custoParcial = acumulateMatrix[0][dimension];
      solucaoSize = solucaoParcial.size();

      //Registra a solução parcial como melhor solução
      custoMelhor = custoParcial;
      solucaoMelhor = solucaoParcial;

      //Busca o melhor ótimo local a partir da solução encontrada no construtivo
      while(interILS < Iils){
        //Busca melhor ótimo local da solução parcial
        custoParcial = rvnd(solucaoParcial, custoParcial, subsequenceMatrix, acumulateMatrix);

        //Testa se houve melhora
        if(custoParcial < custoMelhor){
          //Registra a solução parcial como melhor solução
          custoMelhor = custoParcial;
          solucaoMelhor = solucaoParcial;
          qtdMelhorasDoubleBridge++;

          //Zera o iterador
          interILS = 0;
        }

        //Pertuba a solução
        solucaoParcial = solucaoMelhor;
        custoParcial = custoMelhor;
        doubleBridge(solucaoParcial, custoParcial);

        //Soma o interador
        interILS++;

        updateSubsequenceMatrix(subsequenceMatrix, acumulateMatrix, solucaoParcial);
        custoParcial = acumulateMatrix[0][dimension];
      }

      //Testa se houve melhora
      if(custoMelhor < custoFinal){
        custoFinal = custoMelhor;
        solucaoFinal = solucaoMelhor;
      }

      tempoTotalReinsercao += tempoReinsercao;
    }

    updateSubsequenceMatrix(subsequenceMatrix, acumulateMatrix, solucaoFinal);

    return custoFinal;
  }

  // Vizinhanças
  int reinsertion(vector<int> &solucao, int blocoSize, int custoDaSolucaoAnterior, vector< vector<int> > &subsequenceMatrix, vector< vector<int> > acumulateMatrix){
    // Inicia variáveis
    vector<int> aux;

    int solucaoSize = solucao.size();
    int iterSize = solucaoSize - blocoSize;
    bool flag = false;
    tReinsercao insercao;

    //MLP
    int finalAcumulateCost = custoDaSolucaoAnterior, bestAcumulateCost = custoDaSolucaoAnterior;
    int finalSubsequenceCost = 0, bestSubsequence = 0;
    int c1, c2, t1, t2;
    int ac0_1, ac1_2, sc1_2, ac1_1, tpv, blocoSizepi;

    //Procura local de melhor reinserção
    for(int  i = 1; i < iterSize; i++){
      blocoSizepi = i+blocoSize;
      ac0_1 = acumulateMatrix[0][i-1], ac1_2 = acumulateMatrix[i][blocoSizepi-1];
      ac1_1 = acumulateMatrix[i][i+blocoSize-1];
      tpv = subsequenceMatrix[0][i-1] + matrizAdj[solucao[i-1]][solucao[i+blocoSize]];
      sc1_2 = subsequenceMatrix[i][i+blocoSize-1];

      for(int y = 0; y < i - 1; y++){
        c1 = acumulateMatrix[0][y] + (blocoSize) * (subsequenceMatrix[0][y] + matrizAdj[solucao[y]][solucao[i]]) + ac1_1;
        t1 = subsequenceMatrix[0][y] + matrizAdj[solucao[y]][solucao[i]] + subsequenceMatrix[i][i+blocoSize-1];

        c2 = c1 + (i-1-(y)) * (t1 + matrizAdj[solucao[i+blocoSize-1]][solucao[y+1]]) + acumulateMatrix[y+1][i-1];
        t2 = t1 + matrizAdj[solucao[i+blocoSize-1]][solucao[y+1]] + subsequenceMatrix[y+1][i-1];

        finalAcumulateCost = c2 + ((dimension)-(i+blocoSize-1)) * (t2+matrizAdj[solucao[i-1]][solucao[i+blocoSize]]) + acumulateMatrix[i+blocoSize][dimension];
        finalSubsequenceCost = t2 + matrizAdj[solucao[i-1]][solucao[i+blocoSize]] + subsequenceMatrix[i+blocoSize][dimension];

        if(finalAcumulateCost < bestAcumulateCost){
          flag = true;

          bestAcumulateCost = finalAcumulateCost;
          bestSubsequence = finalSubsequenceCost;
          insercao.posVertice = i;
          insercao.posInsercao = y+1;
          insercao.vertice = solucao[i];
        }
      }

      for(int y = i + blocoSize + 1; y < solucaoSize - 1; y++){
        c1 = ac0_1 + ((y)-(blocoSizepi-1)) * (tpv) + acumulateMatrix[blocoSizepi][y];
        t1 = tpv + subsequenceMatrix[blocoSizepi][y];

        c2 = c1 + blocoSize * (t1 + matrizAdj[solucao[y]][solucao[i]]) + ac1_2;
        t2 = t1 + matrizAdj[solucao[y]][solucao[i]] + sc1_2;

        finalAcumulateCost = c2 + (dimension-(y)) * (t2+matrizAdj[solucao[i+blocoSize-1]][solucao[y+1]]) + acumulateMatrix[y+1][dimension];
        finalSubsequenceCost = t2 + matrizAdj[solucao[i+blocoSize-1]][solucao[y+1]] + subsequenceMatrix[y+1][dimension];

        if(finalAcumulateCost < bestAcumulateCost){
          flag = true;

          bestAcumulateCost = finalAcumulateCost;
          bestSubsequence = finalSubsequenceCost;
          insercao.posVertice = i;
          insercao.posInsercao = y+1;
          insercao.vertice = solucao[i];
        }
      }
    }

    //Aplica reinserção
    if(flag){
      for(size_t i = 0; i < blocoSize; i++){
        aux.push_back(solucao[insercao.posVertice]);
        solucao.erase(solucao.begin() + insercao.posVertice);
      }

      if(insercao.posInsercao > insercao.posVertice) insercao.posInsercao -= blocoSize;

      solucao.insert(solucao.begin() + insercao.posInsercao, aux.begin(), aux.begin() + aux.size());

      flag = true;
    }

    return bestAcumulateCost;
  }

  int swap(vector<int> &solucao, int custoDaSolucaoAnterior, vector< vector<int> > &subsequenceMatrix, vector< vector<int> > acumulateMatrix){
    //Inicia variáveis
    int solucaoSize = solucao.size();
    bool flag = false;
    tSwap swap;

    //MLP
    int finalAcumulateCost = custoDaSolucaoAnterior, bestAcumulateCost = custoDaSolucaoAnterior;
    int finalSubsequenceCost = 0, bestSubsequence = 0;
    int c1, c2, c3, t1, t2, t3;

    //Aplica reinserção
    for(int i = 1; i < solucaoSize; i++){
      for(int y = i+2; y < solucaoSize - 1; y++){

        c1 = acumulateMatrix[0][i-1] + (subsequenceMatrix[0][i-1] + matrizAdj[solucao[i-1]][solucao[y]]);
        t1 = subsequenceMatrix[0][i-1] + matrizAdj[solucao[i-1]][solucao[y]];

        c2 = c1 + (y-1-i) * (t1 + matrizAdj[solucao[y]][solucao[i+1]]) +  acumulateMatrix[i+1][y-1];
        t2 = t1 + matrizAdj[solucao[y]][solucao[i+1]] + subsequenceMatrix[i+1][y-1];

        c3 = c2 + (t2+matrizAdj[solucao[y-1]][solucao[i]]);
        t3 = t2 + matrizAdj[solucao[y-1]][solucao[i]];

        finalAcumulateCost = c3 + (dimension-y) * (t3+matrizAdj[solucao[i]][solucao[y+1]]) + acumulateMatrix[y+1][dimension];
        finalSubsequenceCost = t3 + matrizAdj[solucao[i]][solucao[y+1]] + subsequenceMatrix[y+1][dimension];

        if(finalAcumulateCost < bestAcumulateCost){
          flag = true;

          bestSubsequence = finalSubsequenceCost;
          bestAcumulateCost = finalAcumulateCost;
          swap.pos1 = i;
          swap.vertice1 = solucao[i];
          swap.pos2 = y;
          swap.vertice2 = solucao[y];
        }
      }
    }

    if(flag){
      solucao.erase(solucao.begin() + swap.pos2);
      solucao.emplace(solucao.begin() + swap.pos2, swap.vertice1);

      solucao.erase(solucao.begin() + swap.pos1);
      solucao.emplace(solucao.begin() + swap.pos1, swap.vertice2);

      flag = false;
    }

    return bestAcumulateCost;
  }

  int twoOptN(vector<int> &solucao, int custoDaSolucaoAnterior, vector< vector<int> > &subsequenceMatrix, vector< vector<int> > acumulateMatrix){
    //Inicia variáveis
    int aux = 0;
    int solucaoSize = solucao.size();
    int sizeSwap;
    double tempoInicial = cpuTime();
    bool flag = false;
    tSwap swap;

    //MLP
    int finalAcumulateCost = 0, bestAcumulateCost = custoDaSolucaoAnterior;
    int finalSubsequence = 0, bestSubsequence = 0;
    int c1, t1;

    for(size_t i = 0; i < solucaoSize; i++){
      for(size_t y = i + 2; y < solucaoSize; y++){
        c1 = acumulateMatrix[0][i] + (y-1-i) * (subsequenceMatrix[0][i] + matrizAdj[solucao[i]][solucao[y-1]]) + acumulateMatrix[y-1][i+1];
        t1 = subsequenceMatrix[0][i] + matrizAdj[solucao[i]][solucao[y-1]] + subsequenceMatrix[y-1][i+1];

        finalAcumulateCost = c1 + ((dimension)-(y-1)) * (t1+matrizAdj[solucao[i+1]][solucao[y]]) + acumulateMatrix[y][dimension];
        finalSubsequence = t1 + matrizAdj[solucao[i+1]][solucao[y]] + subsequenceMatrix[y][dimension];

        if(finalAcumulateCost < bestAcumulateCost){
          flag = true;

          bestSubsequence = finalSubsequence;
          bestAcumulateCost = finalAcumulateCost;
          swap.pos1 = i + 1;
          swap.vertice1 = solucao[i];
          swap.pos2 = y - 1;
          swap.vertice2 = solucao[y];
        }
      }
    }

    if(flag){
      sizeSwap = swap.pos2 - swap.pos1;

      for(size_t i = 0; i < sizeSwap; i++){
        aux = solucao[swap.pos2];
        solucao.erase(solucao.begin() + swap.pos2);
        solucao.emplace(solucao.begin() + swap.pos1 + i, aux);
      }

      flag = true;
    }

    return bestAcumulateCost;
  }

  //Pertubações
  int doubleBridge(vector<int> &sol, int cost){
    random_device rd;
    mt19937_64 mt(rd());
    uniform_int_distribution<int> linear_bme20(2, dimension / 3);
    uniform_int_distribution<int> linear_bma20(2, dimension / 10);

    vector<int> subSeq1, subSeq2;

    int sizeSubSeq1, sizeSubSeq2;
    int initSubSeq1, initSubSeq2;
    int initialCost, finalCost, deltaCost = 0;

    sizeSubSeq1 = linear_bme20(mt);
    sizeSubSeq2 = linear_bme20(mt);
    
    uniform_int_distribution<int> linear_p1(1, sol.size() - sizeSubSeq1 - 1);
    uniform_int_distribution<int> linear_p2(1, sol.size() - sizeSubSeq2 - 1);

    while(1){
      initSubSeq1 = linear_p1(mt);
      initSubSeq2 = linear_p2(mt);

      if((initSubSeq2 <= (initSubSeq1 - sizeSubSeq2) && initSubSeq1 > sizeSubSeq2) || (initSubSeq2 >= (initSubSeq1 + sizeSubSeq1) && ((sol.size() - 1) - initSubSeq1 - (sizeSubSeq1 - 1)) > sizeSubSeq2)) break;
    }

    for(size_t i = 0; i < sizeSubSeq1; i++){
      subSeq1.push_back(sol[initSubSeq1 + i]);
    }

    for(size_t i = 0; i < sizeSubSeq2; i++){
      subSeq2.push_back(sol[initSubSeq2 + i]);
    }

    if(initSubSeq1 > initSubSeq2){
      if((initSubSeq1 - sizeSubSeq2) == initSubSeq2){
        deltaCost = (matrizAdj[sol[initSubSeq2 - 1]][sol[initSubSeq1]] + matrizAdj[sol[initSubSeq1 + sizeSubSeq1 - 1]][sol[initSubSeq2]] + matrizAdj[sol[initSubSeq2 + sizeSubSeq2 - 1]][sol[initSubSeq1 + sizeSubSeq1]]) -
                    (matrizAdj[sol[initSubSeq2 - 1]][sol[initSubSeq2]] + matrizAdj[sol[initSubSeq2 + sizeSubSeq2 - 1]][sol[initSubSeq1]] + matrizAdj[sol[initSubSeq1 + sizeSubSeq1 - 1]][sol[initSubSeq1 + sizeSubSeq1]]);
      } else {
        initialCost = (matrizAdj[sol[initSubSeq1 - 1]][sol[initSubSeq1 + sizeSubSeq1]] + matrizAdj[sol[initSubSeq2 - 1]][sol[initSubSeq2 + sizeSubSeq2]]) - 
                      (matrizAdj[sol[initSubSeq1 - 1]][sol[initSubSeq1]] + matrizAdj[sol[initSubSeq1 + sizeSubSeq1 - 1]][sol[initSubSeq1 + sizeSubSeq1]] + 
                        matrizAdj[sol[initSubSeq2 - 1]][sol[initSubSeq2]] + matrizAdj[sol[initSubSeq2 + sizeSubSeq2 - 1]][sol[initSubSeq2 + sizeSubSeq2]]);

        finalCost =(matrizAdj[sol[initSubSeq1 - 1]][sol[initSubSeq2]] + matrizAdj[sol[initSubSeq2 + sizeSubSeq2 - 1]][sol[initSubSeq1 + sizeSubSeq1]] + 
                    matrizAdj[sol[initSubSeq2 - 1]][sol[initSubSeq1]] + matrizAdj[sol[initSubSeq1 + sizeSubSeq1 - 1]][sol[initSubSeq2 + sizeSubSeq2]]) - 
                    (matrizAdj[sol[initSubSeq1 - 1]][sol[initSubSeq1 + sizeSubSeq1]] + matrizAdj[sol[initSubSeq2 - 1]][sol[initSubSeq2 + sizeSubSeq2]]);

        deltaCost = finalCost + initialCost;
      }

      sol.erase(sol.begin() + initSubSeq2, sol.begin() + initSubSeq2 + (sizeSubSeq2));
      sol.insert(sol.begin() + initSubSeq2, subSeq1.begin(), subSeq1.begin() + subSeq1.size());

      sol.erase(sol.begin() + initSubSeq1 + (sizeSubSeq1 - sizeSubSeq2), sol.begin() + initSubSeq1 + (sizeSubSeq1 - sizeSubSeq2) + (sizeSubSeq1));
      sol.insert(sol.begin() + initSubSeq1 + (sizeSubSeq1 - sizeSubSeq2), subSeq2.begin(), subSeq2.begin() + subSeq2.size());

    } else {
      if(initSubSeq1 + sizeSubSeq1 == initSubSeq2){
        deltaCost = (matrizAdj[sol[initSubSeq1 - 1]][sol[initSubSeq2]] + matrizAdj[sol[initSubSeq2 + sizeSubSeq2 - 1]][sol[initSubSeq1]] + matrizAdj[sol[initSubSeq1 + sizeSubSeq1 - 1]][sol[initSubSeq2 + sizeSubSeq2]]) -
                    (matrizAdj[sol[initSubSeq1 - 1]][sol[initSubSeq1]] + matrizAdj[sol[initSubSeq1 + sizeSubSeq1 - 1]][sol[initSubSeq2]] + matrizAdj[sol[initSubSeq2 + sizeSubSeq2 - 1]][sol[initSubSeq2 + sizeSubSeq2]]);
      
      } else {
        initialCost = (matrizAdj[sol[initSubSeq1 - 1]][sol[initSubSeq1 + sizeSubSeq1]] + matrizAdj[sol[initSubSeq2 - 1]][sol[initSubSeq2 + sizeSubSeq2]]) - 
                      (matrizAdj[sol[initSubSeq1 - 1]][sol[initSubSeq1]] + matrizAdj[sol[initSubSeq1 + sizeSubSeq1 - 1]][sol[initSubSeq1 + sizeSubSeq1]] + 
                        matrizAdj[sol[initSubSeq2 - 1]][sol[initSubSeq2]] + matrizAdj[sol[initSubSeq2 + sizeSubSeq2 - 1]][sol[initSubSeq2 + sizeSubSeq2]]);

        finalCost =(matrizAdj[sol[initSubSeq1 - 1]][sol[initSubSeq2]] + matrizAdj[sol[initSubSeq2 + sizeSubSeq2 - 1]][sol[initSubSeq1 + sizeSubSeq1]] + 
                    matrizAdj[sol[initSubSeq2 - 1]][sol[initSubSeq1]] + matrizAdj[sol[initSubSeq1 + sizeSubSeq1 - 1]][sol[initSubSeq2 + sizeSubSeq2]]) - 
                    (matrizAdj[sol[initSubSeq1 - 1]][sol[initSubSeq1 + sizeSubSeq1]] + matrizAdj[sol[initSubSeq2 - 1]][sol[initSubSeq2 + sizeSubSeq2]]);

        deltaCost = finalCost + initialCost;
      }

      sol.erase(sol.begin() + initSubSeq1, sol.begin() + initSubSeq1 + (sizeSubSeq1));
      sol.insert(sol.begin() + initSubSeq1, subSeq2.begin(), subSeq2.begin() + subSeq2.size());

      sol.erase(sol.begin() + initSubSeq2 + (sizeSubSeq2 - sizeSubSeq1), sol.begin() + initSubSeq2 + (sizeSubSeq2 - sizeSubSeq1) + (sizeSubSeq2));
      sol.insert(sol.begin() + initSubSeq2 + (sizeSubSeq2 - sizeSubSeq1), subSeq1.begin(), subSeq1.begin() + subSeq1.size());
    }

    return cost + deltaCost;
  }

  // Untils
  void printData() {
    cout << endl << "dimension: " << dimension << endl;
    for (size_t i = 1; i <= dimension; i++) {
      for (size_t j = 1; j <= dimension; j++) {
        cout << matrizAdj[i][j] << " ";
      }
      cout << endl;
    }
  }

  void printSolucao(vector<int> &solucao, int tamanhoArray){
    cout << endl << "Solucao: [ ";

    for(size_t i = 0; i < solucao.size(); i++){
      cout << solucao[i] << " ";
    }

    cout << "]" << endl;
  }

  void custoSolucao(int *custoTotal, vector<int> solucao, int tamanhoArray) {
    *custoTotal = 0;

    for(size_t i = 0; i < solucao.size()-1; i++){
      *custoTotal += matrizAdj[solucao[i]][solucao[i + 1]];
    }
    cout << endl;
  }

  int custoAcumulate(vector<int> &solution){
    int custo = 0, aux = 0;
    for(int i = 0; i < solution.size(); i++){
      for(int j = 1; j <= i; j++){
        aux += matrizAdj[solution[j-1]][solution[j]];
        cout << aux << " - ";
      }
      custo += aux;
      cout << "| " << custo << endl;
      aux = 0;
    }

    return custo;
  }

  void updateSubsequenceMatrix(vector< vector<int> > &subsequenceMatrix, vector< vector<int> > &acumulateMatrix, vector<int> &solution){
    int solutionSize = solution.size(), subsequenceCost = 0;
    vector<int> row, row2;

    if(subsequenceMatrix.size() > 0) subsequenceMatrix.clear();
    if(acumulateMatrix.size() > 0) acumulateMatrix.clear();

    for(int i = 0; i < solutionSize; i++){
      row.push_back(0);
      row2.push_back(0);

      for(int j = dimension-i; j < dimension; j++){
        subsequenceCost += matrizAdj[solution[dimension-j]][solution[dimension-(j+1)]];
        row.emplace(row.begin(), subsequenceCost);
        row2.emplace(row2.begin(), row2[0] + row[0]);
      }

      subsequenceCost = 0;

      for(int j = i; j < solutionSize-1; j++){
        subsequenceCost += matrizAdj[solution[j]][solution[j+1]];
        row.push_back(subsequenceCost);
        row2.push_back(row2[j] + row[j+1]);
      }

      subsequenceCost = 0;
      subsequenceMatrix.push_back(row);
      acumulateMatrix.push_back(row2);
      row.clear();
      row2.clear();
    }
  }

  void printSubsequenceMatrix(vector< vector<int> > &subsequenceMatrix){
    for(int i = 0; i < subsequenceMatrix.size(); i++){
      for(int j = 0; j < subsequenceMatrix.size(); j++){
        if(subsequenceMatrix[i][j] > 10000) cout << " ";
        else if(subsequenceMatrix[i][j] > 1000 && subsequenceMatrix[i][j] < 10000) cout << "  ";
        else if(subsequenceMatrix[i][j] > 100 && subsequenceMatrix[i][j] < 1000) cout << "   ";
        else if(subsequenceMatrix[i][j] < 100 && subsequenceMatrix[i][j] > 10) cout << "    ";
        else cout << "     ";
        cout << subsequenceMatrix[i][j];
      }
      cout << endl;
    }
    cout << endl;
  }

  bool compareByCost(const tInsercao &data1, const tInsercao &data2){
    return data1.custo < data2.custo;
  }
