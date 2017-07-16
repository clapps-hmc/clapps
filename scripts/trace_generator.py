import sys

link = 0
vault = 0
data = 1
tag = 0
vault_address = 0
bank = 0

def create_data(t, operation_size):
    global data
    
    total_pcks = operation_size/16
    t.write(str(total_pcks))
    for x in range(0, total_pcks):
        t.write(' '+str(data)+'')
        #data = data + 1
    
def create_responser(res, operation_size):
    global tag
    global link
    
    if(operation_size == 32):
        lower = "0001111111111111111111"+bin(link)[2:].zfill(3)+"1111111111111111"+bin(tag)[2:].zfill(11)+"000010001001"
    elif operation_size == 64:
        lower = "0001111111111111111111"+bin(link)[2:].zfill(3)+"1111111111111111"+bin(tag)[2:].zfill(11)+"000010001011"
    elif operation_size == 128:
        lower = "0001111111111111111111"+bin(link)[2:].zfill(3)+"1111111111111111"+bin(tag)[2:].zfill(11)+"000010001111"
    elif operation_size == 256:
        lower = "0001111111111111111111"+bin(link)[2:].zfill(3)+"1111111111111111"+bin(tag)[2:].zfill(11)+"000011001111"
        
        
    upper = "0000000000000000000000000000000000000000000000000000000000000000"
    
    res.write(upper)
    res.write('\n')
    res.write(lower)
    
    res.write('\n \n')
    
    tag = tag + 1
    link = (link + 1) % 4

def create_responser_read(res, operation_size):
    global tag
    global link
    global data

    lower = ["","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""]
    upper = ["","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""]
    
    total_pcks = operation_size/16 + 1
    
    for x in range(0,total_pcks):
        if(x == 0):
            if operation_size == 32:
                lower[x] = "0001111111111111111111"+bin(link)[2:].zfill(3)+"1111111111111111"+bin(tag)[2:].zfill(11)+"000110110001"
            elif operation_size == 64:
                lower[x] = "0001111111111111111111"+bin(link)[2:].zfill(3)+"1111111111111111"+bin(tag)[2:].zfill(11)+"001010110011"
            elif operation_size == 128:
                lower[x] = "0001111111111111111111"+bin(link)[2:].zfill(3)+"1111111111111111"+bin(tag)[2:].zfill(11)+"010010110111"
            elif operation_size == 256:
                lower[x] = "0001111111111111111111"+bin(link)[2:].zfill(3)+"1111111111111111"+bin(tag)[2:].zfill(11)+"100011110111"
        else:
            lower[x] = "0000000000000000000000000000000000000000000000000000000000000000"
        if (x == total_pcks - 1):
            upper[x] = "0000000000000000000000000000000000000000000000000000000000000000"
        else:
            upper[x] = str(bin(data)[2:].zfill(64))
            data = data + 1


    #lower[0] = "0001111111111111111111"+bin(link)[2:].zfill(3)+"1111111111111111"+bin(tag)[2:].zfill(11)+"001010110011"
    #upper[0] = str(bin(data)[2:].zfill(64))
    #data = data + 1

    #lower[1] = "0000000000000000000000000000000000000000000000000000000000000000"
    #upper[1] = str(bin(data)[2:].zfill(64))
    #data = data + 1
    
    #lower[2] = "0000000000000000000000000000000000000000000000000000000000000000"
    #upper[2] = str(bin(data)[2:].zfill(64))
    #data = data + 1

    #lower[3] = "0000000000000000000000000000000000000000000000000000000000000000"
    #upper[3] = str(bin(data)[2:].zfill(64))
    #data = data + 1

    #lower[4] = "0000000000000000000000000000000000000000000000000000000000000000"
    #upper[4] = "0000000000000000000000000000000000000000000000000000000000000000"

    for x in range(0,total_pcks):
        res.write(upper[x])
        res.write('\n')
        res.write(lower[x])
    
        res.write('\n \n')

    
    tag = tag + 1
    link = (link + 1) % 4
    

def get_address(operation_size):
    global link
    global vault
    global bank
    global vault_address
    if(operation_size == 32):
        tmp =  "00000000000000000000"+bin(bank)[2:].zfill(4)+bin(link)[2:].zfill(2)+bin(vault)[2:].zfill(3)+"00000"
    elif operation_size == 64:
        tmp =  "0000000000000000000"+bin(bank)[2:].zfill(4)+bin(link)[2:].zfill(2)+bin(vault)[2:].zfill(3)+"000000"
    elif operation_size == 128:
        tmp =  "000000000000000000"+bin(bank)[2:].zfill(4)+bin(link)[2:].zfill(2)+bin(vault)[2:].zfill(3)+"0000000"
    elif operation_size == 256:
        tmp =  "00000000000000000"+bin(bank)[2:].zfill(4)+bin(link)[2:].zfill(2)+bin(vault)[2:].zfill(3)+"00000000"

    print("link: ",link ," vault: ",vault," bank: ",bank,"\n")
    vault_address = vault_address + 1

    if(vault_address == 32):
        vault_address = 0
        bank = (bank + 1) % 16
    if(link == 3):
        vault = (vault + 1) % 8
        
    return tmp

def reset_address():
    global link
    global vault
    global data
    global bank
    global vault_address
    
    link = 0
    vault = 0
    data = 1
    bank = 0
    vault_address = 0
    
def create_read(testbench, operation_size, banks, responser):
    for x in range (0, banks): #READ
            if(operation_size == 32):
                testbench.write('0110001')
            elif(operation_size == 64):
                testbench.write('0110011')
            elif(operation_size == 128):
                testbench.write('0110111')
            elif(operation_size == 256):
                testbench.write('1110111')
                
            testbench.write(' '+get_address(operation_size)+' ')
            if(x != banks -1):
                testbench.write('0 \n')    
            else:
                testbench.write('0')  
            create_responser_read(responser, operation_size)
            
    
def create_write(testbench, operation_size, banks, responser):
    for x in range (0, banks): #WRITE
        if(operation_size == 32):
            testbench.write('0001001')
        elif(operation_size == 64):
            testbench.write('0001011')
        elif(operation_size == 128):
            testbench.write('0001111')
        elif(operation_size == 256):
            testbench.write('1001111')
        
        testbench.write(' '+get_address(operation_size)+' ')
        create_data(testbench, operation_size)
       
        create_responser(responser, operation_size)
        if(x != banks -1):
            testbench.write('\n')
    
    
def create_template(banks, output, operation, operation_size):
    print banks, output
    testbench = open('trace.txt', 'w+')
    responser = open(output, 'w+')
    
    if(operation == "W"):
        print("write \n")
        create_write(testbench, operation_size, banks, responser)

    if(operation == "R"):
        print("\n \n read \n")
        reset_address() 
        create_read(testbench, operation_size, banks, responser)
        
    if(operation == "WR"):  
        print("write \n")
        create_write(testbench, operation_size, banks, responser)    
        testbench.write('\n')
        print("\n \n read \n")
        reset_address() 
        create_read(testbench, operation_size, banks, responser)    
        
    
    testbench.close();
    responser.close()

#usage number of banks - output name - W/R/WR - 32/64/128/256B
num_of_arguments = len(sys.argv)
if(num_of_arguments != 5):
    print("Invalid number of arguments \n")
else:
    num_banks = int(sys.argv[1])
    golden_output = (sys.argv[2])
    op = (sys.argv[3])
    op_size = int(sys.argv[4])
    
    create_template(num_banks, golden_output, op, op_size)
