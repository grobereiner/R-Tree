compilar:
	g++ main.cc -o main

run:
	./main > desmos.txt

clean:
	rm *.txt main

