verilator --exe --build  --cc top.v\
		fpvga.cpp\
		-CFLAGS -DTOP_NAME=Vtop -CFLAGS -g\
		-o $(VMODEL)