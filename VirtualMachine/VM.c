#include <stdio.h>
#include "VM.h"

void (*functions[COMMANDS_CNT])( processor_t *proc ) = {halt, push, add, sub, out, mul, pop, jump, ja, jae, jb, jbe, je, jne, call, ret, in, div};

int main( int argc, char* argv[] )
{
    const char *file_name = "assembler/code.txt";
    if (argc > 1)
        file_name = argv[1];

    ON_LOG(log_file_open("VirtualMachine/VMlog.log");)
    ON_LOG(LOG("source file %s", file_name);)
    FILE *code = fopen(file_name, "r");
    if (code == NULL)
    {
        log(myLOG_ERROR, "file errror\n");
        return 0;
    }

    ssize_t code_size = file_size(file_name) / (ssize_t)sizeof(byte);
    if (code_size < 0)
    {
        log(myLOG_ERROR, "wrong source file");
        return 0;
    }

    processor_t proc = {};

    proc.buffer = (byte*)calloc((size_t)code_size, sizeof(byte));
    if(proc.buffer == NULL)
    {
        log(myLOG_ERROR, "can't allocate buffer memory");
    }

    proc.RAM = (stackelem_t*)calloc(RAM_SIZE * RAM_SIZE, sizeof(stackelem_t));
    if(proc.RAM == NULL)
    {
        log(myLOG_ERROR, "can't allocate RAM memory");
    }

    fread(proc.buffer, sizeof(byte), (size_t)code_size, code);

    proc.ip = 0;
    proc.stk = {};
    stack_ctor(&proc.stk, 4);

    proc.call_stk = {};
    stack_ctor(&proc.call_stk, 4);

 //for (int i = 0; i < code_size; i++) printf("%d ", proc.buffer[i]); getchar();
#ifdef VMDEBUG
list_bytecode(&proc, (size_t)code_size);
#endif

    while (proc.ip <= (size_t)code_size)
    {
    #ifdef VMDEBUG
print_bytecode(&proc, (size_t)code_size);
#endif
        (functions[proc.buffer[proc.ip++] & COMMAND_MASK])(&proc);
    }

    fclose(code);
    halt(&proc);
    return 0;
}

ssize_t file_size( const char* filename )
{
    struct stat input_file_stat = {};

    if (stat(filename, &input_file_stat) == -1)
        return -1;

    return input_file_stat.st_size;
}

void halt( processor_t *proc )
{
#ifdef VMDEBUG
//dump_ram(proc);
draw_ram(proc);
#endif
    free(proc->buffer);
    free(proc->RAM);
    stack_detor(&proc->stk);
    stack_detor(&proc->call_stk);
    ON_LOG
    (
    LOG("HALT on ip %d", proc->ip);
    log_file_close();
    )

    exit(0);
}

void push( processor_t *proc )
{
    stackelem_t value = 0;
    //printf("command %x, command & KONST %x, command & RX, command & RAM %n \n", );
    int command = proc->buffer[proc->ip - 1];
    if (command & KONST)
    {
        get_value(&value, proc->buffer + proc->ip);
        proc->ip += sizeof(stackelem_t);
    }

    if (command & RX)
    {
        value += proc->rx[(size_t)proc->buffer[proc->ip++]];
    }

    if (command & RAM)
    {
        //printf("RAM %d = ", value);
        value = proc->RAM[(size_t)value];
        //printf("%d\n", value);
    }

    ON_LOG(LOG("PUSH " stack_specifier " on ip %d", value, proc->ip - 1);)
    //stackelem_t value = proc->buffer[proc->ip++];
    stack_push(&proc->stk, value);

    return;
}

