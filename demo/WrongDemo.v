// This is a wrong demo
// FPVGA should return error
module light(
	input clk,
	input rst,
	input [15:0] sw,
	output [15:0] led,
	output [15:0] seg
);
    assign led = sw;
	assign seg = 16'b0;
