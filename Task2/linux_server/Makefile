.PHONY: all,clean
TARGET=linux_server
all:
	verilator --exe --build  --cc\
		linux_server.cpp fpvga.cpp\
		top.v  \
		-LDFLAGS -lpthread -CFLAGS -DTOP_NAME=Vtop -CFLAGS -g\
		-o $(TARGET) 
$(VMODEL):
	verilator --cc top.v
clean:
	rm -rf obj_dir/
run: all
	./obj_dir/$(TARGET)
debug:all
	gdb ./obj_dir/$(TARGET)