void pop( processor_t *proc )
{
    stackelem_t stack_value = 0;
    stack_pop(&proc->stk, &stack_value);
    //printf("command %x, RX %x, RAM %x\n", proc->buffer[proc->ip - 1], RX, RAM);
    int command = proc->buffer[proc->ip - 1];

    if ((command & RX) && (command & RAM) && (command & KONST))
    {
        stackelem_t value = 0;
        get_value(&value, proc->buffer + proc->ip);
        proc->ip += sizeof(stackelem_t);
        ON_LOG(LOG("POP " stack_specifier" to RAM cell %d (from register %d + %d) ", stack_value, proc->rx[proc->buffer[proc->ip]] + value, proc->buffer[proc->ip - 1], value);)
        proc->RAM[(size_t)proc->rx[proc->buffer[proc->ip++]] + (size_t)value] = stack_value;
    }

    else if ((command & RX) && (command & RAM))
    {
        ON_LOG(LOG("POP " stack_specifier" to RAM cell %d (from register %d) ", stack_value, proc->rx[proc->buffer[proc->ip]], proc->buffer[proc->ip]);)
        proc->RAM[(size_t)proc->rx[proc->buffer[proc->ip++]]] = stack_value;
    }

    else if (command & RAM && command & KONST)
    {
        stackelem_t value = 0;
        get_value(&value, proc->buffer + proc->ip);
        proc->ip += sizeof(stackelem_t);
        ON_LOG(LOG("POP " stack_specifier" to RAM cell %d ", stack_value, value);)
        proc->RAM[(size_t)value] = stack_value;
    }

    else if (command & RX)
    {
        ON_LOG(LOG("POP " stack_specifier" to resgister %d ", stack_value, proc->buffer[proc->ip]);)
        proc->rx[proc->buffer[proc->ip++]] = stack_value;
    }

    //if (command & RAM) draw_ram(proc);

    return;
}

void add( processor_t *proc )
{
    ON_LOG(LOG("ADD on ip %d", proc->ip);)
    stackelem_t stack_value1 = 0, stack_value2 = 0;

    stack_pop(&proc->stk, &stack_value1);
    stack_pop(&proc->stk, &stack_value2);
    stack_push(&proc->stk, stack_value1 + stack_value2);

    return;
}

void sub( processor_t *proc )
{
    ON_LOG(LOG("SUB on ip %d", proc->ip);)
    stackelem_t stack_value1 = 0, stack_value2 = 0;

    stack_pop(&proc->stk, &stack_value1);
    stack_pop(&proc->stk, &stack_value2);
    stack_push(&proc->stk, stack_value1 - stack_value2);

    return;
}

void div( processor_t *proc )
{
    ON_LOG(LOG("DIV on ip %d", proc->ip);)
    stackelem_t stack_value1 = 0, stack_value2 = 0;

    stack_pop(&proc->stk, &stack_value1);
    stack_pop(&proc->stk, &stack_value2);
    stack_push(&proc->stk, stack_value1 / stack_value2);

    return;
}

void out( processor_t *proc )
{
    stackelem_t stack_value = 0;

    ON_LOG(log(myLOG_ACTION, "OUT");)
    stack_pop(&proc->stk, &stack_value);
    printf(stack_specifier"\n", stack_value);
}

void in( processor_t *proc )
{
    stackelem_t stack_value = 0;

    scanf(stack_specifier, &stack_value);
    ON_LOG(LOG("IN %d on ip %d",stack_value, proc->ip);)
    stack_push(&proc->stk, stack_value);

    return;
}

void mul( processor_t *proc )
{
    ON_LOG(log(myLOG_ACTION, "MULTIPLY");)
    stackelem_t stack_value1 = 0, stack_value2 = 0;

    stack_pop(&proc->stk, &stack_value1);
    stack_pop(&proc->stk, &stack_value2);
    stack_push(&proc->stk, stack_value1 * stack_value2);

    return;
}

void jump( processor_t *proc )
{
    ON_LOG(LOG("JUMP to ip %d", proc->buffer[proc->ip]))
    stackelem_t new_ip = 0;
    get_value(&new_ip, &proc->buffer[proc->ip]);
    proc->ip = (size_t)new_ip;

    return;
}

