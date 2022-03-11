.PHONY: all server vmodel debug
SERVER=build/server
VMODEL=build/vmodel

all :server vmodel
	bash -c $(SERVER)
debug:server vmodel
	gdb $(SERVER)
server:
	g++ -o $(SERVER) server.cpp -g
vmodel:
	g++ -o $(VMODEL) vmodel.cpp -g
clean:
	rm build/* fifo*
