`timescale 1ns/1ns

module test;
    reg rst; 
    reg power; 
    reg cooling; 
    reg dehumidification; 
    reg ventilation; 
    reg cl_switch;
    reg cl_button; 
    reg clk;
    wire cooler; 
    wire outdoor_fan; 
    wire dehumidifier; 
    wire ventilator;
    wire [2:0] cooling_level;
    wire [2:0] state;

    air air_test(rst, power, cooling, dehumidification, ventilation, cl_switch, cl_button, clk, 
        cooler, outdoor_fan, dehumidifier, ventilator, cooling_level, state);

    always 
    begin 
        #1 clk = ~clk;
    end

    initial
    begin
        clk <= 1;
        rst <= 0;
        power <= 0;
        cooling <= 0;
        dehumidification <= 0;
        ventilation <= 0;
        cl_switch <= 0;
        cl_button <= 0;  
        #2

        // State 0: Idle State
        rst <= 1;
        power <= 0;
        #2

        // State 1: Cooling 1        
        rst <= 0;
        power <= 1;
        cooling <= 1;
        #4

        // State 2: Cooling 2
        // Going Up
        cl_switch <= 1;
        cl_button <= 0;
        #1 // Nothing
        cl_switch <= 0;
        cl_button <= 1;
        #3

        // State 3: Cooling 3
        cl_switch <= 1;
        cl_button <= 0;
        #1
        cl_switch <= 0;
        cl_button <= 1;
        #3

        // State 4: Cooling 4
        cl_switch <= 1;
        cl_button <= 0;
        #1
        cl_switch <= 0;
        cl_button <= 1;
        #3

        // State 3: Cooling 3
        cl_switch <= 0;
        cl_button <= 0;
        #1
        cl_switch <= 0;
        cl_button <= 1;
        #3

        // State 2: Cooling 2
        cl_switch <= 0;
        cl_button <= 0;
        #1
        cl_switch <= 0;
        cl_button <= 1;
        #3

        // State 5: Dehumidification
        cooling <= 0;
        dehumidification <= 1;
        #4

        // State 6: Ventilation
        dehumidification <= 0;
        ventilation <= 1;
        
        // State 0: Idle
        #5
        ventilation <= 0;
    end
endmodule