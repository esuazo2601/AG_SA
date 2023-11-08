#include "greedy-probabilista.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <random>
using namespace std;
int index_crash = 0;
int mutaciones = 0;

//VALORES DE PARAMETROS DEFAULT
int pob_size=500;
int tournament_size = 5;
int mutation_rate = 10;
int determinismo = 5;
int tiempo_max_segundos = 60;

vector<string>inst;

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

vector<int>generate_mask(int largo_genes){
    vector<int>mask(largo_genes,0);
    int punto_cruce = largo_genes/2;
    while(punto_cruce > 0){
        int random_index = rand()%largo_genes;
        if(mask[random_index]!=1){
            mask[random_index] = 1;
            punto_cruce--;
        }
    }
    return mask;
}

/* 
vector<pair<string,int>> replace_population(vector<pair<string,int>>population,vector<pair<string,int>> &new_cromosomes){
    vector<pair<string,int>> new_gen = population;
    random_shuffle(new_cromosomes.begin(),new_cromosomes.end());

    for (int i=0;i<rep;i++){
        int rand_index = rand()%population.size();
        new_gen[rand_index] = new_cromosomes[i];
    }
    return new_gen;
}
*/

pair<string,int> find_fittest(vector<pair<string,int>> pob){
    int pob_size = pob.size();
    pair<string,int>fittest = make_pair("", numeric_limits<int>::max());
    for(int i=0; i<pob_size; i++){
        if(pob.at(i).second < fittest.second){
            fittest = pob.at(i);
        }
    }
    return fittest;
}

pair<string,int> tournament(vector<pair<string, int>> population, vector<bool>fue_padre) {
    vector<pair<string, int>>candidates;
    for(int i=0;i<tournament_size;i++){
        int random_parent = rand()%population.size();
        pair<string,int>candidate = population[random_parent];
        while(fue_padre[random_parent] == true){
            random_parent = rand()%population.size();
            candidate = population[random_parent];
        }
        candidates.push_back(candidate);
    }
    return find_fittest(candidates);
}

string cruzar(pair<string,int> parent1, pair<string,int>parent2){
    vector<int>mask = generate_mask(parent1.first.length());
    string descendiente(parent1.first.length(),'A');
    
    for(int i=0;i<mask.size();i++){
        if(mask[i]==0){
            descendiente[i] = parent1.first[i];
        }
        else{
            descendiente[i] = parent2.first[i];
        }
    }
    
    int random_int = rand()%100;
    if (random_int <= mutation_rate){
        mutaciones++;
        int random_index = rand()%descendiente.size();
        descendiente[random_index] = pick_random_letter();
    }
    return descendiente; 
}

vector<pair<string,int>>doGeneration(vector<pair<string,int>>population, vector<bool>fue_padre){
    int pop_size = population.size();
    vector<pair<string,int>> new_gen;
    for(int i=0;i<pop_size;i++){
        //SELECCIONAR PADRES
        pair<string,int> father = tournament(population, fue_padre);
        pair<string,int> mother = tournament(population, fue_padre);
        //CROSSOVER
        string descendiente = cruzar(father,mother);        
        int cost = hamming_cuadrado_a(descendiente,inst);
        new_gen.push_back(make_pair(descendiente, cost));
    }
    return new_gen;
}

int main(int argc, char *argv[]) {
    std::random_device rd;  // Utiliza una fuente de entropía aleatoria
    std::mt19937 generator(rd());
    if (argc >= 2 && string(argv[1]) == "-i") {
        inst = lee_instancia(string(argv[2]));
        int string_size = inst.at(0).length();
    } else {
        cout << "Debe proporcionar una instancia válida con -i." << endl;
        return 1; // Salir con un código de error
    }

    for (int i = 3; i < argc; i += 2) {
        if (i + 1 < argc) {
            if (string(argv[i]) == "-t") {
                tiempo_max_segundos = atoi(argv[i + 1]);
            } else if (string(argv[i]) == "-d") {
                determinismo = atoi(argv[i + 1]);
            } else if (string(argv[i]) == "-p") {
                pob_size = atoi(argv[i + 1]);
            } else if (string(argv[i]) == "-k") {
                tournament_size = atoi(argv[i + 1]);
            }  else if (string(argv[i]) == "-mu") {
                mutation_rate = atoi(argv[i + 1]);
            }
        }
    }

    if (tournament_size > pob_size) {
        std::cout << "El tamaño del torneo debe ser menor al de la población" << endl;
        return 1;
    }

    // SE CREA POBLACIÓN INICIAL
    vector<pair<string, int>> poblacion;
    vector<bool> fue_padre;
    for (int i = 0; i < pob_size; i++) {
        poblacion.push_back(Greedy_probabilista(inst, determinismo));
        fue_padre.push_back(false);
    }

    pair<string, int> greatest = find_fittest(poblacion);

    vector<pair<string, int>> new_gen;
    new_gen.reserve(poblacion.size());

    double tiempo_greatest = 0; // Inicializar tiempo_greatest en 0
    std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::system_clock::now();
    int gen_nueva = 0;

    while (tiempo_greatest <= tiempo_max_segundos) {  // Cambio en la condición
        new_gen = doGeneration(poblacion, fue_padre);
        
        pair<string, int> gen_best = find_fittest(new_gen);
        if (gen_best.second < greatest.second) {
            greatest = gen_best;
            std::chrono::time_point<std::chrono::system_clock> end_time = std::chrono::system_clock::now();
            std::chrono::duration<double> interval = (end_time - start_time);
            tiempo_greatest = interval.count();
            //cout << greatest.second << " " << tiempo_greatest << endl;
        }
        poblacion = new_gen;

        std::chrono::time_point<std::chrono::system_clock> end_time = std::chrono::system_clock::now();
        std::chrono::duration<double> interval2 = (end_time - start_time);
        tiempo_greatest = interval2.count();
    }
    std::cout << greatest.second << endl;
    return 0;
}