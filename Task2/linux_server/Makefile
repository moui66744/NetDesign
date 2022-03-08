.PHONY: all,clean
TARGET=linux_server
all:$(TARGET)
$(TARGET):
	g++ linux_server.cpp -o $(TARGET) -lpthread 
clean:
	rm -f $(TARGET)
run: $(TARGET)
	bash $(TARGET)
