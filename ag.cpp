#include "greedy-probabilista.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include <chrono>
using namespace std;
int tournament_remake = 0;
int mutaciones = 0;

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

pair<string,string> tournament (int k, vector<pair<string, int>>&pob){
    int pob_size = pob.size();
    random_shuffle(pob.begin(),pob.end());
    vector<pair<string, int>> selected_candidates;

    // Selección de k candidatos únicos de manera aleatoria
    while (selected_candidates.size() < k) {
        int random_index = rand() % pob_size;
        selected_candidates.push_back(pob[random_index]);
    }

    // Barajea los candidatos de manera aleatoria
    random_shuffle(selected_candidates.begin(), selected_candidates.end());

    string father, mother;
    father = selected_candidates[0].first;
    mother = selected_candidates[1].first;

    if(father == mother){
        tournament_remake++;
        return tournament(k,pob);
    } 

    return make_pair(father,mother);
}

string cruzar(pair<string, string> parents){
    int punto_de_cruce = rand()%parents.first.length();
    string descendiente = parents.first.substr(0, punto_de_cruce) + parents.second.substr(punto_de_cruce);
    // Tiene Un 5% de Prob de mutar
    int random_int = rand()%100;
    if (random_int <= 5){
        mutaciones++;
        int random_index = rand()%descendiente.size();
        descendiente[random_index] = pick_random_letter();
    }
    return descendiente; 
}

int main(int argc, char* argv[]) { 
    srand(time(nullptr));
    if (argc != 11 || string(argv[1]) != "-i" || string(argv[3]) != "-t" || 
        string(argv[5]) != "-d" || string(argv[7]) != "-p" 
        || string(argv[9]) != "-k" ) {

        cout << "Uso incorrecto. -i <instancia> -t <tiempo maximo> -d <determinismo> -p <población inicial> -k <tamaño torneo>" << endl;
        return 1; // Código de error
    }

    vector<string>inst = lee_instancia(string(argv[2]));
    int string_size = inst.at(0).length();
    int determinismo = atoi(argv[6]);
    vector<pair<string, int>> poblacion;
    int pob_size = atoi(argv[8]);
    int tournament_size = atoi(argv[10]);
    const int tiempo_max_segundos = atoi(argv[4]);

    if (tournament_size > pob_size){
        cout<<"El tamaño del torneo debe ser menor al de la poblacion"<<endl;
        return 1;
    }

    for (int i=0; i < pob_size; i++){
        poblacion.push_back(Greedy_probabilista(inst,determinismo));
    }
    sort(poblacion.begin(),poblacion.end(),sortbysec);
    pair<string,int> greatest = poblacion.at(0);
    double tiempo_greatest = numeric_limits<double>::max();
    
    auto tiempoInicio = chrono::high_resolution_clock::now();
    bool condicion_de_parada = false;
    while(!condicion_de_parada){
        auto tiempoActual = chrono::high_resolution_clock::now();
        
        for(int i=0;i<pob_size;i++){
            pair<string,string>parents = tournament(tournament_size,poblacion);
            string desendencia = cruzar(parents);
            poblacion[i] = make_pair(desendencia,hamming_cuadrado_a(desendencia,inst));
        }

        sort(poblacion.begin(),poblacion.end(),sortbysec);
        pair<string, int>generation_best = poblacion.at(0);
    
        auto duracion = chrono::duration_cast<std::chrono::seconds>(tiempoActual - tiempoInicio);        
        if(generation_best.second <= greatest.second){ //  -23000 MEJOR QUE -24000
            greatest = generation_best;
            tiempo_greatest = duracion.count();
        }
        if (tiempo_greatest == numeric_limits<double>::max()) {
            tiempo_greatest = 0;
        }
        
        if (duracion.count() >= tiempo_max_segundos) {
            condicion_de_parada = true; // Termina la ejecución del algoritmo
        }
        

    }

    cout<<"MUTACIONES: "<<mutaciones<<"  "<<"REMAKES: "<<tournament_remake<<endl;
    cout<<greatest.first<< "    "<< greatest.second<<endl;
    cout<<"TIEMPO: "<<tiempo_greatest<<endl;
    return 0;
}