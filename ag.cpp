#include "greedy-probabilista.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

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

int generate_even_number(){
    int generated = 2 * rand() % 20;
    while (generated <= 0){
        generated = 2*rand() % 20;
    }
    return generated;
}

map<float, string, greater<float>> costToFitness(vector<pair<string, int>> pob) {
    map<float, string, greater<float>> sorted_map;
    int size = pob.size();
    for (int i = 0; i < size; i++) {
        float fit = 1.0 / pob[i].second; // Cambiado a float
        sorted_map.insert(make_pair(fit, pob[i].first));
    }
    return sorted_map;
}


map<float, string, greater<float>> cruzar(map<float, string, greater<float>>&mapa, vector<string>entrada){
    map<float, string, greater<float>> gen_nueva = mapa;
    int string_size = entrada.at(0).length();

    auto it = mapa.begin();
    while (it != mapa.end()) {
        // Accede al primer elemento
        float fitness1 = it->first;
        string individuo1 = it->second;

        // Avanza al siguiente elemento
        ++it;

        // Asegúrate de que todavía haya elementos en el mapa
        if (it != mapa.end()) {
            // Accede al segundo elemento
            float fitness2 = it->first;
            string individuo2 = it->second;

            // Realiza la operación de cruce (por ejemplo, un cruce de un solo punto)
            int punto_de_cruce = rand() % string_size;
            string descendiente = individuo1.substr(0, punto_de_cruce) + individuo2.substr(punto_de_cruce);

            // Agrega el descendiente al nuevo mapa
            float fit_descendiente = 1.0 / hamming_cuadrado_a(descendiente,entrada);
            gen_nueva.insert(make_pair(fit_descendiente,descendiente));
            // Avanza al siguiente elemento
            ++it;
        }
    }

    return gen_nueva;
}

int main(int argc, char* argv[]) { 
    srand(time(nullptr));
    if (argc != 9 || string(argv[1]) != "-i" || string(argv[3]) != "-t" || string(argv[5]) != "-d" || string(argv[7]) != "-p") {
        std::cout << "Uso incorrecto. -i <instancia> -t <tiempo maximo> -d <determinismo> -p <población inicial>" << endl;
        return 1; // Código de error
    }

    vector<string>inst = lee_instancia(string(argv[2]));
    int string_size = inst.at(0).length();
    int determinismo = atoi(argv[6]);
    vector<pair<string,int>> poblacion;
    int pob_size = atoi(argv[8]);
    
    for (int i=0; i < pob_size; i++){
        poblacion.push_back(Greedy_probabilista(inst,determinismo));
    }
    map<float,string,greater<float>>sorted_map = costToFitness(poblacion);
    cout<<"Generacion original"<<endl;
    for (auto it: sorted_map){
        cout<<it.first<<"     "<<it.second<<endl;
    }
    cout<<"Generacion 1"<<endl;
    map<float,string,greater<float>>gen1 = cruzar(sorted_map,inst);
    for (auto it: gen1){
        cout<<it.first<<"     "<<it.second<<endl;
    }
    
    return 0;
}