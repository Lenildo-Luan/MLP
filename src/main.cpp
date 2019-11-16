  #include "readData.h"
  #include "Util.h"
  #include "json.hpp"

  #include <string>
  #include <random>
  #include <bits/stdc++.h>
  #include <fstream>
  #include <iostream>
  #include <cmath>
  #include <algorithm>

  using namespace std;
  using json = nlohmann::json;

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

  //Benchmark
  json benchmarkReinsercao; // Json
  json benchmarkSwap; // Json
  json benchmarkTwoOpt; // Json
  json benchmarkDoubleBridge; // Json
  json benchmark; // Json

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
  int doubleBridge2(vector<int> &solucao, int custoDaSolucaoAnterior);
  int arnandoBridge(int N, vector<int> &solucaoTop, vector<int> &solucaoILS, int *indexFirstTimeInicio, int *indexFirstTimeFinal);

  //MAIN
  int main(int argc, char** argv){
    readData(argc, argv, &dimension, &matrizAdj);
    printData();

    vector<int> solucao;
    vector<int> arnandoSolucao;
    int arnando1 = 0, arnando2 = 0;

    int custoSoluca, custo;
    int flag = 0;
    int maxIls = (dimension > 150) ? dimension/2 : dimension;
    double tempoInicial;

    // for(size_t i = 0; i < 10; i++){
      tempoInicial = cpuTime();

      custoSoluca = gilsRvnd(solucao, 50, maxIls);

      //printSolucao(solucao, dimension);
      cout << "Custo: " << custoSoluca << endl;
      // custoSolucao(&custo, solucao, solucao.size());
      // cout << "Custo rota: " << custo << endl;
      // cout << "Custo teste: " << custoAcumulate(solucao) << endl;
      cout << "Tempo total: " << cpuTime() - tempoInicial << endl;
      cout << "Tempo das reinsercoes: " << tempoTotalReinsercao << endl;
      cout << "Tempo dos swaps: " << tempoTotalSwap << endl;
      cout << "Tempo dos two opt n: " << tempoTotalTwoOpt<< endl;
      cout << "Tempo dos double bridges: " << tempoTotalDoubleBridge << endl;

      tempoTotalReinsercao = 0;
      tempoTotalDoubleBridge = 0;
      tempoTotalSwap = 0;
      tempoTotalTwoOpt = 0;

      tempoInicial = 0;

      cout << endl;
    // }

      //Declara variáveis do método construtivo
      // random_device rd;
      // mt19937 mt(rd());
      // uniform_real_distribution<float> linear_f1(0.0, 0.5);
      // int coleta = 1;
      // int deposito = 1;
      // float alpha = 0.0;

      //Gera solução inicial
      //while(1){
        // vector< vector<int> > subsequenceMatrix, acumulateMatrix;
        // alpha = linear_f1(mt);
        // custoSoluca = construtivo(solucao, coleta, deposito, alpha);
        // printSolucao(solucao, dimension);
        // cout << endl;
        //
        // updateSubsequenceMatrix(subsequenceMatrix, acumulateMatrix, solucao);
        // printSubsequenceMatrix(subsequenceMatrix);
        // printSubsequenceMatrix(acumulateMatrix);
        // custoSoluca = twoOptN(solucao, acumulateMatrix[0][dimension], subsequenceMatrix, acumulateMatrix);
        // updateSubsequenceMatrix(subsequenceMatrix, acumulateMatrix, solucao);
        //
        // custoSolucao(&custo, solucao, solucao.size());
        // cout << "Result: " << custoSoluca << endl;
        // cout << "Custo: " << custo << endl;

        //if(custoSoluca != custo) break;
      //}

      //Benchmark
      benchmark["tempo"] = cpuTime() - tempoInicial;

      //Benchmark
      string arquivo = argv[1];
      string instancia;
      string barra ("/");
      string ponto (".");

      //Benchmark
      for(size_t i = 0; i < arquivo.size(); i++){
        if(flag == 2 && arquivo[i] == ponto[0]) break;
        else if(flag == 2) instancia.push_back(arquivo[i]);
        else if(arquivo[i] == barra[0]) flag++;
      }

      //Benchmark
      ofstream benchmarkArchiveReinsercao("archive/" + instancia + "_reinsercao_benchmark" + ".json");
      ofstream benchmarkArchiveSwap("archive/" + instancia + "_swap_benchmark" + ".json");
      ofstream benchmarkArchiveTwoOpt("archive/" + instancia + "_twoOpt_benchmark" + ".json");
      ofstream benchmarkArchiveDoubleBridge("../archive/" + instancia + "_doubleBridge_benchmark" + ".json");
      ofstream benchmarkArchive("archive/" + instancia + "_time" + ".json");

      //Benchmark
      benchmarkArchiveReinsercao << setw(4) << benchmarkReinsercao << endl;
      benchmarkArchiveSwap << setw(4) << benchmarkSwap << endl;
      benchmarkArchiveTwoOpt << setw(4) << benchmarkTwoOpt << endl;
      benchmarkArchiveDoubleBridge << setw(4) << benchmarkDoubleBridge << endl;
      benchmarkArchive << setw(4) << benchmark << endl;

      return 0;
  }

  //GILS-RVND
  int construtivo(vector<int> &solucao, int coleta, int deposito, float alpha){
    // Inicia variáveis
    vector<int> verticesList; // Lista de vertices faltando
    vector<tInsercao> bestVerticesList; // Lista dos X melhores verticesList

    random_device rd;
    mt19937 mt(rd()); // Gerador de números aleatórios

    int rBest;
    int randomVertice1, randomVertice2, randomVertice3;
    int tamanhoSolucao;
    int verticesRestantes;
    int distanciaVertice;
    int custoSolucao = 0;
    tInsercao insercao;

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

    // // Mostra solução
    // for (size_t i = 0; i < solucao.size(); i++){
    //   cout << solucao[i] << " ";
    //   //cout << solucaoInicial[i] << " ";
    // }

  }

  int rvnd(vector<int> &solucao, int custoDaSolucaoAnterior, vector< vector<int> > &subsequenceMatrix, vector< vector<int> > acumulateMatrix){
    vector<int> vizinhanca = {1, 2, 3, 4, 5};
    vector<int> vizinhancaInicial;
    vector<int> solucaoTeste;

    vizinhancaInicial = vizinhanca;

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
        //cout << "MELHORA RVND: " << novoCusto << endl;
        custoAnterior = novoCusto;

        solucao.clear();
        solucao = solucaoTeste;

        vizinhanca.clear();
        vizinhanca = vizinhancaInicial;

      } else {
        vizinhanca.erase(vizinhanca.begin() + random);
      }

      //printSolucao(solucao, solucao.size());
      //cout << novoCusto << endl;

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
    vector<int> solucaoArnando;
    vector< vector<int> > subsequenceMatrix, acumulateMatrix;

    int custoFinal = INT32_MAX;
    int custoParcial = 0;
    int custoMelhor = 0;
    int coleta = 1;
    int deposito = 1;
    int interILS;
    int solucaoSize;
    int arnando1 = 0, arnando2 = 0;
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
      //cout << "CUSTO INICIAL: " << custoParcial << endl;
      solucaoSize = solucaoParcial.size();

      //Registra a solução parcial como melhor solução
      custoMelhor = custoParcial;
      solucaoMelhor = solucaoParcial;
      solucaoArnando = solucaoParcial;

      //Busca o melhor ótimo local a partir da solução encontrada no construtivo
      while(interILS < Iils){
        //Busca melhor ótimo local da solução parcial
        custoParcial = rvnd(solucaoParcial, custoParcial, subsequenceMatrix, acumulateMatrix);

        //Testa se houve melhora
        if(custoParcial < custoMelhor){
          //Registra a solução parcial como melhor solução
          custoMelhor = custoParcial;
          solucaoMelhor = solucaoParcial;
          //cout << "MELHORA ILS: " << custoMelhor << endl;
          qtdMelhorasDoubleBridge++;

          //Zera o iterador
          interILS = 0;
        }

        //Pertuba a solução
        //custoParcial = doubleBridge(solucaoParcial, custoParcial);
        solucaoParcial = solucaoMelhor;
        custoParcial = custoMelhor;
        doubleBridge2(solucaoParcial, custoParcial);
        // arnando1 = 0, arnando2 = 0;
        // solucaoArnando = solucaoParcial;
        // arnandoBridge(dimension, solucaoParcial, solucaoArnando, &arnando1, &arnando2);
        //custoSolucao(&custoParcial,  solucaoParcial,  solucaoParcial.size());

        //Soma o interador
        interILS++;

        updateSubsequenceMatrix(subsequenceMatrix, acumulateMatrix, solucaoParcial);
        custoParcial = acumulateMatrix[0][dimension];
      }

      //Testa se houve melhora
      if(custoMelhor < custoFinal){
        custoFinal = custoMelhor;
        solucaoFinal = solucaoMelhor;
        cout << "MELHORA GRASP: " << custoFinal << endl;
      }

      //Benchmark
      benchmarkReinsercao[i]["Iteração"] = to_string(i);
      benchmarkReinsercao[i]["qtdMelhoras"] = qtdMelhorasReinsercao;
      benchmarkReinsercao[i]["tempo"] = tempoReinsercao;

      tempoTotalReinsercao += tempoReinsercao;
      qtdMelhorasReinsercao = 0;
      tempoReinsercao = 0;

      //Benchmark
      benchmarkSwap[i]["Iteração"] = to_string(i);
      benchmarkSwap[i]["qtdMelhoras"] = qtdMelhorasSwap;
      benchmarkSwap[i]["tempo"] = tempoSwap;

      tempoTotalSwap += tempoSwap;
      qtdMelhorasSwap = 0;
      tempoSwap = 0;

      //Benchmark
      benchmarkTwoOpt[i]["Iteração"] = to_string(i);
      benchmarkTwoOpt[i]["qtdMelhoras"] = qtdMelhorasTwoOpt;
      benchmarkTwoOpt[i]["tempo"] = tempoTwoOpt;

      tempoTotalTwoOpt += tempoTwoOpt;
      qtdMelhorasTwoOpt = 0;
      tempoTwoOpt = 0;

      //Benchmark
      benchmarkDoubleBridge[i]["Iteração"] = to_string(i);
      benchmarkDoubleBridge[i]["qtdMelhoras"] = qtdMelhorasDoubleBridge;
      benchmarkDoubleBridge[i]["tempo"] = tempoDoubleBridge;

      tempoTotalDoubleBridge += tempoDoubleBridge;
      qtdMelhorasDoubleBridge = 0;
      tempoDoubleBridge = 0;

    }

    updateSubsequenceMatrix(subsequenceMatrix, acumulateMatrix, solucaoFinal);

    cout << endl << "Latence: " << acumulateMatrix[0][dimension] << endl;

    //printSubsequenceMatrix(acumulateMatrix);
    //printSubsequenceMatrix(subsequenceMatrix);

    return custoFinal;
  }

  // Vizinhanças
  int reinsertion(vector<int> &solucao, int blocoSize, int custoDaSolucaoAnterior, vector< vector<int> > &subsequenceMatrix, vector< vector<int> > acumulateMatrix){
    // Inicia variáveis
    vector<int> aux;

    int deltaCusto = 0, custoRetirada = 0, custoInsercao = 0, custoDaSolucao = custoDaSolucaoAnterior, solucaoSize = solucao.size();
    int iterSize = solucaoSize - blocoSize;
    double tempoInicial = cpuTime();
    bool flag = false;
    tReinsercao insercao;

    //MLP
    int finalAcumulateCost = custoDaSolucaoAnterior, bestAcumulateCost = custoDaSolucaoAnterior;
    int finalSubsequenceCost = 0, bestSubsequence = 0;
    int c1, c2, t1, t2;

    qtdReinsercoes++;

    //Procura local de melhor reinserção
    for(int  i = 1; i < iterSize; i++){
      //custoRetirada = matrizAdj[solucao[i-1]][solucao[i+blocoSize]] - (matrizAdj[solucao[i-1]][solucao[i]] + matrizAdj[solucao[i+blocoSize-1]][solucao[i+blocoSize]]);

      for(int y = 0; y < solucaoSize - 1; y++){
        if(y >= (i-1) && y <= i + blocoSize) continue;

        //TODO: Passar contas que só dependem de I para o for acima
        //TODO: Passar parte da conta que calcula o bloco para fora dos fors
        //if(i < y) continue;
        if(i < y){
          c1 = acumulateMatrix[0][i-1] + ((y)-(i+blocoSize-1)) * (subsequenceMatrix[0][i-1] + matrizAdj[solucao[i-1]][solucao[i+blocoSize]]) + acumulateMatrix[i+blocoSize][y];
          t1 = subsequenceMatrix[0][i-1] + matrizAdj[solucao[i-1]][solucao[i+blocoSize]] + subsequenceMatrix[i+blocoSize][y];
          //cout << "C1: " << acumulateMatrix[0][i-1] << " + " << ((y-1)-(i+blocoSize-1)) << " * " << (subsequenceMatrix[0][i-1] + matrizAdj[solucao[i-1]][solucao[i+blocoSize]]) << " + " << acumulateMatrix[i+blocoSize][y-1] << endl;

          c2 = c1 + blocoSize * (t1 + matrizAdj[solucao[y]][solucao[i]]) + acumulateMatrix[i][i+blocoSize-1];
          t2 = t1 + matrizAdj[solucao[y]][solucao[i]] + subsequenceMatrix[i][i+blocoSize-1];
          //cout << "C2: " << c1 << " + " << blocoSize << " * " << (t1 + matrizAdj[solucao[y-1]][solucao[i]]) << " + " << acumulateMatrix[i][i+blocoSize-1] << endl;

          finalAcumulateCost = c2 + (dimension-(y)) * (t2+matrizAdj[solucao[i+blocoSize-1]][solucao[y+1]]) + acumulateMatrix[y+1][dimension];
          finalSubsequenceCost = t2 + matrizAdj[solucao[i+blocoSize-1]][solucao[y+1]] + subsequenceMatrix[y+1][dimension];
          //cout << "FinalAcumulateCost: " << finalAcumulateCost << " = " << c2 << " + " << (dimension-1-(y-1)) << " * " << t2+matrizAdj[solucao[i+blocoSize-1]][solucao[y]] << " + " << acumulateMatrix[y][dimension] << endl;
          //cout << "Remove: " << i << endl;
          //cout << "Insert: " << y << endl;
          //cout << "finalAcumulateCost: " << finalAcumulateCost << endl;
        } else {
          c1 = acumulateMatrix[0][y] + (blocoSize) * (subsequenceMatrix[0][y] + matrizAdj[solucao[y]][solucao[i]]) + acumulateMatrix[i][i+blocoSize-1];
          t1 = subsequenceMatrix[0][y] + matrizAdj[solucao[y]][solucao[i]] + subsequenceMatrix[i][i+blocoSize-1];

          c2 = c1 + (i-1-(y)) * (t1 + matrizAdj[solucao[i+blocoSize-1]][solucao[y+1]]) + acumulateMatrix[y+1][i-1];
          t2 = t1 + matrizAdj[solucao[i+blocoSize-1]][solucao[y+1]] + subsequenceMatrix[y+1][i-1];

          finalAcumulateCost = c2 + ((dimension)-(i+blocoSize-1)) * (t2+matrizAdj[solucao[i-1]][solucao[i+blocoSize]]) + acumulateMatrix[i+blocoSize][dimension];
          finalSubsequenceCost = t2 + matrizAdj[solucao[i-1]][solucao[i+blocoSize]] + subsequenceMatrix[i+blocoSize][dimension];
          //cout << "finalAcumulateCost: " << finalAcumulateCost << endl;
        }

        //custoInsercao = (matrizAdj[solucao[y]][solucao[i]] + matrizAdj[solucao[i+blocoSize-1]][solucao[y+1]]) - matrizAdj[solucao[y]][solucao[y+1]];

        if(finalAcumulateCost < bestAcumulateCost){
          flag = true;
          //cout << "HOUVE MELHORA! " << finalAcumulateCost << endl;
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
      // cout << "=====================================================" << endl;
      // cout << "Remove: " << insercao.posVertice << " - " << solucao[insercao.posVertice] << endl;
      // cout << "Insert: " << insercao.posInsercao << " - " << solucao[insercao.posInsercao] << endl;

      custoDaSolucao = bestAcumulateCost;
      //cout << "bestAcumulateCost: " << bestAcumulateCost << endl;
      for(size_t i = 0; i < blocoSize; i++){
        aux.push_back(solucao[insercao.posVertice]);
        solucao.erase(solucao.begin() + insercao.posVertice);
      }

      if(insercao.posInsercao > insercao.posVertice) insercao.posInsercao -= blocoSize;

      solucao.insert(solucao.begin() + insercao.posInsercao, aux.begin(), aux.begin() + aux.size());

      //updateSubsequenceMatrix(subsequenceMatrix, acumulateMatrix, solucao);
      // cout << "Acumulate: " << acumulateMatrix[0][dimension] << " - " << bestAcumulateCost << endl;
      // cout << "Subsequence: " << subsequenceMatrix[0][dimension] << " - " << bestSubsequence << endl;


      deltaCusto = 0;
      flag = true;
    }

    //Benchmark
    if(flag){
      qtdMelhorasReinsercao++;
    }

    tempoReinsercao += cpuTime() - tempoInicial;

    return bestAcumulateCost;
  }

  int swap(vector<int> &solucao, int custoDaSolucaoAnterior, vector< vector<int> > &subsequenceMatrix, vector< vector<int> > acumulateMatrix){
    //Inicia variáveis
    int deltaCusto = 0, custoRetirada = 0, custoInsercao = 0, custoDaSolucao = custoDaSolucaoAnterior;
    int solucaoSize = solucao.size();
    double tempoInicial = cpuTime();
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
        //cout << "T1: " << subsequenceMatrix[0][i-1] << " + " << matrizAdj[solucao[i-1]][solucao[y]] << endl;

        c2 = c1 + (y-1-i) * (t1 + matrizAdj[solucao[y]][solucao[i+1]]) +  acumulateMatrix[i+1][y-1];
        t2 = t1 + matrizAdj[solucao[y]][solucao[i+1]] + subsequenceMatrix[i+1][y-1];
        //cout << "T2: " << t1 << " + " <<   matrizAdj[solucao[y]][solucao[i+1]] << " + " << subsequenceMatrix[i+1][y-1] << endl;

        c3 = c2 + (t2+matrizAdj[solucao[y-1]][solucao[i]]);
        t3 = t2 + matrizAdj[solucao[y-1]][solucao[i]];
        //cout << "T3: " << t2 << " + " << matrizAdj[solucao[y-1]][solucao[i]] << endl;

        finalAcumulateCost = c3 + (dimension-y) * (t3+matrizAdj[solucao[i]][solucao[y+1]]) + acumulateMatrix[y+1][dimension];
        finalSubsequenceCost = t3 + matrizAdj[solucao[i]][solucao[y+1]] + subsequenceMatrix[y+1][dimension];
        //cout << "finalSubsequenceCost: " << finalSubsequenceCost << " = " << t3 << " + " << matrizAdj[solucao[i]][solucao[y+1]] << " + " << subsequenceMatrix[y+1][dimension-1] << endl;

        // cout << "finalAcumulateCost: " << finalAcumulateCost << endl;
        // cout << "finalSubsequenceCost: " << finalSubsequenceCost << endl;
        //cout << "Remove: " << i << endl;
        //cout << "Insert: " << y << endl << endl;

        if(finalAcumulateCost < bestAcumulateCost){
          flag = true;
          //cout << "ENTROUASDJSLSDNJLFKS" << endl;
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
      custoDaSolucao = bestAcumulateCost;

      solucao.erase(solucao.begin() + swap.pos2);
      solucao.emplace(solucao.begin() + swap.pos2, swap.vertice1);

      solucao.erase(solucao.begin() + swap.pos1);
      solucao.emplace(solucao.begin() + swap.pos1, swap.vertice2);

      //updateSubsequenceMatrix(subsequenceMatrix, acumulateMatrix, solucao);
      //cout << "==================================================================================" << endl;
      //cout << "Acumulate: " << acumulateMatrix[0][dimension] << " - " << bestAcumulateCost << endl;
      //cout << "Subsequence: " << subsequenceMatrix[0][dimension] << " - " << bestSubsequence << endl;
      //cout << "pos1: " << swap.pos1 << endl;
      //cout << "pos2: " << swap.pos2 << endl;

      flag = false;
      deltaCusto = 0;
    }

    //Benchmark
    if(true){
      qtdMelhorasSwap++;
    }

    tempoSwap += cpuTime() - tempoInicial;

    return bestAcumulateCost;
  }

  int twoOptN(vector<int> &solucao, int custoDaSolucaoAnterior, vector< vector<int> > &subsequenceMatrix, vector< vector<int> > acumulateMatrix){
    //Inicia variáveis
    int deltaCusto = 0, custoRetirada = 0, custoInsercao = 0, custoDaSolucao = custoDaSolucaoAnterior;
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
        // custoRetirada = matrizAdj[solucao[i]][solucao[i+1]] + matrizAdj[solucao[y-1]][solucao[y]];
        // custoInsercao = matrizAdj[solucao[i]][solucao[y-1]] + matrizAdj[solucao[i+1]][solucao[y]];

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
      custoDaSolucao = acumulateMatrix[0][dimension];
      sizeSwap = swap.pos2 - swap.pos1;

      for(size_t i = 0; i < sizeSwap; i++){
        aux = solucao[swap.pos2];
        solucao.erase(solucao.begin() + swap.pos2);
        solucao.emplace(solucao.begin() + swap.pos1 + i, aux);
      }

      // updateSubsequenceMatrix(subsequenceMatrix, acumulateMatrix, solucao);
      // cout << "Acumulate: " << acumulateMatrix[0][dimension] << " - " << bestAcumulateCost << endl;
      // cout << "Subsequence: " << subsequenceMatrix[0][dimension] << " - " << bestSubsequence << endl;

      flag = true;
      deltaCusto = 0;

    }

    //Benchmark
    if(true){
      qtdMelhorasTwoOpt++;
    }

    tempoTwoOpt+= cpuTime() - tempoInicial;

    return bestAcumulateCost;
  }

  //Pertubações
  int doubleBridge(vector<int> &solucao, int custoDaSolucaoAnterior){
    random_device rd;
    mt19937 mt(rd());
    int sizeBlock = (solucao.size() - 2) / 3;
    uniform_int_distribution<int> linear_i(1, sizeBlock);
    uniform_int_distribution<int> linear_p1(1, ((solucao.size() - 2)/2) - sizeBlock);
    uniform_int_distribution<int> linear_p2(((solucao.size() - 2)/2) + 1, solucao.size() - 1 - sizeBlock);
    int bloco1 = linear_i(mt), bloco2 = linear_i(mt);
    int pos1 = linear_p1(mt), pos2 = linear_p2(mt);
    int custoInicial;
    int custoFinal;
    int deltaCusto = 0;
    int aux;
    double tempoInicial = cpuTime();

    custoInicial = (matrizAdj[solucao[pos1 - 1]][solucao[pos1 + bloco1]] + matrizAdj[solucao[pos2 - 1]][solucao[pos2 + bloco2]]) -
                   (matrizAdj[solucao[pos1 - 1]][solucao[pos1]] + matrizAdj[solucao[pos1 + bloco1 - 1]][solucao[pos1 + bloco1]] +
                    matrizAdj[solucao[pos2 - 1]][solucao[pos2]] + matrizAdj[solucao[pos2 + bloco2 - 1]][solucao[pos2 + bloco2]]);

    custoFinal =(matrizAdj[solucao[pos1 - 1]][solucao[pos2]] + matrizAdj[solucao[pos2 + bloco2 - 1]][solucao[pos1 + bloco1]] +
                 matrizAdj[solucao[pos2 - 1]][solucao[pos1]] + matrizAdj[solucao[pos1 + bloco1 - 1]][solucao[pos2 + bloco2]]) -
                (matrizAdj[solucao[pos1 - 1]][solucao[pos1 + bloco1]] + matrizAdj[solucao[pos2 - 1]][solucao[pos2 + bloco2]]);

    for(size_t i = 0; i < bloco1; i++){
      aux = solucao[pos1];
      solucao.emplace(solucao.begin() + pos2, aux);
      solucao.erase(solucao.begin() + pos1);
    }

    for(size_t i = 0; i < bloco2; i++){
      aux = solucao[pos2 + i];
      solucao.erase(solucao.begin() + pos2 + i);
      solucao.emplace(solucao.begin() + pos1 + i, aux);
    }

    deltaCusto = custoFinal + custoInicial;

    tempoDoubleBridge += cpuTime() - tempoInicial;

    return custoDaSolucaoAnterior + deltaCusto;
  }

  int doubleBridge2(vector<int> &solucao, int custoDaSolucaoAnterior){
    random_device rd;
    mt19937_64 mt(rd());
    uniform_int_distribution<int> linear_bme20(2, dimension / 3);
    uniform_int_distribution<int> linear_bma20(2, dimension / 10);

    vector<int> aux1, aux2;

    int bloco1, bloco2;
    int pos1, pos2;
    int custoInicial, custoFinal, deltaCusto = 0;
    double tempoInicial = cpuTime();

    // if(dimension < 20){
      bloco1 = linear_bme20(mt);
      bloco2 = linear_bme20(mt);
    // } else {
    //   bloco1 = linear_bma20(mt);
    //   bloco2 = linear_bma20(mt);
    // }

    uniform_int_distribution<int> linear_p1(1, solucao.size() - bloco1 - 1);
    uniform_int_distribution<int> linear_p2(1, solucao.size() - bloco2 - 1);

    while(1){
      pos1 = linear_p1(mt);
      pos2 = linear_p2(mt);

      if((pos2 <= (pos1 - bloco2) && pos1 > bloco2) || (pos2 >= (pos1 + bloco1) && ((solucao.size() - 1) - pos1 - (bloco1 - 1)) > bloco2)) break;
    }

    for(size_t i = 0; i < bloco1; i++){
      aux1.push_back(solucao[pos1 + i]);
    }

    for(size_t i = 0; i < bloco2; i++){
      aux2.push_back(solucao[pos2 + i]);
    }

    // cout << endl;
    // cout << pos1 << " " << bloco1 << " " << solucao[pos1] << endl << pos2 << " " << bloco2 << " " << solucao[pos2];
    // cout << endl;

    if(pos1 > pos2){
      if((pos1 - bloco2) == pos2){
        deltaCusto = (matrizAdj[solucao[pos2 - 1]][solucao[pos1]] + matrizAdj[solucao[pos1 + bloco1 - 1]][solucao[pos2]] + matrizAdj[solucao[pos2 + bloco2 - 1]][solucao[pos1 + bloco1]]) -
                    (matrizAdj[solucao[pos2 - 1]][solucao[pos2]] + matrizAdj[solucao[pos2 + bloco2 - 1]][solucao[pos1]] + matrizAdj[solucao[pos1 + bloco1 - 1]][solucao[pos1 + bloco1]]);
      } else {
        custoInicial = (matrizAdj[solucao[pos1 - 1]][solucao[pos1 + bloco1]] + matrizAdj[solucao[pos2 - 1]][solucao[pos2 + bloco2]]) -
                      (matrizAdj[solucao[pos1 - 1]][solucao[pos1]] + matrizAdj[solucao[pos1 + bloco1 - 1]][solucao[pos1 + bloco1]] +
                        matrizAdj[solucao[pos2 - 1]][solucao[pos2]] + matrizAdj[solucao[pos2 + bloco2 - 1]][solucao[pos2 + bloco2]]);

        custoFinal =(matrizAdj[solucao[pos1 - 1]][solucao[pos2]] + matrizAdj[solucao[pos2 + bloco2 - 1]][solucao[pos1 + bloco1]] +
                    matrizAdj[solucao[pos2 - 1]][solucao[pos1]] + matrizAdj[solucao[pos1 + bloco1 - 1]][solucao[pos2 + bloco2]]) -
                    (matrizAdj[solucao[pos1 - 1]][solucao[pos1 + bloco1]] + matrizAdj[solucao[pos2 - 1]][solucao[pos2 + bloco2]]);

        deltaCusto = custoFinal + custoInicial;
      }

      solucao.erase(solucao.begin() + pos2, solucao.begin() + pos2 + (bloco2));
      solucao.insert(solucao.begin() + pos2, aux1.begin(), aux1.begin() + aux1.size());

      solucao.erase(solucao.begin() + pos1 + (bloco1 - bloco2), solucao.begin() + pos1 + (bloco1 - bloco2) + (bloco1));
      solucao.insert(solucao.begin() + pos1 + (bloco1 - bloco2), aux2.begin(), aux2.begin() + aux2.size());

    } else {
      if(pos1 + bloco1 == pos2){
        deltaCusto = (matrizAdj[solucao[pos1 - 1]][solucao[pos2]] + matrizAdj[solucao[pos2 + bloco2 - 1]][solucao[pos1]] + matrizAdj[solucao[pos1 + bloco1 - 1]][solucao[pos2 + bloco2]]) -
                    (matrizAdj[solucao[pos1 - 1]][solucao[pos1]] + matrizAdj[solucao[pos1 + bloco1 - 1]][solucao[pos2]] + matrizAdj[solucao[pos2 + bloco2 - 1]][solucao[pos2 + bloco2]]);

      } else {
        custoInicial = (matrizAdj[solucao[pos1 - 1]][solucao[pos1 + bloco1]] + matrizAdj[solucao[pos2 - 1]][solucao[pos2 + bloco2]]) -
                      (matrizAdj[solucao[pos1 - 1]][solucao[pos1]] + matrizAdj[solucao[pos1 + bloco1 - 1]][solucao[pos1 + bloco1]] +
                        matrizAdj[solucao[pos2 - 1]][solucao[pos2]] + matrizAdj[solucao[pos2 + bloco2 - 1]][solucao[pos2 + bloco2]]);

        custoFinal =(matrizAdj[solucao[pos1 - 1]][solucao[pos2]] + matrizAdj[solucao[pos2 + bloco2 - 1]][solucao[pos1 + bloco1]] +
                    matrizAdj[solucao[pos2 - 1]][solucao[pos1]] + matrizAdj[solucao[pos1 + bloco1 - 1]][solucao[pos2 + bloco2]]) -
                    (matrizAdj[solucao[pos1 - 1]][solucao[pos1 + bloco1]] + matrizAdj[solucao[pos2 - 1]][solucao[pos2 + bloco2]]);

        deltaCusto = custoFinal + custoInicial;
      }

      solucao.erase(solucao.begin() + pos1, solucao.begin() + pos1 + (bloco1));
      solucao.insert(solucao.begin() + pos1, aux2.begin(), aux2.begin() + aux2.size());

      solucao.erase(solucao.begin() + pos2 + (bloco2 - bloco1), solucao.begin() + pos2 + (bloco2 - bloco1) + (bloco2));
      solucao.insert(solucao.begin() + pos2 + (bloco2 - bloco1), aux1.begin(), aux1.begin() + aux1.size());
    }

    //deltaCusto = custoFinal + custoInicial;

    tempoDoubleBridge += cpuTime() - tempoInicial;

    return custoDaSolucaoAnterior + deltaCusto;
  }

  int arnandoBridge(int N, vector<int> &solucaoTop, vector<int> &solucaoILS, int *indexFirstTimeInicio, int *indexFirstTimeFinal){
    int posicaoAleatoria1, posicaoAleatoria2, posicaoAleatoria3, posicaoAleatoria4;
    int custo = 0;

    //int length = (N)/25;
    int length = N/10;

    posicaoAleatoria1 = 1 + rand() % (N-2);

    if (posicaoAleatoria1  < N - 2 - length) {
      posicaoAleatoria2 = posicaoAleatoria1 + 1 + rand() % (length-1);
    }

    else {
      posicaoAleatoria2 = posicaoAleatoria1 + 1 + rand() % (N+1 - posicaoAleatoria1 - 2);
    }

    while (true) {
      posicaoAleatoria3 = 1 + rand() % (N - 2);

      if (posicaoAleatoria3  < N - 2 - length) {
        posicaoAleatoria4 = posicaoAleatoria3 + 1 + rand() % (length-1);
      }

      else {
        posicaoAleatoria4 = posicaoAleatoria3 + 1 + rand() % (N + 1 - posicaoAleatoria3 - 2);
      }

      if ((posicaoAleatoria4 < posicaoAleatoria1) || (posicaoAleatoria3 > posicaoAleatoria2)) {
        break;
      }
    }

    int ponto1, ponto2, ponto3, ponto4;
    if (posicaoAleatoria1 < posicaoAleatoria3) {
      ponto1 = posicaoAleatoria1;
      ponto2 = posicaoAleatoria2;
      ponto3 = posicaoAleatoria3;
      ponto4 = posicaoAleatoria4;
    }

    else {
      ponto1 = posicaoAleatoria3;
      ponto2 = posicaoAleatoria4;
      ponto3 = posicaoAleatoria1;
      ponto4 = posicaoAleatoria2;
    }

    // Executar a troca das duas particoes (emprestado de Bruno)

    int tampart1 = ponto2 - ponto1 + 1; // tamanho particao 1
    int tampart2 =  ponto4 - ponto3 + 1; // tamanho particao 2

    for (int i = 0 ; i < tampart2 ; i++) {
      solucaoTop[ponto1 + i] = solucaoILS[ponto3 + i];
    }

    for (int i = 0 ; i < ponto3-ponto2-1; i++) {
      solucaoTop[ponto1 + tampart2 + i] = solucaoILS[ponto2 +1 + i];
    }


    for (int i = 0 ; i < tampart1; i++)	{
      solucaoTop[ponto1 + tampart2 + ponto3 - ponto2 - 1 + i] = solucaoILS[ponto1 + i];
    }

    *indexFirstTimeInicio = ponto1;
    *indexFirstTimeFinal = ponto4;

    //custoSolucao(&custo, solucaoTop, solucaoTop.size());

    return custo;
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
      //  cout << *custoTotal << " - ";
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
