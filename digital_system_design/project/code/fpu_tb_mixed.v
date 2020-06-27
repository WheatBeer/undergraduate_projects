`timescale 1ns/100ps

module test;
    reg clk;
    reg rst;
    reg [31:0] a;
    reg [31:0] b;
    reg [1:0] sel;
    wire err;
    wire overflow;
    wire [31:0] y;

    fpu fpu_test(clk, rst, a, b, sel, err, overflow, y);

    always
    begin
        #0.5 clk = ~clk;
    end

    initial
    begin
        clk <= 1;
        rst <= 0;
        a <= 0;
        b <= 0;
        sel <= 0;
        
        #0.9
        a <= 32'b0_10000000_10000000000000000000000;  // a =  3.0
        b <= 32'b0_10000000_00000000000000000000000;  // b =  2.0
        sel <= 2'b00; // y =  5.0

        #1
        sel <= 2'b01; // y =  1.0

        #1
        sel <= 2'b10; // y =  6.0

        #1
        sel <= 2'b11; // y =  1.5

        #1
        sel <= 2'b00; // y =  5.0

        #1
        sel <= 2'b01; // y =  1.0

        #1
        sel <= 2'b10; // y =  6.0

        #1
        sel <= 2'b11; // y =  1.5

        #1
        sel <= 2'b00; // y =  5.0

        #1
        sel <= 2'b01; // y =  1.0
    end
endmodule
