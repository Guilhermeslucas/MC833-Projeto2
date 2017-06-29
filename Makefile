all: server client
	python3 test_script.py

server: server.c collisionChecker.c
	gcc server.c collisionChecker.c -o server

client: client.c
	gcc client.c -o client
