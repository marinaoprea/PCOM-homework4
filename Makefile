CC=g++

client: client.cpp
	$(CC) -o client client.cpp buffer.cpp requests.cpp helpers.cpp -Wall

run: client
	./client

zip:
	zip -r 323CA_Oprea_Marina_Tema4PC.zip *.cpp *.hpp ./nlohmann/*.hpp Makefile README.md

clean:
	rm -f *.o client *.zip