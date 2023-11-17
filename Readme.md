# Proyecto desarrollado por:
- Elizabeth Bravo Campos
- Gustavo González Gutierrez
- Esteban Suazo Montalba

En este proyecto se implementó una metaheurística poblacional con torneo y cruza uniforme.

## Contenido de la entrega:
1. greedy-probabilista.cpp : Es el algoritmo greedy que se usa para obtener una población inicial.
1. ag.cpp : Es el algoritmo genético finalmente implementado, diseñado para correr una instancia a la vez.
1. ag_dataset.cpp : Son los mismos algoritmos que en el archivo anterior pero adaptados para correr 100 instancias en una ejecución, de tamaño m x l.

## Parámetros:
1. En el caso de ag.cpp los parámetros usables son:

* -i : Indica la instancia a ejecutar.
* -t : Indica el tiempo maximo en segundos de ejecucion del algoritmo, está por defecto en 60.
* -d : El grado de determinismo que usará el greedy-probabilista, por defecto en 5.
* -p : El tamaño de población que tomará el algoritmo, por defecto en 500.
* -k : Define el tamaño de cada torneo del algoritmo, por defecto en 5.
* -mu : Define la probabilidad de mutación de un hijo al realizar la cruza, por defecto en 10.

2. En el caso de ag_dataset.cpp los parámetros usables son:
* Se pasa por parámetros los tamaños de m y l. 

## Instrucciones de compilación
- Si desea compilar ag para probar la implementación con una sola instancia:
```
g++ ag.cpp greedy-probabilista.cpp -o <Nombre archivo salida>
```
- Si por otro lado desea compilar GraspDataset para ejecutar 100 instancias de una vez:
```
g++ ag_dataset.cpp greedy-probabilista.cpp -o <Nombre archivo salida>
```

## Instrucciones de ejecución
- Una vez creado el/los ejecutables en el caso de ag se tiene:
```
./<Nombre archivo salida> -i <instancia> -t <tiempo> -d <determinismo> -p <pob_size> -k <tournament_size> -mu <mutation_rate>
```
- En el caso de ag_dataset.cpp
```
./<Nombre archivo salida> <numero_strings> <largo_string>
```
**Cabe destacar que las 100 instancias de cada tipo deben estar en una carpeta donde su nombre sea <M L> junto al archivo ag_dataset**
- Por ejemplo, para probar instancias de 200x15, debe tener una carpeta llamada 200 15, con sus 100 instancias dentro, y ejecutar ag_dataset con los parámetros 200 15
