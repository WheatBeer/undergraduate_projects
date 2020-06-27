import struct
import argparse

parser = argparse.ArgumentParser(description='Print out testbench examples')

parser.add_argument('-a', '--in1', type=float, metavar='#', default=0, help='input 1 | Default: 0')
parser.add_argument('-b', '--in2', type=float, metavar='#', default=0, help='input 2 | Default: 0')
parser.add_argument('-o', '--op', type=int, metavar='#', default=0, help='operation | + : 0 | - : 1 | * : 2 | / : 3')

def fp32_to_bin(num):
  string_bin = format(struct.unpack('!I', struct.pack('!f', num))[0], '032b')
  return string_bin

def under_bar(bin_str):
  for i in range(32):
      if (i == 0):
        print(bin_str[i], end='')
        print('_', end='')
      elif (i > 0 and i < 8):
        print(bin_str[i], end='')
      elif (i == 8):
        print(bin_str[i], end='')
        print('_', end='')
      else:
        print(bin_str[i], end='')

def make_tb(a, b, op):
  str_a = fp32_to_bin(a)
  str_b = fp32_to_bin(b)
  if op == 0: 
  	print("// y = ", '%.2f' % (a + b))
  elif op == 1:
  	print("// y = ", '%.2f' % (a - b))
  elif op == 2:	
  	print("// y = ", '%.2f' % (a * b))
  elif op == 3:
  	print("// y = ", '%.2f' % (a / b))
  print("a <= 32'b", end='')
  under_bar(str_a)
  print(";  // a = ", a)
  print("b <= 32'b", end='')
  under_bar(str_b)
  print(";  // b = ", b)

def main():
	args = parser.parse_args()
	if (args.op >= 0 and args.op < 4):
		make_tb(args.in1, args.in2, args.op)
	else:
		print('retry with exact operator')
		return

if __name__ == '__main__':
    main()

