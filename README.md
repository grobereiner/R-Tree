# R-Tree

## Comandos make

### ```make```
Se obtiene el compilado en el directorio ```bin/debug```

### ```make run```
Compila y ejecuta el programa

### ```make display```
Define el valor de la variable de entorno DISPLAY

### ```make clean```
Elimina archivos generados por otros comandos **make**

## Clases

### R_MBR
Clase encargada de la región de mínima expansión que contenga otros nodos

### R_Nodo
Clase que tiene un nodo con tuplas o con (MBR, R_Nodo hijos)

### R_Tree
Clase que representa al R-Tree, contiene métodos para la inserción, eliminación y visualización en desmos o en sfml.