void jae( processor_t *proc )
{
    ON_LOG(LOG("JAE to ip %d", proc->buffer[proc->ip]))

    stackelem_t stack_value1 = 0, stack_value2 = 0;

    stack_pop(&proc->stk, &stack_value1);
    stack_pop(&proc->stk, &stack_value2);
    if (stack_value1 >= stack_value2)
    {
        stackelem_t new_ip = 0;
        get_value(&new_ip, &proc->buffer[proc->ip]);
        proc->ip = (size_t)new_ip;
    }
    else
        proc->ip += sizeof(stackelem_t);

    return;
}

void ja( processor_t *proc )
{
    ON_LOG(LOG("JA to ip %d", proc->buffer[proc->ip]))
    stackelem_t stack_value1 = 0, stack_value2 = 0;

    stack_pop(&proc->stk, &stack_value1);
    stack_pop(&proc->stk, &stack_value2);
    if (stack_value1 > stack_value2)
    {
        stackelem_t new_ip = 0;
        get_value(&new_ip, &proc->buffer[proc->ip]);
        proc->ip = (size_t)new_ip;
    }
    else
        proc->ip += sizeof(stackelem_t);

    return;
}

void jbe( processor_t *proc )
{
    ON_LOG(LOG("JBE to ip %d", proc->buffer[proc->ip]))
    stackelem_t stack_value1 = 0, stack_value2 = 0;

    stack_pop(&proc->stk, &stack_value1);
    stack_pop(&proc->stk, &stack_value2);
    if (stack_value1 <= stack_value2)
    {
        stackelem_t new_ip = 0;
        get_value(&new_ip, &proc->buffer[proc->ip]);
        proc->ip = (size_t)new_ip;
    }
    else
        proc->ip += sizeof(stackelem_t);

    return;
}

void jb( processor_t *proc )
{
    ON_LOG(LOG("JB to ip %d", proc->buffer[proc->ip]))
    stackelem_t stack_value1 = 0, stack_value2 = 0;

    stack_pop(&proc->stk, &stack_value1);
    stack_pop(&proc->stk, &stack_value2);
    if (stack_value1 < stack_value2)
    {
        stackelem_t new_ip = 0;
        get_value(&new_ip, &proc->buffer[proc->ip]);
        proc->ip = (size_t)new_ip;
    }
    else
        proc->ip += sizeof(stackelem_t);

    return;
}

void je( processor_t *proc )
{
    ON_LOG(LOG("JE to ip %d", proc->buffer[proc->ip]))
    stackelem_t stack_value1 = 0, stack_value2 = 0;

    stack_pop(&proc->stk, &stack_value1);
    stack_pop(&proc->stk, &stack_value2);
    if (stack_value1 == stack_value2)
    {
        stackelem_t new_ip = 0;
        get_value(&new_ip, &proc->buffer[proc->ip]);
        proc->ip = (size_t)new_ip;
    }
    else
        proc->ip += sizeof(stackelem_t);

    return;
}

void jne( processor_t *proc )
{
    ON_LOG(LOG("JNE to ip %d", proc->buffer[proc->ip]))
    stackelem_t stack_value1 = 0, stack_value2 = 0;

    stack_pop(&proc->stk, &stack_value1);
    stack_pop(&proc->stk, &stack_value2);
    if (stack_value1 != stack_value2)
    {
        stackelem_t new_ip = 0;
        get_value(&new_ip, &proc->buffer[proc->ip]);
        proc->ip = (size_t)new_ip;
    }
    else
        proc->ip += sizeof(stackelem_t);

    return;
}

void call( processor_t *proc )
{
    ON_LOG(LOG("CALL to ip %d", proc->buffer[proc->ip]))
    stack_push(&proc->call_stk, (stackelem_t)(proc->ip + sizeof(stackelem_t)));
    {
        stackelem_t new_ip = 0;
        get_value(&new_ip, &proc->buffer[proc->ip]);
        proc->ip = (size_t)new_ip;
    }

    return;
}

