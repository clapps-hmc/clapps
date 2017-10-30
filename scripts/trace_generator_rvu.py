#usage: this script reads a text file with a series of RVU's instructions
#and convert them to clapps trace file
#python trace_generator_rvu.py input.txt output.txt
# LOAD32 rs, address
# LOAD64 rs, address
# LOAD128 rs, address
# LOAD256 rs, address
# STORE32 rs, address
# STORE64 rs, address
# STORE128 rs, address
# STORE256 rs, address
# ADD32 RVU_ID rd, rs, rt, size


import sys

def create_trace(input_file, output_file, operation_size):
    output_trace = open(output_file, 'w+')
    input_trace = open(input_file)
    pim_instruction_opcode = '0000001'

    counter = 0;
    for trace in input_trace:
        counter=counter+1

    input_trace.close()

    input_trace = open(input_file)
    tmp = 0;
    for trace in input_trace:
        isa  = trace.split(" ")
        print isa[0]
        if   (isa[0] == "LOAD32"):
            rvu_opcode = '1000000'
            register = isa[1].split('r')
            rs = bin(int(register[1]))[2:].zfill(5)
            address = bin(int(isa[2]))[2:].zfill(34)
            rvu_instruction = address+rs+rvu_opcode
        elif (isa[0] == "LOAD64"):
            rvu_opcode = '1000001'
            register = isa[1].split('r')
            rs = bin(int(register[1]))[2:].zfill(5)
            address = bin(int(isa[2]))[2:].zfill(34)
            rvu_instruction = address+rs+rvu_opcode
        elif (isa[0] == "LOAD128"):
            rvu_opcode = '1000010'
            register = isa[1].split('r')
            rs = bin(int(register[1]))[2:].zfill(5)
            address = bin(int(isa[2]))[2:].zfill(34)
            rvu_instruction = address+rs+rvu_opcode
        elif (isa[0] == "LOAD256"):
            rvu_opcode = '1000011'
            register = isa[1].split('r')
            rs = bin(int(register[1]))[2:].zfill(5)
            address = bin(int(isa[2]))[2:].zfill(34)
            rvu_instruction = address+rs+rvu_opcode
        elif (isa[0] == "STORE32"):
            rvu_opcode = '1010000'
            register = isa[1].split('r')
            rs = bin(int(register[1]))[2:].zfill(5)
            address = bin(int(isa[2]))[2:].zfill(34)
            rvu_instruction = address+rs+rvu_opcode
        elif (isa[0] == "STORE64"):
            rvu_opcode = '1010000'
            register = isa[1].split('r')
            rs = bin(int(register[1]))[2:].zfill(5)
            address = bin(int(isa[2]))[2:].zfill(34)
            rvu_instruction = address+rs+rvu_opcode
        elif (isa[0] == "STORE128"):
            rvu_opcode = '1010010'
            register = isa[1].split('r')
            rs = bin(int(register[1]))[2:].zfill(5)
            address = bin(int(isa[2]))[2:].zfill(34)
            rvu_instruction = address+rs+rvu_opcode
        elif (isa[0] == "STORE256"):
            rvu_opcode = '1010011'
            register = isa[1].split('r')
            rs = bin(int(register[1]))[2:].zfill(5)
            address = bin(int(isa[2]))[2:].zfill(34)
            rvu_instruction = address+rs+rvu_opcode
        elif (isa[0] == "ADD"):
            rvu_opcode = '0100000'
            address = int(isa[1])
            if(operation_size == 32):
                address =  "000000000000000000000000"+bin(address)[2:].zfill(5)+"00000"
            elif operation_size == 64:
                address =  "00000000000000000000000"+bin(address)[2:].zfill(5)+"000000"
            elif operation_size == 128:
                address =  "0000000000000000000000"+bin(address)[2:].zfill(5)+"0000000"
            elif operation_size == 256:
                address =  "000000000000000000000"+bin(address)[2:].zfill(5)+"00000000"

            register = isa[2].split('r')
            rd = bin(int(register[1]))[2:].zfill(5)
            register = isa[3].split('r')
            rs = bin(int(register[1]))[2:].zfill(5)
            register = isa[4].split('r')
            rt = bin(int(register[1]))[2:].zfill(5)
            size = int(isa[5])
            size = bin(size)[2:].zfill(7)
            rvu_instruction = size+rd+rt+rs+rvu_opcode

        output_trace.write(pim_instruction_opcode)
        output_trace.write(' ')
        output_trace.write(address)
        output_trace.write(' ')
        output_trace.write("1")
        output_trace.write(' ')
        output_trace.write(rvu_instruction)
        tmp = tmp + 1
        if(counter != tmp):
            output_trace.write('\n')


num_of_arguments = len(sys.argv)
if(num_of_arguments != 4):
    print("Invalid number of arguments \n")
    print("Usage: python trace_generator_rvu.py input.txt output.txt MAX_ROW_BUFFER \n")
else:
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    row_buffer = int(sys.argv[3])
    create_trace(input_file, output_file, row_buffer)

