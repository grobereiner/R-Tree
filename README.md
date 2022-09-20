# R-Tree

## Comandos make

### ```make```
Se obtiene el compilado en el directorio ```bin/release```

### ```make run```
Compila y ejecuta el programa

### ```make clean```
Elimina archivos generados por otros comandos **make**

## Variables de entorno para ejecución en WSL

1. Descargar XLaunch
2. Setear el siguiente comando si se utiliza WSL:
```
export DISPLAY=$(route.exe print | grep 0.0.0.0 | head -1 | awk '{print $4}'):0.0
```

## Clases

### R_MBR
Clase encargada de la región de mínima expansión que contenga otros nodos

### R_Nodo
Clase que tiene un nodo con tuplas o con (MBR, R_Nodo hijos)

### R_Tree
Clase que representa al R-Tree, contiene métodos para la inserción, eliminación y visualización en desmos o en sfml.

