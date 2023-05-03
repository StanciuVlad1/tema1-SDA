build : 
	gcc -Wall -g  tema1.c -o tema1
run : 
	./tema1
.PHONY: clean
clean : 
	rm ./tema1