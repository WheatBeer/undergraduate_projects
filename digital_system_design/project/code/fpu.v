
`timescale 1ns/100ps

module fpu(
    input clk, input rst, input [31:0] a, input [31:0] b,input [1:0] sel,
    output reg err, output reg overflow, output reg [31:0] y);

/* Temperary variables */
// stage 0
reg         [1:0]       sel_0;
reg                     a_s, b_s;
reg         [7:0]       a_e, b_e;
reg         [23:0]      a_m; 
reg         [23:0]      b_m;

// stage 1
reg         [1:0]       sel_1;
reg                     tmp_s0;
reg signed  [9:0]       tmp_e0;
reg         [47:0]      tmp_m0;
reg                     tmp_a_s0;
reg         [7:0]       tmp_a_e0;
reg         [23:0]      tmp_a_m0;
reg                     tmp_b_s0;
reg         [7:0]       tmp_b_e0;
reg         [23:0]      tmp_b_m0;

// stage 2
reg         [1:0]       sel_2;
reg                     tmp_s1;
reg signed  [9:0]       tmp_e1;
reg         [47:0]      tmp_m1;
reg                     tmp_a_s1;
reg         [7:0]       tmp_a_e1;
reg         [23:0]      tmp_a_m1;
reg                     tmp_b_s1;
reg         [7:0]       tmp_b_e1;
reg         [23:0]      tmp_b_m1;

/* Main */
always @ (posedge clk or posedge rst)
begin
    if (rst)
    begin
        {a_s, a_e, a_m} <= 33'd0;
        {b_s, b_e, b_m} <= 33'd0;
        {tmp_s0, tmp_s1, tmp_e0, tmp_e1, tmp_m0, tmp_m1} <= 118'd0;
        {sel_0, sel_1} <= 4'd0;
        err <= 0; overflow <= 0; y <= 0;
    end
    else
    begin
/******************** Operator ********************/ 
/******************** Stage 0 ********************/ 

        {a_s, a_e, a_m[22:0]} <= a;
        {b_s, b_e, b_m[22:0]} <= b;
        // mantissa(24bits) =  1.frac(23bits)
        a_m[23] <= 1;
        b_m[23] <= 1;
        sel_0 <= sel;

/******************** Stage 1 ********************/ 

/* sel 0 or 1: plus or minus */

        if (sel_0 == 0 || sel_0 == 1)
        begin
            {tmp_a_s0, tmp_a_e0, tmp_a_m0} <= {a_s, a_e, a_m};
            {tmp_b_s0, tmp_b_e0, tmp_b_m0} <= {b_s, b_e, b_m};      

            if (sel_0 == 0) // plus case
            begin
                if (a_s == b_s) // same sign so +
                begin
                    if (a_e < b_e)
                    begin 
                        tmp_m0 <= b_m + (a_m >> (b_e - a_e)); 
                        tmp_e0 <= b_e;
                        tmp_s0 <= a_s;
                        sel_1  <= sel_0;
                    end
                        
                    else if (a_e > b_e)
                    begin
                        tmp_m0 <= a_m + (b_m >> (a_e - b_e));
                        tmp_e0 <= a_e;
                        tmp_s0 <= a_s;
                        sel_1  <= sel_0;
                    end

                    else // a_e == b_e
                    begin
                        if (a_m < b_m)
                        begin
                            tmp_m0 <= a_m + b_m; 
                            tmp_e0 <= b_e;
                            tmp_s0 <= a_s;  
                            sel_1  <= sel_0;                                    
                        end
                        else
                        begin
                            tmp_m0 <= a_m + b_m; 
                            tmp_e0 <= a_e;
                            tmp_s0 <= a_s;   
                            sel_1  <= sel_0;                              
                        end
                    end
                end

                else // different sign so -
                begin
                    if (a_e < b_e)
                    begin
                        tmp_m0 <= b_m - (a_m >> (b_e - a_e)); 
                        tmp_e0 <= b_e;
                        tmp_s0 <= b_s;
                        sel_1  <= sel_0;
                    end
                        
                    else if (a_e > b_e)
                    begin
                        tmp_m0 <= a_m - (b_m >> (a_e - b_e)); 
                        tmp_e0 <= a_e;
                        tmp_s0 <= a_s;
                        sel_1  <= sel_0;
                    end

                    else // a_e == b_e
                    begin
                        if (a_m < b_m)
                        begin
                            tmp_m0 <= b_m - a_m; 
                            tmp_e0 <= b_e;
                            tmp_s0 <= b_s;    
                            sel_1  <= sel_0;                             
                        end
                        else // (a_m > b_m)
                        begin
                            tmp_m0 <= a_m - b_m; 
                            tmp_e0 <= a_e;
                            tmp_s0 <= a_s;  
                            sel_1  <= sel_0;                                     
                        end
                    end
                end 
            end

            else // sel ==1, minus case
            begin
                if (a_s == b_s) // different sign so -
                begin
                    if (a_e < b_e)
                    begin
                        tmp_m0 <= b_m - (a_m >> (b_e - a_e)); 
                        tmp_e0 <= b_e;
                        tmp_s0 <= ~ b_s;
                        sel_1  <= sel_0;
                    end
                        
                    else if (a_e > b_e)
                    begin
                        tmp_m0 <= a_m - (b_m >> (a_e - b_e)); 
                        tmp_e0 <= a_e;
                        tmp_s0 <= a_s;
                        sel_1  <= sel_0;
                    end
 
                    else // a_e == b_e
                    begin
                        if (a_m < b_m)
                        begin
                            tmp_m0 <= b_m - a_m; 
                            tmp_e0 <= b_e;
                            tmp_s0 <= ~ b_s;   
                            sel_1  <= sel_0;                  
                        end
                        else // (a_m > b_m)
                        begin
                            tmp_m0 <= a_m - b_m; 
                            tmp_e0 <= a_e;
                            tmp_s0 <= a_s;
                            sel_1  <= sel_0;
                        end
                    end
                end

                else // same sign so +
                begin
                    if (a_e < b_e)
                    begin
                        tmp_m0 <= b_m + (a_m >> (b_e - a_e)); 
                        tmp_e0 <= b_e;
                        tmp_s0 <= a_s;
                        sel_1  <= sel_0;
                    end
                        
                    else if (a_e > b_e)
                    begin
                        tmp_m0 <= a_m + (b_m >> (a_e - b_e)); 
                        tmp_e0 <= a_e;
                        tmp_s0 <= a_s;
                        sel_1  <= sel_0;
                    end

                    else // a_e == b_e
                    begin
                        if (a_m < b_m)
                        begin
                            tmp_m0 <= a_m + b_m; 
                            tmp_e0 <= b_e;
                            tmp_s0 <= a_s;  
                            sel_1  <= sel_0;                                      
                        end
                        else
                        begin
                            tmp_m0 <= a_m + b_m; 
                            tmp_e0 <= a_e;
                            tmp_s0 <= a_s;    
                            sel_1  <= sel_0;                                  
                        end
                    end
                end 
            end
        end

        else if (sel_0 == 2) // multiple case
        begin
            {tmp_a_s0, tmp_a_e0, tmp_a_m0} <= {a_s, a_e, a_m};
            {tmp_b_s0, tmp_b_e0, tmp_b_m0} <= {b_s, b_e, b_m};
            tmp_s0 <= a_s ^ b_s;
            tmp_e0 <= a_e + b_e;
            tmp_m0 <= a_m * b_m;
            sel_1  <= sel_0; 
        end

        else // divide case
        begin
            {tmp_a_s0, tmp_a_e0, tmp_a_m0} <= {a_s, a_e, a_m};
            {tmp_b_s0, tmp_b_e0, tmp_b_m0} <= {b_s, b_e, b_m};
            tmp_s0 <= a_s ^ b_s;
            tmp_e0 <= a_e - b_e;
            tmp_m0 <= {a_m, 23'd0} / b_m;
            sel_1  <= sel_0; 
        end

/******************** Stage 2 ********************/            
 
        if (sel_1 == 0 || sel_1 == 1) 
        begin
            {tmp_a_s1, tmp_a_e1, tmp_a_m1} <= {tmp_a_s0, tmp_a_e0, tmp_a_m0};
            {tmp_b_s1, tmp_b_e1, tmp_b_m1} <= {tmp_b_s0, tmp_b_e0, tmp_b_m0}; 
            tmp_s1 <= tmp_s0;
            sel_2  <= sel_1;
            if (tmp_m0[24:9] == 0) // 8;1 check needed // for tmp_m0[0] == 0, operating zero 
            begin
                if (tmp_m0[8:1] == 0) // last bit only exist
                begin
                tmp_e1 <= tmp_e0 - 23;
                tmp_m1 <= 23'h800000;
                end
                else
                begin
                    if (tmp_m0[8:5] == 0) // 4;1 check needed
                    begin
                        if (tmp_m0[4:3] == 0) // 2;1 check needed
                        begin
                            if (tmp_m0[2] == 0) // 1 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 22; // 23-1=22
                                tmp_m1 <= tmp_m0 << 22;
                            end
                            else // 2 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 21;
                                tmp_m1 <= tmp_m0 << 21;
                            end
                        end
                        else // 4;3 check needed
                        begin
                            if (tmp_m0[4] == 0) // 3 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 20;
                                tmp_m1 <= tmp_m0 << 20;
                            end
                            else // 4 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 19;
                                tmp_m1 <= tmp_m0 << 19;
                            end
                        end
                    end
                    else // 8;5 check needed
                    begin
                        if (tmp_m0[8:7] == 0) // 6;5 check needed
                        begin
                            if (tmp_m0[6] == 0) // 5 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 18;
                                tmp_m1 <= tmp_m0 << 18;
                            end
                            else // 6 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 17;
                                tmp_m1 <= tmp_m0 << 17;
                            end
                        end
                        else // 8;7 check needed
                        begin
                            if (tmp_m0[8] == 0) // 7 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 16;
                                tmp_m1 <= tmp_m0 << 16;
                            end
                            else // 8 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 15;
                                tmp_m1 <= tmp_m0 << 15;
                            end
                        end
                    end
                end
            end
            else 
            begin
                if (tmp_m0[24:17] == 0) // 16;9 check needed
                begin
                    if (tmp_m0[16:13] == 0) // 12;9 check needed
                    begin
                        if (tmp_m0[12:11] == 0) // 10;9 check needed
                        begin
                            if (tmp_m0[10] == 0) // then 9 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 14; // 23-9=14
                                tmp_m1 <= tmp_m0 << 14;
                            end
                            else // then 10 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 13;
                                tmp_m1 <= tmp_m0 << 13;
                            end
                        end 
                        else // 12;11 check needed
                        begin
                            if (tmp_m0[12] == 0) // then 11 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 12;
                                tmp_m1 <= tmp_m0 << 12;
                            end
                            else // then 12 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 11;
                                tmp_m1 <= tmp_m0 << 11;
                            end
                        end
                    end
                    else // 16;13 check needed
                    begin
                        if (tmp_m0[16:15] == 0) // 14;13 check needed
                        begin
                            if (tmp_m0[14] == 0) // then 13 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 10;
                                tmp_m1 <= tmp_m0 << 10;
                            end
                            else // then 14 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 9;
                                tmp_m1 <= tmp_m0 << 9;
                            end
                        end 
                        else // 16;15 check needed
                        begin
                            if (tmp_m0[16] == 0) // then 15 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 8;
                                tmp_m1 <= tmp_m0 << 8;
                            end
                            else // then 16 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 7;
                                tmp_m1 <= tmp_m0 << 7;
                            end
                        end
                    end
                end
                else // 24;17 check needed
                begin
                    if (tmp_m0[24:21] == 0) // 20;17 check needed
                    begin
                        if (tmp_m0[20:19] == 0) // 18;17 check needed
                        begin
                            if (tmp_m0[18] == 0) // then 17 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 6;
                                tmp_m1 <= tmp_m0 << 6;
                            end
                            else // then 18 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 5;
                                tmp_m1 <= tmp_m0 << 5;
                            end
                        end
                        else // 20;19 check needed
                        begin
                            if (tmp_m0[20] == 0) // then 19 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 4;
                                tmp_m1 <= tmp_m0 << 4;
                            end
                            else // then 20 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 3;
                                tmp_m1 <= tmp_m0 << 3;
                            end
                        end
                    end 
                    else // 24;21 check needed
                    begin
                        if (tmp_m0[24:23] == 0) // 22;21 check needed
                        begin
                            if (tmp_m0[22] == 0) // then 21 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 2;
                                tmp_m1 <= tmp_m0 << 2;
                            end
                            else // then 22 is answer
                            begin
                                tmp_e1 <= tmp_e0 - 1;
                                tmp_m1 <= tmp_m0 << 1;
                            end
                        end
                        else // 24;23 check needed
                        begin
                            if (tmp_m0[24] == 0) // then 23 is answer
                            begin
                                tmp_e1 <= tmp_e0;
                                tmp_m1 <= tmp_m0;
                            end
                            else // then 24 is answer
                            begin
                                tmp_e1 <= tmp_e0 + 1;
                                tmp_m1 <= tmp_m0 >> 1;
                            end
                        end
                    end 
                end
            end
        end

        else if (sel_1 == 2)
        begin 
            if (tmp_m0[47] == 1) 
            begin
                {tmp_a_s1, tmp_a_e1, tmp_a_m1} <= {tmp_a_s0, tmp_a_e0, tmp_a_m0};
                {tmp_b_s1, tmp_b_e1, tmp_b_m1} <= {tmp_b_s0, tmp_b_e0, tmp_b_m0};
                tmp_s1 <= tmp_s0;
                tmp_e1 <= tmp_e0 - 126;
                tmp_m1 <= tmp_m0 >> 1;
                sel_2  <= sel_1; 
            end
            else
            begin
                {tmp_a_s1, tmp_a_e1, tmp_a_m1} <= {tmp_a_s0, tmp_a_e0, tmp_a_m0};
                {tmp_b_s1, tmp_b_e1, tmp_b_m1} <= {tmp_b_s0, tmp_b_e0, tmp_b_m0};
                tmp_s1 <= tmp_s0;
                tmp_e1 <= tmp_e0 - 127;
                tmp_m1 <= tmp_m0;
                sel_2  <= sel_1; 
            end
        end

        else // divide case
        begin
            if (tmp_m0[23] == 1)
            begin
                {tmp_a_s1, tmp_a_e1, tmp_a_m1} <= {tmp_a_s0, tmp_a_e0, tmp_a_m0};
                {tmp_b_s1, tmp_b_e1, tmp_b_m1} <= {tmp_b_s0, tmp_b_e0, tmp_b_m0};
                tmp_s1 <= tmp_s0;
                tmp_e1 <= tmp_e0 + 127;
                tmp_m1 <= tmp_m0;
                sel_2  <= sel_1;
            end
            else
            begin
                {tmp_a_s1, tmp_a_e1, tmp_a_m1} <= {tmp_a_s0, tmp_a_e0, tmp_a_m0};
                {tmp_b_s1, tmp_b_e1, tmp_b_m1} <= {tmp_b_s0, tmp_b_e0, tmp_b_m0};
                tmp_s1 <= tmp_s0;
                tmp_e1 <= tmp_e0 + 126;
                tmp_m1 <= tmp_m0 << 1;
                sel_2  <= sel_1;
            end
        end

/******************** Stage 3 ********************/ 

        if (sel_2 == 0 || sel_2 == 1)
        begin
            /* if a is NaN or b is NaN 
            or a and b are both inf, y is NaN */
            if ((tmp_a_e1 == 255 && tmp_a_m1[22:0] != 0) || (tmp_b_e1 == 255 && tmp_b_m1[22:0] != 0) || (tmp_a_e1 == 255 && tmp_b_e1 == 255))
            begin
                // y is NaN
                y <=  {1'b0, 8'd255, 23'h400000};
                err <=  1;
                overflow <= 0;
            end
            /* if a is inf and b is normal #, y is a */ 
            else if (tmp_a_e1 == 255)
            begin
                // y = inf (sign follows a's)
                y <= {tmp_a_s1, 8'd255, 23'd0};
                err <= 0;
                overflow <= 1;
            end
            /* if b is inf and a is normal #, y is b */ 
            else if (tmp_b_e1 == 255)
            begin
                // y = inf (sign follows b's)
                y <= {tmp_b_s1, 8'd255, 23'd0};
                err <= 0;
                overflow <= 1;
            end
            /* if a and b are same, y is zero */ 
            else if (sel_2[0] ^ tmp_a_s1 ^ tmp_b_s1 == 1 && tmp_a_e1 == tmp_b_e1 && tmp_a_m1[22:0] == tmp_b_m1[22:0])
            begin
            // y = zero (sign follows a's)
                y <= {tmp_a_s1, 31'd0};  
                err <= 0;
                overflow <= 1;
            end
            /* if a is zero, y is b */ 
            else if (tmp_a_e1 == 0 && tmp_a_m1[22:0] == 0)
            begin
                if (tmp_b_e1 == 0 && tmp_b_m1[22:0] == 0) // b is also zero
                begin
                    // y = zero (sign follows a's)
                    y <= {tmp_a_s1, 31'd0};  
                    err <= 0;
                    overflow <= 1;
                end
                else
                begin
                    y <= {tmp_b_s1, tmp_b_e1[7:0], tmp_b_m1[22:0]};
                    err <= 0;
                    overflow <= 0;
                end
            end
            /* if b is zero, y is a */ 
            else if (tmp_b_e1 == 0 && tmp_b_m1[22:0] == 0)
            begin
                y <= {tmp_a_s1, tmp_a_e1[7:0], tmp_a_m1[22:0]};
                err <= 0;
                overflow <= 0;
            end
            /* General cases */
            else
            begin
                if (tmp_e1 >= 255) // overflow
                begin
                    // y = inf
                    y <= {tmp_s1, 8'd255, 23'd0};
                    err <= 0;
                    overflow <= 1;
                end
                else if (tmp_e1 <= 0) // underflow
                begin
                    // y = zero
                    y <= {tmp_s1, 31'd0};
                    err <= 0;
                    overflow <= 1;
                end
                else
                begin // jsy
                   if (tmp_m1[0]) // last bit is 1, round half up
                   begin
                       if ((tmp_m1 + 1'b1) > 24'hffffff) // exponent is changing and overflow can be occured
                       begin
                           if (tmp_e1 >= 254) // overflow
                           begin
                               // y = inf
                               y <= {tmp_s1, 8'd255, 23'd0};
                               err <= 0;
                               overflow <= 1;
                           end
                           else // shift needed
                           begin
                               y <= {tmp_s1, (tmp_e1[7:0]+1'b1), (tmp_m1[23:2]+1'b1), 1'd0}; // we take the bit from 23 to 2
                               err <= 0;
                               overflow <= 0;
                           end
                       end
                       else // no change for exponent
                       begin 
                            y <= {tmp_s1, tmp_e1[7:0], (tmp_m1[22:1]+1'b1), 1'd0};
                            err <= 0;
                            overflow <= 0;
                       end
                   end
                   else // last bit is 0
                   begin
                        y <= {tmp_s1, tmp_e1[7:0], tmp_m1[22:1], 1'd0};
                        err <= 0;
                        overflow <= 0;
                   end
                end
            end
        end

        else if (sel_2 == 2)
        begin
            /* if a is NaN or b is NaN, y is NaN. */
            if ((tmp_a_e1 == 255 && tmp_a_m1[22:0] != 0) || (tmp_b_e1 == 255 && tmp_b_m1[22:0] != 0))
            begin
                // y = NaN
                y <= {1'b0, 8'd255, 23'h400000};
                err <= 1;
                overflow <= 0;
            end
            /* if a is inf and b is zero, y is NaN.
            Or, if a is inf and b is not zero, y is inf. */
            else if (tmp_a_e1 == 255)
            begin
                if (tmp_b_e1 == 0 && tmp_b_m1[22:0] == 0)
                begin
                    // y = NaN
                    y <=  {1'b0, 8'd255, 23'h400000};
                    err <=  1;
                    overflow <= 0;
                end
                else
                begin 
                    // y = inf
                    y <= {tmp_s1, 8'd255, 23'd0};
                    err <= 0;
                    overflow <= 1;
                end
            end
            /* if b is inf and a is zero, y is NaN.
            Or, if b is inf and a is not zero, y is inf. */
            else if (tmp_b_e1 == 255)
            begin
                if (tmp_a_e1 == 0 && tmp_a_m1[22:0] == 0)
                begin
                    // y = NaN
                    y <=  {1'b0, 8'd255, 23'h400000};
                    err <=  1;
                    overflow <= 0;
                end
                else
                begin
                    // y = inf
                    y <= {tmp_s1, 8'd255, 23'd0};
                    err <= 0;
                    overflow <= 1;
                end
            end
            /* if a is zero, y is zero.
            Or, if b is zero, y is zero. */
            else if ((tmp_a_e1 == 0 && tmp_a_m1[22:0] == 0) || (tmp_b_e1 == 0 && tmp_b_m1[22:0] == 0))
            begin
                // y = zero
                y <= {tmp_s1, 31'd0};
                err <= 0;
                overflow <= 1;
            end
            /* General cases a(Norm) x b(Norm) */
            else
            begin
                if (tmp_e1 >= 255) // overflow
                begin
                    // y = inf
                    y <= {tmp_s1, 8'd255, 23'd0};
                    err <= 0;
                    overflow <= 1;
                end
                else if (tmp_e1 <= 0) // underflow
                begin
                    // y = zero
                    y <= {tmp_s1, 31'd0};
                    err <= 0;
                    overflow <= 1;
                end
                else
                begin
                   if (tmp_m1[23]) // last bit is 1, round half up
                   begin
                       if ((tmp_m1+1'b1) > 47'h7fffffffffff) // exponent is changing and overflow can be occured
                       begin
                           if (tmp_e1 >= 254) // overflow
                           begin
                               // y = inf
                               y <= {tmp_s1, 8'd255, 23'd0};
                               err <= 0;
                               overflow <= 1;
                           end
                           else // shift needed
                           begin
                               y <= {tmp_s1, (tmp_e1[7:0]+1'b1), (tmp_m1[46:25]+1'b1), 1'd0}; // 
                               err <= 0;
                               overflow <= 0;
                           end
                       end
                       else // no change for exponent
                       begin 
                            y <= {tmp_s1, tmp_e1[7:0], (tmp_m1[45:24]+1'b1), 1'd0};
                            err <= 0;
                            overflow <= 0;
                       end
                   end
                   else // last bit is 0
                   begin
                        y <= {tmp_s1, tmp_e1[7:0], tmp_m1[45:24], 1'd0};
                        err <= 0;
                        overflow <= 0;
                   end
                end
            end
        end

        else
        begin
            /* if a is NaN or b is NaN, y is NaN. */
            if ((tmp_a_e1 == 255 && tmp_a_m1[22:0] != 0) || (tmp_b_e1 == 255 && tmp_b_m1[22:0] != 0))
            begin
                y <= {1'b0, 8'd255, 23'h400000};
                err <= 1;
                overflow <= 0;
            end
            /* if b is zero(dividing by zero), y is NaN. */
            else if (tmp_b_e1 == 0 && tmp_b_m1[22:0] == 0)
            begin
                // y is NaN
                y <= {1'b0, 8'd255, 23'h400000};
                err <= 1;
                overflow <= 0;
            end
            /* if a is zero, y is zero. */
            else if (tmp_a_e1 == 0 && tmp_a_m1[22:0] == 0)
            begin
                // y = zero
                y <= {tmp_s1, 31'd0};
                err <= 0;
                overflow <= 1;
            end
            /* if b is inf and a is inf, y is NaN. */
            else if (tmp_b_e1 == 255 || tmp_a_e1 == 255)
            begin
                // y is NaN
                y <= {1'b0, 8'd255, 23'h400000};
                err <= 1;
                overflow <= 0;
            end
            /* if b is inf, y is zero. */
            else if (tmp_b_e1 == 255)
            begin
                // y = zero
                y <= {tmp_s1, 31'd0};
                err <= 0;
                overflow <= 1;
            end
            /* if a is inf, y is inf. */
            else if (tmp_a_e1 == 255)
            begin
                // y = inf
                y <= {tmp_s1, 8'd255, 23'd0};
                err <= 0;
                overflow <= 1;
            end
           /* General cases */
            else
            begin
                if (tmp_e1 >= 255) // overflow
                begin
                    // y = inf
                    y <= {tmp_s1, 8'd255, 23'd0};
                    err <= 0;
                    overflow <= 1;
                end
                else if (tmp_e1 <= 0) // underflow
                begin
                    // y = zero
                    y <= {tmp_s1, 31'd0};
                    err <= 0;
                    overflow <= 1;
                end
                else
                begin // jsy
                   if (tmp_m1[0]) // last bit is 1, round half up
                   begin
                       if ((tmp_m1 + 1'b1) > 24'hffffff) // exponent is changing and overflow can be occured
                       begin
                           if (tmp_e1 >= 254) // overflow
                           begin
                               // y = inf
                               y <= {tmp_s1, 8'd255, 23'd0};
                               err <= 0;
                               overflow <= 1;
                           end
                           else // shift needed
                           begin
                               y <= {tmp_s1, (tmp_e1[7:0]+1'b1), (tmp_m1[23:2]+1'b1), 1'd0}; // we take the bit from 23 to 2
                               err <= 0;
                               overflow <= 0;
                           end
                       end
                       else // no change for exponent
                       begin 
                            y <= {tmp_s1, tmp_e1[7:0], (tmp_m1[22:1]+1'b1), 1'd0};
                            err <= 0;
                            overflow <= 0;
                       end
                   end
                   else // last bit is 0
                   begin
                        y <= {tmp_s1, tmp_e1[7:0], tmp_m1[22:1], 1'd0};
                        err <= 0;
                        overflow <= 0;
                   end
                end
            end
        end
    end
end
endmodule
