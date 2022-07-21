all: build run

build:
	sudo g++ -o main main.cpp Arm.cpp Joint.cpp -lpigpio -lrt -lpthread

run:
	sudo ./main
	
clean: 
	rm -f main
