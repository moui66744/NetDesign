// Running LED
// Every time can shift to the left one, two, four, eight
// or shift to the right one, two
module light(
    input clk,
    input rst,
    input [15:0] sw,
    output [15:0] led,
    output [15:0] seg
);
    reg [15:0] led_reg;
    reg [15:0] count;
    assign seg = 16'b0;
    always @(posedge clk)begin
        if (!rst)
            led_reg <= 16'b1;
        else begin
            if (count == 0)begin
                if(sw[15] == 1'b1)
                    led_reg <= {led_reg[0],led_reg[15:1]};
                else if (sw[14] == 1'b1)
                    led_reg <= {led_reg[1:0],led_reg[15:2]};
                else if (sw[3] == 1'b1)
                    led_reg <= {led_reg[7:0],led_reg[15:8]};
                else if (sw[2] == 1'b1)
                   led_reg <= {led_reg[11:0],led_reg[15:12]};
                else if (sw[1] == 1'b1)
                   led_reg <= {led_reg[13:0],led_reg[15:14]};
                else if (sw[0] == 1'b1)
                   led_reg <= {led_reg[14:0],led_reg[15]};
            end
            count <= (count >= 10) ? 16'b0: count + 1;
        end
    end
    assign led = led_reg;
endmodule
