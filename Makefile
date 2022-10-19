CXX = g++

main: main.o Arbol_R.o Entrada_Hoja.o Entrada_Interna.o Entrada.o Interfaz.o 
	$(CXX) bin/out/*.o -o bin/release/main -lsfml-graphics -lsfml-window -lsfml-system

Arbol_R.o:
	$(CXX) -o bin/out/Arbol_R.o -c lib/Arbol_R.cc

Entrada_Hoja.o:
	$(CXX) -o bin/out/Entrada_Hoja.o -c lib/Entrada_Hoja.cc

Entrada_Interna.o:
	$(CXX) -o bin/out/Entrada_Interna.o -c lib/Entrada_Interna.cc

Entrada.o:
	$(CXX) -o bin/out/Entrada.o -c lib/Entrada.cc

Interfaz.o:
	$(CXX) -o bin/out/Interfaz.o -c lib/Interfaz.cc

main.o:
	$(CXX) -o bin/out/main.o -c src/main.cc

run: main
	./bin/release/main

clean:
	rm bin/release/*
	rm bin/out/* 
	rm bin/debug/*