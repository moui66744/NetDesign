.PHONY: all,clean
TARGET=linux_server
VMODEL=vmodel
all:vmodel
	g++ linux_server.cpp -o $(TARGET)\
		-lpthread -g
vmodel: #build wmodel
	touch ./obj_dir/$(VMODEL)
	rm ./obj_dir/$(VMODEL)
	verilator --exe --build  --cc top.v\
		fpvga.cpp\
		-CFLAGS -DTOP_NAME=Vtop -CFLAGS -g -CFLAGS -w\
		-o $(VMODEL)
clean:
	rm -rf obj_dir/ test linux_server fifo*
run: all
	./$(TARGET)
debug:all
	gdb ./obj_dir/$(TARGET)
test:
	verilator --exe --build  --cc\
		fpvga.cpp\
		top.v  \
		-LDFLAGS -lpthread -CFLAGS -DTOP_NAME=Vtop -CFLAGS -g\
		-CFLAGS -DUSE_MAIN\
		-o $(TARGET)
	./obj_dir/$(TARGET)
test_mq:vmodel # local test
	g++ -o test test.cpp -g
	./test
