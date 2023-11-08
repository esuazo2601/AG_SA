#include "greedy-probabilista.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <random> 
using namespace std;
int index_crash = 0;
int mutaciones = 0;

//VALORES DE PARAMETROS DEFAULT
int pob_size=500;
int tournament_size = 5;
int max_tries=25;
int mutation_rate = 10;
int determinismo = 5;
int tiempo_max_segundos = 60;
vector<string> inst;

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

pair<string,int> tournament(vector<pair<string, int>> population) {
    int random_parent1 = rand()%population.size();
    int random_parent2 = rand()%population.size();

    pair<string,int> parent1 = population[random_parent1];
    pair<string,int> parent2 = population[random_parent2];
    //SI los seleccionados son iguales o alguno de los 2 fueron padres entra
    while(parent1.first == parent2.first){
        random_parent2 = rand()%population.size();
        parent2 = population[random_parent2];
    }

    if(parent1.second < parent2.second){
        //fue_padre[random_parent1] = true;
        return parent1;
    }else{
        //fue_padre[random_parent2] = true;
        return parent2;
    }
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

vector<pair<string,int>>doGeneration(vector<pair<string,int>>population){
    int pop_size = population.size();
    vector<pair<string,int>> new_gen;
    for(int i=0;i<pop_size;i++){
        //SELECCIONAR PADRES
        pair<string,int> father = tournament(population);
        pair<string,int> mother = tournament(population);
        //CROSSOVER
        string descendiente = cruzar(father,mother);        
        int cost = hamming_cuadrado_a(descendiente,inst);
        new_gen.push_back(make_pair(descendiente, cost));
    }
    return new_gen;
}

void AG(int tournament_size, vector<string> inst, int i, int m, int l) {
    vector<pair<string, int>> poblacion;
    vector<bool>fue_padre;
    for (int i=0; i < pob_size; i++){
        poblacion.push_back(Greedy_probabilista(inst,determinismo));
        fue_padre.push_back(false);
    }

    pair<string,int> greatest = make_pair("",numeric_limits<int>::max());
    vector<pair<string,int>> new_gen;
    new_gen.reserve(poblacion.size());

    double tiempo_greatest = numeric_limits<double>::max(); 
    double time = 0;
    std::chrono::time_point<std::chrono::system_clock> start, end, found_solution;
    start = std::chrono::system_clock::now();
    while(time <= tiempo_max_segundos){
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> interval1 = (end - start);
        time = interval1.count();
        
        if (time >= tiempo_max_segundos) break;

        new_gen = doGeneration(poblacion);

        //fue_padre = {false};

        pair<string, int>generation_best = find_fittest(new_gen);

        if(generation_best.second < greatest.second){
            greatest = generation_best;
            std::cout << greatest.second <<endl;
            found_solution = std::chrono::system_clock::now();
            std::chrono::duration<double> intervalo = (found_solution - start);
            tiempo_greatest = intervalo.count();
            cout<<"Actualice: " <<tiempo_greatest<<endl;
        }

        if (tiempo_greatest == numeric_limits<double>::max()) {
            tiempo_greatest = 0;
        }
    
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> interval2 = (end - start);
        time = interval2.count();
        poblacion = new_gen;
    }

    /*    std::cout<<"MUTACIONES: "<<mutaciones<<"  "<<"INDEX_CRASH: "<<index_crash<<endl;
    std::cout<<greatest.first<< "    "<< greatest.second<<endl;
    std::cout<<"TIEMPO: "<<tiempo_greatest<<endl; 
    */

    std::cout << greatest.second<<endl;

    printf("%d ;%d ;%d ;%d ;%.2f\n", i, m, l, greatest.second, tiempo_greatest);
}

int main(int argc, char* argv[]) { 
    std::mt19937 generator(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    int m = atoi(argv[1]);
    int l = atoi(argv[2]);
    std::cout << "inst ;" << " m;" << " l;" << " ga;" << " gatime" << endl;
    for (int i = 1; i < 100; i++) {
        string instancia = "inst_"+ to_string(m)+"_"+to_string(l)+"_4_" + to_string(i) + ".txt";
        inst = lee_instancia(instancia);
        int string_size = inst.at(0).length();
        if (tournament_size > pob_size){
            std::cout<<"El tamaño del torneo debe ser menor al de la poblacion"<<endl;
            return 1;
        }
        AG(tournament_size, inst, i, m, l);
    }
    return 0;
}