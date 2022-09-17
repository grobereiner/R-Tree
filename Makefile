CXX = g++

main: main.o R_MBR.o R_Nodo.o R_Tree.o
	$(CXX) -o bin/debug/main bin/out/main.o bin/out/R_MBR.o bin/out/R_Nodo.o bin/out/R_Tree.o

R_MBR.o:
	$(CXX) -o bin/out/R_MBR.o -c lib/R_MBR.cc

R_Nodo.o:
	$(CXX) -o bin/out/R_Nodo.o -c lib/R_Nodo.cc

R_Tree.o:
	$(CXX) -o bin/out/R_Tree.o -c lib/R_Tree.cc

main.o:
	$(CXX) -o bin/out/main.o -c src/main.cc

run: display main
	./bin/debug/main > desmos.txt

clean:
	rm bin/debug/*
	rm bin/out/* 
	rm *.txt 

display: 
	. commands/display.zsh