#include <stdio.h>
#include <string.h>
#include <cstdlib>



int decode (unsigned char *buffer, int program_counter, FILE *fp );

int main(int argc, char* argv[])
{
    FILE *f= fopen(argv[1], "rb");    
    if (f==NULL)    
    {    
        printf("error: Couldn't open %s\n", argv[1]);    
        exit(1);    
    }    

    //Get the file size and read it into a memory buffer    
    fseek(f, 0L, SEEK_END);    
    int fsize = ftell(f);    
    fseek(f, 0L, SEEK_SET);    

    unsigned char *buffer= (unsigned char*) malloc(fsize);    
    unsigned program_counter =0 ;
    fread(buffer, fsize, 1, f);    
    fclose(f);  

    FILE *fp= fopen( "file.txt" , "wb" );


    for (int i=0; i< fsize; i++){
       program_counter += decode(buffer, program_counter, fp);
        
    }

    fclose(fp);

}   

int decode (unsigned char *buffer, int program_counter, FILE *fp)
{

    unsigned char *temp_buffer= &buffer[program_counter];
    int opcode_lenb =1;
    // printf("%02x ", *temp_buffer);
    fprintf(fp, "%04x %s", program_counter, " ");
    switch(*temp_buffer){
        case 0x0 : fprintf(fp, "%s", "NOP\n"); break;
        case 0x1 : fprintf(fp, "%s %02x %02x\n", "LXI B,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0x2 : fprintf(fp, "%s", "STAX B\n"); break;
        case 0x3 : fprintf(fp, "%s", "INX B\n"); break;
        case 0x4 : fprintf(fp, "%s", "INR B\n"); break;
        case 0x5 : fprintf(fp, "%s", "DCR B\n"); break;
        case 0x6 : fprintf(fp, "%s %02x\n", "MVI B," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0x7 : fprintf(fp, "%s", "RLC\n"); break;
        case 0x8 : fprintf(fp, "%s", "NOP\n"); break;
        case 0x9 : fprintf(fp, "%s", "DAD B\n"); break;
        case 0xa : fprintf(fp, "%s", "LDAX B\n"); break;
        case 0xb : fprintf(fp, "%s", "DCX B\n"); break;
        case 0xc : fprintf(fp, "%s", "INR C\n"); break;
        case 0xd : fprintf(fp, "%s", "DCR C\n"); break;
        case 0xe : fprintf(fp, "%s %02x\n", "MVI C," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0xf : fprintf(fp, "%s", "RRC\n"); break;
        case 0x10 : fprintf(fp, "%s", "NOP\n"); break;
        case 0x11 : fprintf(fp, "%s %02x %02x\n", "LXI D,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0x12 : fprintf(fp, "%s", "STAX D\n"); break;
        case 0x13 : fprintf(fp, "%s", "INX D\n"); break;
        case 0x14 : fprintf(fp, "%s", "INR D\n"); break;
        case 0x15 : fprintf(fp, "%s", "DCR D\n"); break;
        case 0x16 : fprintf(fp, "%s %02x\n", "MVI D," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0x17 : fprintf(fp, "%s", "RAL\n"); break;
        case 0x18 : fprintf(fp, "%s", "NOP\n"); break;
        case 0x19 : fprintf(fp, "%s", "DAD D\n"); break;
        case 0x1a : fprintf(fp, "%s", "LDAX D\n"); break;
        case 0x1b : fprintf(fp, "%s", "DCX D\n"); break;
        case 0x1c : fprintf(fp, "%s", "INR E\n"); break;
        case 0x1d : fprintf(fp, "%s", "DCR E\n"); break;
        case 0x1e : fprintf(fp, "%s %02x\n", "MVI E," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0x1f : fprintf(fp, "%s", "RAR\n"); break;
        case 0x20 : fprintf(fp, "%s", "NOP\n"); break;
        case 0x21 : fprintf(fp, "%s %02x%02x\n", "LXI H,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0x22 : fprintf(fp, "%s %02x%02x\n", "SHLD,", temp_buffer[1], temp_buffer[2]);  opcode_lenb = 3; break;
        case 0x23 : fprintf(fp, "%s", "INX H\n"); break;
        case 0x24 : fprintf(fp, "%s", "INR H\n"); break;
        case 0x25 : fprintf(fp, "%s", "DCR H\n"); break;
        case 0x26 : fprintf(fp, "%s %02x\n", "MVI H," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0x27 : fprintf(fp, "%s", "DAA\n"); break;
        case 0x28 : fprintf(fp, "%s", "NOP\n"); break;
        case 0x29 : fprintf(fp, "%s", "DAD H\n"); break;
        case 0x2a : fprintf(fp, "%s %02x%02x\n", "LHLD", temp_buffer[1], temp_buffer[2]);  opcode_lenb = 3; break;
        case 0x2b : fprintf(fp, "%s", "DCX H\n"); break;
        case 0x2c : fprintf(fp, "%s", "INR L\n"); break;
        case 0x2d : fprintf(fp, "%s", "DCR L\n"); break;
        case 0x2e : fprintf(fp, "%s %02x\n", "MVI L," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0x2f : fprintf(fp, "%s", "CMA\n"); break;
        case 0x30 : fprintf(fp, "%s", "NOP\n"); break;
        case 0x31 : fprintf(fp, "%s %02x%02x\n", "LXI SP,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0x32 : fprintf(fp, "%s %02x%02x\n", "STA", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0x33 : fprintf(fp, "%s", "INX SP\n"); break;
        case 0x34 : fprintf(fp, "%s", "INR M\n"); break;
        case 0x35 : fprintf(fp, "%s", "DCR M\n"); break;
        case 0x36 : fprintf(fp, "%s %02x\n", "MVI M," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0x37 : fprintf(fp, "%s", "STC\n"); break;
        case 0x38 : fprintf(fp, "%s", "NOP\n"); break;
        case 0x39 : fprintf(fp, "%s", "DAD SP\n"); break;
        case 0x3a : fprintf(fp, "%s %02x%02x\n", "LDA", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0x3b : fprintf(fp, "%s", "DCX SP\n"); break;
        case 0x3c : fprintf(fp, "%s", "INR A\n"); break;
        case 0x3d : fprintf(fp, "%s", "DCR An"); break;
        case 0x3e : fprintf(fp, "%s %02x\n", "MVI A," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0x3f : fprintf(fp, "%s", "CMC\n"); break;
        case 0x40 : fprintf(fp, "%s", "MOV B,B\n"); break;
        case 0x41 : fprintf(fp, "%s", "MOV B,C\n"); break;
        case 0x42 : fprintf(fp, "%s", "MOV B,D\n"); break;
        case 0x43 : fprintf(fp, "%s", "MOV B,E\n"); break;
        case 0x44 :fprintf(fp, "%s", "MOV B,H\n"); break;
        case 0x45 :fprintf(fp, "%s", "MOV B,L\n"); break;
        case 0x46 :fprintf(fp, "%s", "MOV B,M\n"); break;
        case 0x47 :fprintf(fp, "%s", "MOV B,A\n"); break;
        case 0x48 :fprintf(fp, "%s", "MOV C,B\n"); break;
        case 0x49 :fprintf(fp, "%s", "MOV C,C\n"); break;
        case 0x4a :fprintf(fp, "%s", "MOV C,D\n"); break;
        case 0x4b :fprintf(fp, "%s", "MOV C,E\n"); break;
        case 0x4c :fprintf(fp, "%s", "MOV C,H\n"); break;
        case 0x4d :fprintf(fp, "%s", "MOV C,L\n"); break;
        case 0x4e :fprintf(fp, "%s", "MOV C,M\n"); break;
        case 0x4f :fprintf(fp, "%s", "MOV C,A\n"); break;
        case 0x50 :fprintf(fp, "%s", "MOV D,B\n"); break;
        case 0x51 :fprintf(fp, "%s", "MOV D,C\n"); break;
        case 0x52 :fprintf(fp, "%s", "MOV D,D\n"); break;
        case 0x53 :fprintf(fp, "%s", "MOV D,E\n"); break;
        case 0x54 :fprintf(fp, "%s", "MOV D,H\n"); break;
        case 0x55 :fprintf(fp, "%s", "MOV D,L\n"); break;
        case 0x56 :fprintf(fp, "%s", "MOV D,M\n"); break;
        case 0x57 :fprintf(fp, "%s", "MOV D,A\n"); break;
        case 0x58 :fprintf(fp, "%s", "MOV E,B\n"); break;
        case 0x59 :fprintf(fp, "%s", "MOV E,C\n"); break;
        case 0x5a :fprintf(fp, "%s", "MOV E,D\n"); break;
        case 0x5b :fprintf(fp, "%s", "MOV E,E\n"); break;
        case 0x5c :fprintf(fp, "%s", "MOV E,H\n"); break;
        case 0x5d :fprintf(fp, "%s", "MOV E,L\n"); break;
        case 0x5e :fprintf(fp, "%s", "MOV E,M\n"); break;
        case 0x5f :fprintf(fp, "%s", "MOV E,A\n"); break;
        case 0x60 :fprintf(fp, "%s", "MOV H,B\n"); break;
        case 0x61 :fprintf(fp, "%s", "MOV H,C\n"); break;
        case 0x62 :fprintf(fp, "%s", "MOV H,D\n"); break;
        case 0x63 :fprintf(fp, "%s", "MOV H,E\n"); break;
        case 0x64 :fprintf(fp, "%s", "MOV H,H\n"); break;
        case 0x65 :fprintf(fp, "%s", "MOV H,L\n"); break;
        case 0x66 :fprintf(fp, "%s", "MOV H,M\n"); break;
        case 0x67 :fprintf(fp, "%s", "MOV H,A\n"); break;
        case 0x68 :fprintf(fp, "%s", "MOV L,B\n"); break;
        case 0x69 :fprintf(fp, "%s", "MOV L,C\n"); break;
        case 0x6a :fprintf(fp, "%s", "MOV L,D\n"); break;
        case 0x6b :fprintf(fp, "%s", "MOV L,E\n"); break;
        case 0x6c :fprintf(fp, "%s", "MOV L,H\n"); break;
        case 0x6d :fprintf(fp, "%s", "MOV L,L\n"); break;
        case 0x6e :fprintf(fp, "%s", "MOV L,M\n"); break;
        case 0x6f :fprintf(fp, "%s", "MOV L,A\n"); break;
        case 0x70 :fprintf(fp, "%s", "MOV M,B\n"); break;
        case 0x71 :fprintf(fp, "%s", "MOV M,C\n"); break;
        case 0x72 :fprintf(fp, "%s", "MOV M,D\n"); break;
        case 0x73 :fprintf(fp, "%s", "MOV M,E\n"); break;
        case 0x74 :fprintf(fp, "%s", "MOV M,H\n"); break;
        case 0x75 :fprintf(fp, "%s", "MOV M,L\n"); break;
        case 0x76 :fprintf(fp, "%s", "HLT\n"); break;
        case 0x77 :fprintf(fp, "%s", "MOV M,A\n"); break;
        case 0x78 :fprintf(fp, "%s", "MOV A,B\n"); break;
        case 0x79 :fprintf(fp, "%s", "MOV B,C\n"); break;
        case 0x7a :fprintf(fp, "%s", "MOV A,D\n"); break;
        case 0x7b :fprintf(fp, "%s", "MOV A,E\n"); break;
        case 0x7c :fprintf(fp, "%s", "MOV A,H\n"); break;
        case 0x7d :fprintf(fp, "%s", "MOV A,L\n"); break;
        case 0x7e :fprintf(fp, "%s", "MOV A,M\n"); break;
        case 0x7f :fprintf(fp, "%s", "MOV A,A\n"); break;
        case 0x80 :fprintf(fp, "%s", "ADD B\n"); break;
        case 0x81 :fprintf(fp, "%s", "ADD C\n"); break;
        case 0x82 :fprintf(fp, "%s", "ADD D\n"); break;
        case 0x83 :fprintf(fp, "%s", "ADD E\n"); break;
        case 0x84 :fprintf(fp, "%s", "ADD H\n"); break;
        case 0x85 :fprintf(fp, "%s", "ADD L\n"); break;
        case 0x86 :fprintf(fp, "%s", "ADD M\n"); break;
        case 0x87 :fprintf(fp, "%s", "ADD A\n"); break;
        case 0x88 :fprintf(fp, "%s", "ADC B\n"); break;
        case 0x89 :fprintf(fp, "%s", "ADC C\n"); break;
        case 0x8a :fprintf(fp, "%s", "ADC D\n"); break;
        case 0x8b :fprintf(fp, "%s", "ADC E\n"); break;
        case 0x8c :fprintf(fp, "%s", "ADC H\n"); break;
        case 0x8d :fprintf(fp, "%s", "ADC L\n"); break;
        case 0x8e :fprintf(fp, "%s", "ADC M\n"); break;
        case 0x8f :fprintf(fp, "%s", "ADC A\n"); break;
        case 0x90 :fprintf(fp, "%s", "SUB B\n"); break;
        case 0x91 :fprintf(fp, "%s", "SUB C\n"); break;
        case 0x92 :fprintf(fp, "%s", "SUB D\n"); break;
        case 0x93 :fprintf(fp, "%s", "SUB E\n"); break;
        case 0x94 :fprintf(fp, "%s", "SUB H\n"); break;
        case 0x95 :fprintf(fp, "%s", "SUB L\n"); break;
        case 0x96 :fprintf(fp, "%s", "SUB M\n"); break;
        case 0x97 :fprintf(fp, "%s", "SUB A\n"); break;
        case 0x98 :fprintf(fp, "%s", "SBB B\n"); break;
        case 0x99 :fprintf(fp, "%s", "SBB C\n"); break;
        case 0x9a :fprintf(fp, "%s", "SBB D\n"); break;
        case 0x9b :fprintf(fp, "%s", "SBB E\n"); break;
        case 0x9c :fprintf(fp, "%s", "SBB H\n"); break;
        case 0x9d :fprintf(fp, "%s", "SBB L\n"); break;
        case 0x9e :fprintf(fp, "%s", "SBB M\n"); break;
        case 0x9f :fprintf(fp, "%s", "SBB A\n"); break;
        case 0xa0 :fprintf(fp, "%s", "ANA B\n"); break;
        case 0xa1 :fprintf(fp, "%s", "ANA C\n"); break;
        case 0xa2 :fprintf(fp, "%s", "ANA D\n"); break;
        case 0xa3 :fprintf(fp, "%s", "ANA E\n"); break;
        case 0xa4 :fprintf(fp, "%s", "ANA H\n"); break;
        case 0xa5 :fprintf(fp, "%s", "ANA L\n"); break;
        case 0xa6 :fprintf(fp, "%s", "ANA M\n"); break;
        case 0xa7 :fprintf(fp, "%s", "ANA A\n"); break;
        case 0xa8 :fprintf(fp, "%s", "XRA B\n"); break;
        case 0xa9 :fprintf(fp, "%s", "XRA C\n"); break;
        case 0xaa :fprintf(fp, "%s", "XRA D\n"); break;
        case 0xab :fprintf(fp, "%s", "XRA E\n"); break;
        case 0xac :fprintf(fp, "%s", "XRA H\n"); break;
        case 0xad :fprintf(fp, "%s", "XRA L\n"); break;
        case 0xae :fprintf(fp, "%s", "XRA M\n"); break;
        case 0xaf :fprintf(fp, "%s", "XRA A\n"); break;
        case 0xb0 :fprintf(fp, "%s", "ORA B\n"); break;
        case 0xb1 :fprintf(fp, "%s", "ORA C\n"); break;
        case 0xb2 :fprintf(fp, "%s", "ORA D\n"); break;
        case 0xb3 :fprintf(fp, "%s", "ORA E\n"); break;
        case 0xb4 :fprintf(fp, "%s", "ORA H\n"); break;
        case 0xb5 :fprintf(fp, "%s", "ORA L\n"); break;
        case 0xb6 :fprintf(fp, "%s", "ORA M\n"); break;
        case 0xb7 :fprintf(fp, "%s", "ORA A\n"); break;
        case 0xb8 :fprintf(fp, "%s", "CMP B\n"); break;
        case 0xb9 :fprintf(fp, "%s", "CMP C\n"); break;
        case 0xba :fprintf(fp, "%s", "CMP D\n"); break;
        case 0xbb :fprintf(fp, "%s", "CMP E\n"); break;
        case 0xbc :fprintf(fp, "%s", "CMP H\n"); break;
        case 0xbd :fprintf(fp, "%s", "CMP L\n"); break;
        case 0xbe :fprintf(fp, "%s", "CMP M\n"); break;
        case 0xbf :fprintf(fp, "%s", "CMP A\n"); break;
        case 0xc0 :fprintf(fp, "%s", "RNZ\n"); break;
        case 0xc1 :fprintf(fp, "%s", "POP B\n"); break;
        case 0xc2 :fprintf(fp, "%s %02x%02x\n", "JNZ", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xc3 :fprintf(fp, "%s %02x%02x\n", "JMP", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xc4 :fprintf(fp, "%s %02x%02x\n", "CNZ", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xc5 :fprintf(fp, "%s", "PUSH B\n"); break;
        case 0xc6 :fprintf(fp, "%s %02x\n", "ADI," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0xc7 :fprintf(fp, "%s", "RST $0\n"); break;
        case 0xc8 :fprintf(fp, "%s", "RZ\n"); break;
        case 0xc9 :fprintf(fp, "%s", "RET\n"); break; 
        case 0xca :fprintf(fp, "%s %02x%02x\n", "JZ,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xcb :fprintf(fp, "%s %02x%02x\n", "JMP,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xcc :fprintf(fp, "%s %02x%02x\n", "CZ,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xcd :fprintf(fp, "%s %02x%02x\n", "CALL,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xce :fprintf(fp, "%s %02x\n", "ACI," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0xcf :fprintf(fp, "%s", "RST $8\n"); break;
        case 0xd0 :fprintf(fp, "%s", "RNC\n"); break;
        case 0xd1 :fprintf(fp, "%s", "POP D\n"); break;
        case 0xd2 :fprintf(fp, "%s %02x%02x\n", "JNC,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xd3 :fprintf(fp, "%s %02x\n", "OUT," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0xd4 :fprintf(fp, "%s %02x%02x\n", "CNC,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xd5 :fprintf(fp, "%s", "PUSH D\n"); break;
        case 0xd6 :fprintf(fp, "%s %02x\n", "SUI," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0xd7 :fprintf(fp, "%s", "RST $10\n"); break;
        case 0xd8 :fprintf(fp, "%s", "RC\n"); break;
        case 0xd9 :fprintf(fp, "%s", "RET\n"); break;
        case 0xda :fprintf(fp, "%s %02x%02x\n", "JC,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xdb :fprintf(fp, "%s %02x\n", "IN," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0xdc :fprintf(fp, "%s %02x%02x\n", "CC,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xdd :fprintf(fp, "%s %02x%02x\n", "CALL,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xde :fprintf(fp, "%s %02x\n", "SBI," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0xdf :fprintf(fp, "%s", "RST $18\n"); break;
        case 0xe0 :fprintf(fp, "%s", "RPO\n"); break;
        case 0xe1 :fprintf(fp, "%s", "POP H\n"); break;
        case 0xe2 :fprintf(fp, "%s %02x%02x\n", "JPO,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xe3 :fprintf(fp, "%s", "XTHL\n"); break;
        case 0xe4 :fprintf(fp, "%s %02x%02x\n", "CPO,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xe5 :fprintf(fp, "%s", "PUSH H\n"); break;
        case 0xe6 :fprintf(fp, "%s %02x\n", "ANI," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0xe7 :fprintf(fp, "%s", "RST $20\n"); break;
        case 0xe8 :fprintf(fp, "%s", "RPE\n"); break;
        case 0xe9 :fprintf(fp, "%s", "PCHL\n"); break;
        case 0xea :fprintf(fp, "%s %02x%02x\n", "JPE,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xeb :fprintf(fp, "%s", "XCHG\n"); break;
        case 0xec :fprintf(fp, "%s %02x%02x\n", "CPE,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xed :fprintf(fp, "%s %02x%02x\n", "CALL,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xee :fprintf(fp, "%s %02x\n", "XRI," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0xef :fprintf(fp, "%s", "RST $28\n"); break;
        case 0xf0 :fprintf(fp, "%s", "RP\n"); break;
        case 0xf1 :fprintf(fp, "%s", "POP PSW\n"); break;
        case 0xf2 :fprintf(fp, "%s %02x%02x\n", "JP,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xf3 :fprintf(fp, "%s", "DI\n"); break;
        case 0xf4 :fprintf(fp, "%s %02x%02x\n", "CP,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xf5 :fprintf(fp, "%s", "PUSH PSW\n"); break;
        case 0xf6 :fprintf(fp, "%s %02x\n", "ORI," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0xf7 :fprintf(fp, "%s", "RST $30\n"); break;
        case 0xf8 :fprintf(fp, "%s", "RM\n"); break;
        case 0xf9 :fprintf(fp, "%s", "SPHL\n"); break;
        case 0xfa :fprintf(fp, "%s %02x%02x\n", "JM,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xfb :fprintf(fp, "%s", "EI\n"); break;
        case 0xfc :fprintf(fp, "%s %02x%02x\n", "CM,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xfd :fprintf(fp, "%s %02x%02x\n", "CALL,", temp_buffer[2], temp_buffer[1]);  opcode_lenb = 3; break;
        case 0xfe :fprintf(fp, "%s %02x\n", "CPI," ,temp_buffer[1] ); opcode_lenb = 2; break;
        case 0xff :fprintf(fp, "%s", "RST $38\n"); break;

    }
//printf("%d\n",opcode_lenb);
return opcode_lenb; 



}