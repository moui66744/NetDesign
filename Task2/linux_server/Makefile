.PHONY: all,clean
TARGET=linux_server
all:$(linux_server)
$(linux_server):
	g++ linux_server.cpp -o $(TARGET) -lpthread 
clean:
	rm -f $(TARGET)
run: $(linux_server)
	bash $(TARGET)
