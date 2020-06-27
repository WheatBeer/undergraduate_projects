`timescale 1ns/1ns
/*
• Design air conditioner in behavior level based on below in/out ports. 
    input rst, power, cooling, dehumidification, ventilation, cl_switch, cl_button, clk;
    output reg cooler, outdoor_fan, dehumidifier, ventilator;
    output reg [2:0] cooling_level;

• Use 7 states : s_idle, s_cooling(1~4), s_dehumidification, s_ventilation

• In case of cooling level up/down signals, use cl_switch and cl_button to control(up/down
switch and select button).

• Ventilation operation ends automatically after T1 seconds and returns to Idle state.

• Time : T1 = 5 clocks

• The air conditioner returns to Idle state whenever power is turned off.
*/

module air(
    input rst, 
    input power, 
    input cooling, 
    input dehumidification, 
    input ventilation, 
    input cl_switch, 
    input cl_button, 
    input clk, 
    output reg cooler, 
    output reg outdoor_fan, 
    output reg dehumidifier, 
    output reg ventilator, 
    output reg [2:0] cooling_level,
    output reg [2:0] state); // Current State

    /* 7 States */
    parameter s_idle = 3'd000;                  // State 0
    parameter s_cooling1 = 3'd001;              // State 1
    parameter s_cooling2 = 3'd010;              // State 2
    parameter s_cooling3 = 3'd011;              // State 3
    parameter s_cooling4 = 3'd100;              // State 4
    parameter s_dehumidification = 3'd101;      // State 5
    parameter s_ventilation = 3'd110;           // State 6

    /* Time */
    parameter T1 = 5;

    /* Variables */
    wire cooling_up;             // cl_switch & cl_button'                  
    wire cooling_down;           // cl_switch’ & cl_button’
    
    /* Assign the cooling_up and cooling_down buttons */
    assign cooling_up = cl_switch && ~cl_button; 
    assign cooling_down = ~cl_switch && ~cl_button;

    /* Main: Set each state and outputs */
    always @ (posedge clk or posedge rst or posedge cooling or posedge dehumidification or posedge ventilation)
    begin
        // State 0: Idle
        if (rst || ~power)
        begin
            state <= s_idle;
            cooler <= 0;
            outdoor_fan <= 0;
            dehumidifier <= 0;
            ventilator <= 0;
            cooling_level <= 0;
        end
        // State 1 ~ 4: Cooling(1 ~ 4)
        else if (power && cooling)
        begin
            if ((state != s_cooling2) && (state != s_cooling3) && (state != s_cooling4))
            begin
                state <= s_cooling1;
                cooler <= 1;
                outdoor_fan <= 0;
                dehumidifier <= 0;
                ventilator <= 0;
                cooling_level <= 1;
            end
        end
        // State 5: Dehumidification
        else if (power && dehumidification)
        begin
            state <= s_dehumidification;
            cooler <= 0;
            outdoor_fan <= 0;
            dehumidifier <= 1;
            ventilator <= 0;
            cooling_level <= 0;
        end
        // State 6: Ventilation
        else if (power && ventilation)
        begin
            state <= s_ventilation;
            state <= #T1 s_idle;
            cooler <= 0;
            outdoor_fan <= 0;
            dehumidifier <= 0;
            ventilator <= 1;
            ventilator <= #T1 0;
            cooling_level <= 0;
        end
    end

    /* Cooling level controller */
    always @ (posedge cooling_up or posedge cooling_down)
    begin
        /* Going up */
        if ((state == s_cooling1) && cooling_up)
        begin
            cooling_level <= 2;
            state <= s_cooling2;
        end
        else if ((state == s_cooling2) && cooling_up)
        begin
            cooling_level <= 3;
            state <= s_cooling3;
            outdoor_fan <= 1;
        end
        else if ((state == s_cooling3) && cooling_up)
        begin
            cooling_level <= 4;
            state <= s_cooling4;
        end

        /* Going down */
        if ((state == s_cooling4) && cooling_down)
        begin
            cooling_level <= 3;
            state <= s_cooling3;
        end
        else if ((state == s_cooling3) && cooling_down)
        begin
            cooling_level <= 2;
            state <= s_cooling2;
            outdoor_fan <= 0;
        end
        else if ((state == s_cooling2) && cooling_down)
        begin
            cooling_level <= 1;
            state <= s_cooling1;
        end
    end
endmodule