#include "greedy-probabilista.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>

using namespace std;

int distancia(char a, char b);
char menor_distancia(vector<pair<int, char>>& dist, int j);
vector<string>mat;

int generate_random_value(){
    int randomValue = rand()%100 + 1;
    return randomValue;
}

pair<string,int> Greedy_probabilista(vector<string>mat, int deter_int){  
    string ADN = "ATCG"; //Para selección random
    unordered_map<char, int> frec;
    
    int lenY = mat.size();
    int lenX = mat.at(0).length();
    string resultado(lenX, ' '); // Inicializa la solución con espacios
    
    //Si el numero generado es mayor al grado de determinismo
    if(generate_random_value() > deter_int){
        int index = rand()%4;
        resultado[0] =  ADN[index]; //Se añade una letra aleatoria 
    }else{ //de lo contrario se añade la que mas se repite como primera entrada
        for (int i = 0; i <lenY ; i++) {
            char letra = mat[i][0];
            if (frec.find(letra) != frec.end()) {
                frec[letra]++;
            } else {
                frec[letra] = 1;
            }
        }

        int max_rep = 0;
        char letra;
        for (auto caracter : frec) {
            if (caracter.second > max_rep) {
                letra = caracter.first;
                max_rep = caracter.second;
            }
        }
        resultado[0] = letra;
    }

    vector<pair<int, char>> dist(mat.size(), make_pair(0, 'A'));
    //Se calculan las distancias para la primera columna 
    for (int i = 0; i < lenY; i++) { 
        dist[i].first = dist[i].first + distancia(mat[i][0], resultado[0]);
        dist[i].second = mat[i][0];
    }

    //Se itera por columnas y se agrega al vector respuesta aquella letra que tenga menor distancia
    for (int i = 1; i < lenX; i++) {
        for (int j = 0; j < lenY; j++) {
            dist[j].first += distancia(mat[j][i], resultado[i - 1]); //distancia numerica
            dist[j].second = mat[j][i]; //letra en cuestión
        }
        if(generate_random_value() < deter_int) //Si el numero generado es menor al grado de determinismo, se añade la de menor distanica
            resultado[i] = menor_distancia(dist, i);
        else{ //de lo contrario se añade una letra aleatoria
            int random_index = rand()%4;
            resultado[i] = ADN[random_index];
        }
    }

    int costo_total = 0;
    for(int i=0;i<lenY; i++){
        int dist_2 = dist[i].first*dist[i].first;
        costo_total += dist_2;
    }
    return make_pair(resultado,costo_total);
}
//Se retorna 0 si las letras son iguales, de lo contrario 1, se usa para sumar distancia.
int distancia(char a, char b) {
    if (a == b) {
        return 0;
    } else {
        return 1;
    }
}

//Función que determina cual es la letra con menor distancia
char menor_distancia(vector<pair<int, char>>& dist, int j) {
    int len = dist.size();
    int menor = dist[0].first;
    char letra = dist[0].second;
    for (int i = j; i < len; i++) {
        if (dist[i].first < menor) {
            menor = dist[i].first;
            letra = dist[i].second;
        }
    }
    return letra;
}
