module light(
	input clk,
	input rst,
	input [15:0] sw,
	output reg [15:0] led,
	output seg
);
	reg [31:0] count;
	always @(posedge clk) begin
		if (!rst) begin
			led <= 1;
		end
		else begin
			if (count == 0) led <= {led[14:0], led[15]};
			count <= (count >= 50 ? 32'b0 : count + 1);
		end
	end
endmodule
