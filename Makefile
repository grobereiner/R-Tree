compilar:
	g++ main.cc -o main

run: compilar
	./main > desmos.txt

clean:
	rm *.txt main

