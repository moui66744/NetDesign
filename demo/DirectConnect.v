// The switch is directly connected to the LED
module light(
	input clk,
	input rst,
	input [15:0] sw,
	output [15:0] led,
	output [15:0] seg
);
	reg [31:0] count;
	assign seg = 16'b0;
	assign led = sw;
endmodule
