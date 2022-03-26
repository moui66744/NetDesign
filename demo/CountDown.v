// countdown program
module light(
    input clk,
    input rst,
    input [15:0] sw,
    output [15:0] led,
    output [15:0] seg
);
    assign led = 0;
    reg [3:0]num;
    reg [4:0]count;
    always @(posedge clk)begin
        if(!rst)
            count <= 0;
        else
            count <= count + 1;
    end 
    always @(posedge clk)begin
        if (!rst)
            num <= 0;
        else if (count == 0)
            num <= num - 1;
    end
    wire [7:0]seg1,seg2;
    decode d1(num,seg1);
    decode d2(num,seg2);
    assign seg = {seg1,seg2};
endmodule

module decode(
    input [3:0]num,
    output [7:0]seg
);
    assign seg = (num == 4'd0) ? 8'b1111_1100:
                (num == 4'd1) ? 8'b0110_0000:
                (num == 4'd2) ? 8'b1101_1010:
                (num == 4'd3) ? 8'b1111_0010:
                (num == 4'd4) ? 8'b0110_0110:
                (num == 4'd5) ? 8'b1011_0110:
                (num == 4'd6) ? 8'b1011_1110:
                (num == 4'd7) ? 8'b1110_0000:
                (num == 4'd8) ? 8'b1111_1110:
                (num == 4'd9) ? 8'b1111_0110:
    (num == 4'ha) ? 8'b1110_1110:
    (num == 4'hb) ? 8'b0011_1110:
    (num == 4'hc) ? 8'b1001_1100:
    (num == 4'hd) ? 8'b0111_1010:
    (num == 4'he) ? 8'b1001_1110:
    (num == 4'hf) ? 8'b1000_1110:
                    8'b0000_0000;

endmodule
