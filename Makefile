CXX = g++

main: main.o R_MBR.o R_Nodo.o R_Tree.o R_Info.o R_Poligono.o Interfaz.o
	$(CXX) bin/out/*.o -o bin/debug/main -lsfml-graphics -lsfml-window -lsfml-system

Interfaz.o:
	$(CXX) -o bin/out/Interfaz.o -c lib/Interfaz.cc

R_Poligono.o:
	$(CXX) -o bin/out/R_Poligono.o -c lib/R_Poligono.cc

R_Info.o:
	$(CXX) -o bin/out/R_Info.o -c lib/R_Info.cc

R_MBR.o:
	$(CXX) -o bin/out/R_MBR.o -c lib/R_MBR.cc

R_Nodo.o:
	$(CXX) -o bin/out/R_Nodo.o -c lib/R_Nodo.cc

R_Tree.o:
	$(CXX) -o bin/out/R_Tree.o -c lib/R_Tree.cc

main.o:
	$(CXX) -o bin/out/main.o -c src/main.cc

run: main
	./bin/debug/main

clean:
	rm bin/debug/*
	rm bin/out/* 
	rm *.txt 