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

int generate_even_number(){
    int generated = 2 * rand() % 20;
    while (generated <= 0){
        generated = 2*rand() % 20;
    }
    return generated;
}

bool sortbysec(const pair<string,int> &a, const pair<string,int> &b){
    return (a.second < b.second);
}

pair<string,string> tournament (int k, vector<pair<string, int>>pob){
    int pob_size = pob.size();
    vector<pair<string, int>> bracket_fight_father;
    vector<pair<string, int>> bracket_fight_mother;
    
    for (int i=0; i < k ;i++){
        int random_index = rand()%pob_size;
        bracket_fight_father.push_back(pob[random_index]);
    }
    for (int i=0; i < k ;i++){
        int random_index = rand()%pob_size;
        bracket_fight_mother.push_back(pob[random_index]);
    }
    sort(bracket_fight_father.begin(),bracket_fight_father.end(), sortbysec);
    sort(bracket_fight_mother.begin(),bracket_fight_mother.end(), sortbysec);
    string father,mother;
    father = bracket_fight_father.at(0).first;
    mother = bracket_fight_mother.at(0).first;

    if(father == mother) return tournament(k,pob);

    return make_pair(father,mother);
}


string cruzar(pair<string, string> parents){
    int punto_de_cruce = rand()%parents.first.length();
    string descendiente = parents.first.substr(0, punto_de_cruce) + parents.second.substr(punto_de_cruce);
    int random_int = random()%100;
    if (random_int < 5){
        int random_index = random()%descendiente.size();
        descendiente[random_index] = pick_random_letter();
    }
    return descendiente;
}

int main(int argc, char* argv[]) { 
    srand(time(nullptr));
    if (argc != 11 || string(argv[1]) != "-i" || string(argv[3]) != "-t" || string(argv[5]) != "-d" || string(argv[7]) != "-p" || string(argv[9]) != "-k" ) {
        cout << "Uso incorrecto. -i <instancia> -t <tiempo maximo> -d <determinismo> -p <población inicial>" << endl;
        return 1; // Código de error
    }

    vector<string>inst = lee_instancia(string(argv[2]));
    int string_size = inst.at(0).length();
    int determinismo = atoi(argv[6]);
    vector<pair<string, int>> poblacion;
    int pob_size = atoi(argv[8]);
    int tournament_size = atoi(argv[10]);

    for (int i=0; i < pob_size; i++){
        poblacion.push_back(Greedy_probabilista(inst,determinismo));
    }
    
    return 0;
}