void ret( processor_t *proc )
{
    stackelem_t addr = 0;
    stack_pop(&proc->call_stk, &addr);
    ON_LOG(LOG("ret to ip %d", addr))
    proc->ip = (size_t)addr;

    return;
}

void get_value( stackelem_t *value, byte *ptr )
{
    memcpy(value, ptr, sizeof(stackelem_t));

    return;
}

void print_bytecode( processor_t *proc, size_t code_size )
{
    printf("|%8s|%8s|\t\t%8s|\n", "ip", "byte", "command");
    printf("|________|________|_____________________|\n");

    for (size_t i = (proc->ip <= 15 ? 0 : proc->ip - 15); i < (proc->ip + 15 >= code_size ? code_size : proc->ip + 15); i++)
    {
        int ram = 0, current = 0;
        byte cmd = proc->buffer[i] & COMMAND_MASK;
        if (i == proc->ip) current = 1;
        if (current) printf(myGREEN);
        printf("%9zu|", i);
        printf("%8d|", proc->buffer[i]);
        printf("\t\t");

        printf("%8s|", cmd_name(cmd));


        if (proc->buffer[i] & RAM) ram = 1;

        if (args_len(proc->buffer[i]) == 1)
        {
            printf("\n");
            printf("%9zu|", ++i);
            printf("%8d|", proc->buffer[i]);
            printf("\t\t");
            if (ram) printf("[%6s]|",reg_name(proc->buffer[i] & COMMAND_MASK));
            else     printf("%8s|",  reg_name(proc->buffer[i] & COMMAND_MASK));

        }
        if (args_len(proc->buffer[i]) == 4)
        {
            printf("\n");
            printf("%9zu|", ++i);
            printf("%8d|", proc->buffer[i]);
            printf("\t\t");
            stackelem_t value;
            get_value(&value, &proc->buffer[i]);
            if (ram) printf("[");
            printf(stack_specifier"|", value);
            if (ram) printf("]");
            i += sizeof(stackelem_t) - 1;
        }
        if (args_len(proc->buffer[i]) == 5)
        {
            printf("\n");
            printf("%9zu|", ++i);
            printf("%8d|", proc->buffer[i]);
            printf("\t\t");
            stackelem_t value;
            get_value(&value, &proc->buffer[i]);
            if (ram) printf("[");
            printf(stack_specifier, value);
            i += sizeof(stackelem_t) - 1;

            printf("\n");
            printf("%9zu|", ++i);
            printf("%8d|", proc->buffer[i]);
            printf("\t\t");
            if (ram) printf("+ %5s]|", reg_name(proc->buffer[i] & COMMAND_MASK));
            else     printf("+ %6s|",  reg_name(proc->buffer[i] & COMMAND_MASK));

        }
        if (current) printf(myWHITE);
        printf("\n");
    }
    printf("|________|________|_____________________|\n");

    char c = (char)getchar();
    if (c == 'q') halt(proc);

    return;
}

