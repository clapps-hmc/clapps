import sys
import random

link = 0
vault = 0
data = 0
tag = 0
vault_address = 0
bank = 0
prev_link = -1
prev_vault = -1
prev_bank = -1 

def get_decimal_value(num):
    return sum(map(lambda x: x[1]*(2**x[0]), enumerate(map(int, str(num))[::-1])))
    
def create_data(t):
    global data
    
    total_pcks = 2
    t.write(str(total_pcks))
    for x in range(0, total_pcks):        
        t.write(' '+str(0)+'')
    
def create_responser(res):
    global tag
    global link
    
    
    lower = "0001111111111111111111"+bin(link)[2:].zfill(3)+"1111111111111111"+bin(tag)[2:].zfill(11)+"000010001001"
    upper = "0000000000000000000000000000000000000000000000000000000000000000"
    
    res.write(upper)
    res.write('\n')
    res.write(lower)
    
    res.write('\n \n')
    
    tag = tag + 1
    link = (link + 1) % 4

def create_responser_read(res):
    global tag
    global link
    global data

    lower = ["","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""]
    upper = ["","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""]
    
    total_pcks = 32/16 + 1
    
    for x in range(0,total_pcks):
        if(x == 0):
            lower[x] = "0001111111111111111111"+bin(link)[2:].zfill(3)+"1111111111111111"+bin(tag)[2:].zfill(11)+"000110110001"
        else:
            lower[x] = str(bin(1)[2:].zfill(64))
        if (x == total_pcks - 1):
            upper[x] = "0000000000000000000000000000000000000000000000000000000000000000"
        else:
            upper[x] = str(bin(1)[2:].zfill(64))

    for x in range(0,total_pcks):
        res.write(upper[x])
        res.write('\n')
        res.write(lower[x])
    
        res.write('\n \n')

    
    tag = tag + 1
    link = (link + 1) % 4
    

def get_address(flag, next_address):
    global link
    global vault
    global bank
    global vault_address
    global prev_link
    global prev_vault
    global prev_bank
    
    bank = random.randint(0,15)
    vault = random.randint(0,7)
    link = random.randint(0,3)
    
    while((prev_link == link) and (prev_bank == bank) and (prev_vault == vault)):
        bank = random.randint(0,15)
        vault = random.randint(0,7)
        link = random.randint(0,3)
   
    tmp =  "00000000000000000000"+bin(bank)[2:].zfill(4)+bin(link)[2:].zfill(2)+bin(vault)[2:].zfill(3)+str(flag)+"0000"
   
    prev_bank = bank
    prev_link = link
    prev_vault = vault

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
    
def create_read(testbench, banks, responser):
    for x in range (0, banks): #READ
        testbench.write('0110001')
        testbench.write(' '+get_address(0,1)+' ')
        
        if(x != banks -1):
            testbench.write('0 \n')    
        else:
            testbench.write('0')  
            
        create_responser_read(responser)
            
    
def create_write(testbench, banks, responser):
    for x in range (0, banks): #WRITE
        testbench.write('0001001')
    
        testbench.write(' '+get_address(0,1)+' ')
        create_data(testbench)
       
        create_responser(responser)
        if(x != banks -1):
            testbench.write('\n')
    
def create_responser_pim(res, next_address):
    global tag
    global link
    
    
    lower = "0001111111111111111111"+bin(link)[2:].zfill(3)+"1111111111111111"+bin(tag)[2:].zfill(11)+"000010010010"
    upper = "0000000000000000000000000000000000000000000000000000000000000000"
    
    res.write(upper)
    res.write('\n')
    res.write(lower)
    
    res.write('\n \n')
    
    tag = tag + 1
    #if(next_address == 1):
     #   link = (link + 1) % 4
    
def create_pim(testbench, banks, responser):
    for x in range (0, banks): #READ
        testbench.write('0010010')
        testbench.write(' '+get_address(0,1)+' ')
        testbench.write('1 1')
        create_responser_pim(responser, 1)
        
   #     testbench.write('0010010')
   #     testbench.write(' '+get_address(1,1)+' ')
   #     testbench.write('1 1')
   #     create_responser_pim(responser, 1)
        
        if(x != banks -1):
            testbench.write('\n')
            
    
    
def create_template(banks, output,trace_name):
    print banks, output
    testbench = open(trace_name, 'w+')
    responser = open(output, 'w+')
     

    create_pim(testbench, banks, responser)
    
    testbench.close();
    responser.close()

num_of_arguments = len(sys.argv)
if(num_of_arguments != 4):
    print("Invalid number of arguments \n")
    print("Usage: python trace_generator_pim_rand.py NUMBER_INSTRUCTIONS GOLD_NAME TRACE_NAME \n")
else:
    num_banks = int(sys.argv[1])
    golden_output = (sys.argv[2])
    trace_name = (sys.argv[3])

    create_template(num_banks, golden_output,trace_name)
