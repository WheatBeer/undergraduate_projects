# fpu_32bits
Design 32bits fpu

### Design vision command

~~~
$ write -format verilog -hier -output ../outputs/fpu_elab.v

$ create_clock { "clk" } -name "clk" -period 10 -waveform { 0 5 }
-> 10ns, 100MHz 결과

$ compile -area_effort high

$ report_timing > ../reports/fpu_mapped.timing.rpt
$ report_area > ../reports/fpu_mapped.area.rpt

$ write -format verilog -hier -output ../outputs/fpu_mapped.v
~~~
 <br />

### Usage of tb_print.py

tb_print.py makes a verilog testbench code which consists of 2 inputs(a, b) and an output(y).
Put 2 fp intputs in the command line option as fllows, then tb code is printed. 

~~~
$ python3 tb_print.py -h

usage: tb_print.py [-h] [-a #] [-b #] [-o #]

Print out testbench examples

optional arguments:
  -h, --help     show this help message and exit
  -a #, --in1 #  input 1 | Default: 0
  -b #, --in2 #  input 2 | Default: 0
  -o #, --op #   operation | + : 0 | - : 1 | * : 2 | / : 3
~~~
  
  - an example(a(1) + b(2) = y(3))
~~~
$ python3 tb_print.py -a 1 -b 2 -o 0

// y =  3.00
a <= 32'b0_01111111_00000000000000000000000;  // a =  1.0
b <= 32'b0_10000000_00000000000000000000000;  // b =  2.0
~~~