void list_bytecode( processor_t *proc, size_t code_size )
{
    FILE *listing = fopen("VirtualMachine/listing.txt", "w");
    if (listing == NULL)
    {
        printf("can't open listing file\n");
        return;
    }
    fprintf(listing, "|%8s|%8s|\t\t%8s|\n", "ip", "byte", "command");
    fprintf(listing, "|________|________|___________________|\n");

    for (size_t i = 0; i < code_size; i++)
    {
        int ram = 0, current = 0;
        byte cmd = proc->buffer[i] & COMMAND_MASK;
        if (i == proc->ip) current = 1;

        fprintf(listing, "%9zu|", i);
        fprintf(listing, "%8d|", proc->buffer[i]);
        fprintf(listing, "\t\t");

        fprintf(listing, "%8s|", cmd_name(cmd));


        if (proc->buffer[i] & RAM) ram = 1;

        if (args_len(proc->buffer[i]) == 1)
        {
            fprintf(listing, "\n");
            fprintf(listing, "%9zu|", ++i);
            fprintf(listing, "%8d|", proc->buffer[i]);
            fprintf(listing, "\t\t");
            if (ram) fprintf(listing, "[");
            if (ram) fprintf(listing, "%6s]|", reg_name(proc->buffer[i] & COMMAND_MASK));
            else     fprintf(listing, "%8s|",  reg_name(proc->buffer[i] & COMMAND_MASK));

        }
        if (args_len(proc->buffer[i]) == 4)
        {
            fprintf(listing, "\n");
            fprintf(listing, "%9zu|", ++i);
            fprintf(listing, "%8d|", proc->buffer[i]);
            fprintf(listing, "\t\t");
            stackelem_t value;
            get_value(&value, &proc->buffer[i]);
            if (ram) fprintf(listing, "[");
            fprintf(listing, stack_specifier"|", value);
            if (ram) fprintf(listing, "]");
            i += sizeof(stackelem_t) - 1;
        }
        if (args_len(proc->buffer[i]) == 5)
        {
            fprintf(listing, "\n");
            fprintf(listing, "%9zu|", ++i);
            fprintf(listing, "%8d|", proc->buffer[i]);
            fprintf(listing, "\t\t");
            stackelem_t value;
            get_value(&value, &proc->buffer[i]);
            if (ram) fprintf(listing, "[");
            fprintf(listing, stack_specifier, value);
            i += sizeof(stackelem_t) - 1;

            fprintf(listing, "\n");
            fprintf(listing, "%9zu|", ++i);
            fprintf(listing, "%8d|", proc->buffer[i]);
            fprintf(listing, "\t\t");
            if (ram) fprintf(listing, "+ %5s]|", reg_name(proc->buffer[i] & COMMAND_MASK));
            else     fprintf(listing, "+ %6s|",  reg_name(proc->buffer[i] & COMMAND_MASK));
        }
        fprintf(listing, "\n");
    }
    fprintf(listing, "|________|________|___________________|\n");
    fclose(listing);
    return;
}

const char* cmd_name( byte cmd )
{
    switch (cmd) {
        case HALT: return "HALT";
        case PUSH: return "PUSH";
        case ADD:  return "ADD";
        case SUB:  return "SUB";
        case OUT:  return "OUT";
        case MUL:  return "MUL";
        case POP:  return "POP";
        case JUMP: return "JUMP";
        case JA:   return "JA";
        case JAE:  return "JAE";
        case JB:   return "JB";
        case JBE:  return "JBE";
        case JE:   return "JE";
        case JNE:  return "JNE";
        case CALL: return "CALL";
        case RET:  return "RET";
        case IN:   return "IN";
        case DIV:  return "DIV";
        default:   return NULL;
    }
}

const char* reg_name( byte reg )
{
    switch (reg) {
        case AX: return "AX";
        case BX: return "BX";
        case CX: return "CX";
        case DX: return "DX";
        case EX: return "EX";
        case 5:  return "r5";
        case 6:  return "r6";
        case 7:  return "r7";
        case 8:  return "r8";
        case 9:  return "r9";
        case 10: return "r10";
        case 11: return "r11";
        case 12: return "r12";
        case 13: return "r13";
        case 14: return "r14";
        case 15: return "r15";
        default: return "hui";
    }
}

int args_len( byte cmd )
{
    byte bait = cmd;
    cmd = cmd & COMMAND_MASK;
    if (cmd == CALL || cmd == JUMP || cmd == JE || cmd == JAE || cmd == JB || cmd == JBE || cmd == JNE || cmd == JE)
        return sizeof(int);
    if (cmd == PUSH || cmd == POP)
    {
        int len = 0;
        if (bait & RX)
            len += sizeof(byte);
        if (bait & KONST)
            len += sizeof(int);
        return len;
    }

    return 0;
}
