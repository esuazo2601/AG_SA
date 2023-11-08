#include "greedy-probabilista.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <iomanip> 
using namespace std;
int index_crash = 0;
int mutaciones = 0;

//VALORES DE PARAMETROS DEFAULT
int pob_size=500;
int tournament_size = 100;
int max_tries=25;
int mutation_rate = 5;
int determinismo = 5;
int tiempo_max_segundos = 5;

int hamming_cuadrado_a(string sol_in, vector<string>entrada){
    int len = sol_in.length();
    int len_entrada = entrada.size();
    vector<int>costos;

    for (int i=0;i<len_entrada;i++){
        int costo = 0;
        for (int j=0 ; j<len ; j++){
            if(sol_in[j] != entrada[i][j]){
                costo++;
            }
        }
        costos.push_back(costo);
    }

    int costos_len = costos.size();
    int costo_total=0;

    for (int i=0 ;i<costos_len ;i++){
        costo_total += costos[i]*costos[i];
    }
    return costo_total;
}

char pick_random_letter(){
    string DNA = "ATCG";
    return DNA [rand()%DNA.size()];
}

vector<string> lee_instancia(string nombre){
    ifstream archivo_txt(nombre);
    vector<string> mat;
    string linea;
    while (getline(archivo_txt, linea)) {
        mat.push_back(linea);
    }
    archivo_txt.close();
    return mat;
}

bool sortbysec(const pair<string,int> &a, const pair<string,int> &b){
    return (a.second < b.second);
}

pair<string, string> tournament(int tournament_size, vector<pair<string, int>> population, vector<string>inst) {
    pair<string, string> parents;
    vector<pair<string,int>>candidatesF;
    vector<pair<string,int>>candidatesM;
    int max_tries_copy = max_tries;

    for (int i=0; i<tournament_size; i++){
        int index_father = rand()%pob_size;
        int index_mother = rand()%pob_size;
        
        while (index_father == index_mother && max_tries_copy > 0){
            index_crash++;
            max_tries_copy--;
            index_mother = rand()%pob_size;
        }
        string cand_father = population[index_father].first;
        candidatesF.push_back(make_pair (cand_father, hamming_cuadrado_a(cand_father,inst)));

        string cand_mother = population[index_mother].first;
        candidatesM.push_back(make_pair (cand_mother, hamming_cuadrado_a(cand_mother,inst)));
    }
    sort(candidatesM.begin(),candidatesM.end(), sortbysec);
    sort(candidatesF.begin(),candidatesF.end(), sortbysec);
    parents.first = candidatesM.at(0).first;
    parents.second = candidatesF.at(0).first;
    return parents;
}

string cruzar(pair<string, string> parents){
    int punto_de_cruce = rand()%parents.first.length();
    string descendiente = parents.first.substr(0, punto_de_cruce) + parents.second.substr(punto_de_cruce);
    // Tiene Un 5% de Prob de mutar
    int random_int = rand()%100;
    if (random_int <= mutation_rate){
        mutaciones++;
        int random_index = rand()%descendiente.size();
        descendiente[random_index] = pick_random_letter();
    }
    return descendiente; 
}

void AG(int tournament_size, vector<string> inst, int i, int m, int l) {
    vector<pair<string, int>> poblacion;
    for (int i = 0; i < pob_size; i++) {
        poblacion.push_back(Greedy_probabilista(inst, determinismo));
    }

    sort(poblacion.begin(), poblacion.end(), sortbysec);
    pair<string, int> greatest = poblacion.at(0);
    double tiempo_greatest = numeric_limits<double>::max();
    double time = 0;  // Inicializar time a 0
    std::chrono::time_point<std::chrono::system_clock> start, end, found_solution;

    start = std::chrono::system_clock::now();
    while (time <= tiempo_max_segundos) {
        for (int i = 0; i < pob_size; i++) {
            pair<string, string> parents = tournament(tournament_size, poblacion, inst);
            string descendencia = cruzar(parents);
            poblacion[i] = make_pair(descendencia, hamming_cuadrado_a(descendencia, inst));
        }

        sort(poblacion.begin(), poblacion.end(), sortbysec);
        pair<string, int> generation_best = poblacion.at(0);

        if (generation_best.second < greatest.second) {  // Cambiar la condición
            greatest = generation_best;
            found_solution = std::chrono::system_clock::now();
            std::chrono::duration<double> intervalo = (found_solution - start);
            tiempo_greatest = intervalo.count();
        }

        end = std::chrono::system_clock::now();
        std::chrono::duration<double> interval = (end - start);
        time = interval.count();
    }

    printf("%d ;%d ;%d ;%d ;%.2f\n", i, m, l, greatest.second, tiempo_greatest);
}



int main(int argc, char* argv[]) { 
    srand(time(nullptr));

    
    int m = atoi(argv[1]);
    int l = atoi(argv[2]);

    std::cout << "inst ;" << " m;" << " l;" << " ga;" << " gatime" << endl;
    for (int i = 0; i < 100; i++) {
        string instancia = "inst_"+ to_string(m)+"_"+to_string(l)+"_4_" + to_string(i) + ".txt";
        vector<string>inst = lee_instancia(instancia);
        int string_size = inst.at(0).length();

        if (tournament_size > pob_size){
            std::cout<<"El tamaño del torneo debe ser menor al de la poblacion"<<endl;
            return 1;
        }
        AG(tournament_size, inst, i, m, l);
    }
    return 0;
}