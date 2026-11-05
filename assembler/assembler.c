#include "assembler.h"
#include "sort.h"
//#define ONDEBUG

//const char *source_file_name = "assembler/factorial.asm";
const char *source_file_name = "assembler/circle.asm";
//const char *source_file_name = "assembler/code.asm";
//const char *source_file_name = "assembler/sqr.asm";
//const char *source_file_name = "assembler/pisun.asm";
const char *output_file_name = "assembler/code.txt";

int main( void )
{
    FILE *output_file = fopen(output_file_name, "w");
    if (output_file == NULL)
    {
        printf("output file error\n");
        return 0;
    }

    // ON_LOG(log_file_open("assembler/ASMBlog.log");)
    // ON_LOG(LOG("source file %s", file_name);)

    assembler_t asmb = {};
    assembler_init(&asmb);

// for (asmb.line_number = 0; asmb.line_number < asmb.string_cnt; asmb.line_number++)
// {
//     printf("%s\n", asmb.lines[asmb.line_number].index);
// }
// printf("pritn");
    //______________________________________
    for (int k = 0; k < 2; k++)
    {

        if (k == 1)
        {
            //for(int i = 0; i < asmb.labels.labels_cnt; i++){printf("%d label hash %lu ip %d\n", i, asmb.labels.Keys[i], asmb.labels.Labels[i] );}
            //printf("\n\n\n");
            sort_labels(&asmb.labels);
            //for(int i = 0; i < asmb.labels.labels_cnt; i++){printf("%d label hash %lu ip %d\n", i, asmb.labels.Keys[i], asmb.labels.Labels[i] );}
        }
        asmb.ip = 0;
        for (asmb.line_number = 0; asmb.line_number < asmb.string_cnt; asmb.line_number++)
        {
#ifdef ONDEBUG
printf("%s\n", asmb.lines[asmb.line_number].index);
#endif
            if (sscanf(asmb.lines[asmb.line_number].index, "%s", asmb.command) != -1)
            {
                if (asmb.command[0] == ';')
                    continue;

                if (asmb.command[0] == ':')
                {
                    if (k == 0) place_label(&asmb);
//printf("assembly %d  ip %zu label %s\n", k, asmb.ip, asmb.lines[asmb.line_number].index);
                    continue;
                }
//printf("assembly %d command %s ip %zu\n", k, asmb.command, asmb.ip);
                switch(asmb.command[0])
                {
                    case 'P':
                        sscanf(asmb.lines[asmb.line_number].index, "%*s" stack_specifier, &asmb.value);
                        if (strcmp(asmb.command, "PUSH") == 0)
                        {
                            push(&asmb);
                        }
                        if (strcmp(asmb.command, "POP") == 0)
                        {
                            pop(&asmb);
                        }
                        break;
                    case 'M':
                        if (strcmp(asmb.command, "MUL") == 0)
                        {
                            asmb.byte_code[asmb.ip++] = MUL;
                        }
                        break;
                    case 'D':
                        if (strcmp(asmb.command, "DIV") == 0)
                        {
                            asmb.byte_code[asmb.ip++] = DIV;
                        }
                        break;
                    case 'H':
                        if (strcmp(asmb.command, "HALT") == 0)
                        {
                            asmb.byte_code[asmb.ip++] = HALT;
                        }
                        break;
                    case 'R':
                        if (strcmp(asmb.command, "RET") == 0)
                        {
                            asmb.byte_code[asmb.ip++] = RET;
                        }
                        break;
                    case 'C':
                    {
                        if (strcmp(asmb.command, "CALL") == 0)
                        {
                            asmb.byte_code[asmb.ip++] = CALL;
                            if (k > 0)
                            {
                                get_label(&asmb);
                            }
                            else
                                asmb.ip += sizeof(stackelem_t);
                        }
                        break;
                    }
                    case 'A':
                        if (strcmp(asmb.command, "ADD") == 0)
                            {
                                asmb.byte_code[asmb.ip++] = ADD;
                            }
                        break;
                    case 'S':
                        if (strcmp(asmb.command, "SUB") == 0)
                            asmb.byte_code[asmb.ip++] = SUB;
                        break;
                    case 'J':
                    {
                        if      (strcmp(asmb.command, "JUMP") == 0)
                            asmb.byte_code[asmb.ip++] = JUMP;
                        else if (strcmp(asmb.command, "JAE")  == 0)
                            asmb.byte_code[asmb.ip++] = JAE;
                        else if (strcmp(asmb.command, "JA")   == 0)
                            asmb.byte_code[asmb.ip++] = JA;
                        else if (strcmp(asmb.command, "JBE")  == 0)
                            asmb.byte_code[asmb.ip++] = JBE;
                        else if (strcmp(asmb.command, "JB")   == 0)
                            asmb.byte_code[asmb.ip++] = JB;
                        else if (strcmp(asmb.command, "JE")   == 0)
                            asmb.byte_code[asmb.ip++] = JE;
                        else if (strcmp(asmb.command, "JNE")  == 0)
                            asmb.byte_code[asmb.ip++] = JNE;
                        else
                            break;

                        if (k > 0)
                        {
                            get_label(&asmb);
                        }
                        else
                            asmb.ip += sizeof(stackelem_t); //SPACE FOR INT LABEL
                        break;
                    }
                    case 'O':
                        if (strcmp(asmb.command, "OUT") == 0)
                            asmb.byte_code[asmb.ip++] = OUT;
                        break;
                    case 'I':
                        if (strcmp(asmb.command, "IN") == 0)
                            asmb.byte_code[asmb.ip++] = IN;
                        break;
                    default:
                    //for (size_t i = 0; i < asmb.ip; i++) printf("%d ", asmb.byte_code[i]);
                        fwrite(asmb.byte_code, sizeof(byte), asmb.ip, output_file);
                        free(asmb.byte_code);
                        free(asmb.lines);
                        free(asmb.source);
                        fclose(output_file);
                        printf("ERROR: unknown command %s\n", asmb.command);
                        return 0;
                        break;
                }
            }
        }
    }
//for (size_t i = 0; i < asmb.ip; i++) printf("%d ", asmb.byte_code[i]);
    fwrite(asmb.byte_code, sizeof(byte), asmb.ip, output_file);
    free(asmb.byte_code);
    free(asmb.lines);
    free(asmb.source);
    //fclose(output_file);
    return 0;
}

ssize_t file_size( const char* filename )
{
    struct stat input_file_stat = {};

    if (stat(filename, &input_file_stat) == -1)
        return -1;

    return input_file_stat.st_size;
}

size_t input_text_from_file( const char *file_name, char **buffer )
{
    FILE *input_file = fopen(file_name, "r");
    if (input_file == NULL)
    {
        printf("ERROR: cant open file");
        return 0;
    }

    const ssize_t FILE_SIZE = file_size(file_name);
    if (FILE_SIZE < 0)
        return 0;

    *buffer = (char*)calloc((size_t)FILE_SIZE + 1, sizeof(char));
    if (*buffer == NULL)
        return 0;

    size_t symbcnt = fread(*buffer, sizeof(char), (size_t)FILE_SIZE, input_file);
    (*buffer)[symbcnt - 1] = '\0';

    if (fclose(input_file) == -1)
        fprintf(stderr, "can't close input file\n");

    return symbcnt;
}

size_t str_amount( const char* text, size_t symbcnt )
{
    size_t stringcnt = 1;
    for (size_t i = 0; i < symbcnt; i++)
    {
        if (text[i] == '\n') stringcnt++;
        if (text[i] == '\0') break;
    }

    return stringcnt;
}

void divide_text( char* text, size_t symbcnt, line *lines, size_t string_cnt )
{
    size_t i = 0;
    lines[0].index = &text[0];
    size_t j = 0;
    for (; i < symbcnt; i++)
    {
        if (text[i] == '\n' && j < string_cnt - 1)
        {
            if (i < symbcnt - 1 && text[i + 1] == '\n')
                continue;
            j++;
            text[i] = '\0';
            lines[j].index = &text[i + 1];
            lines[j - 1].len = (size_t)(lines[j].index - lines[j - 1].index);
            lines[j].len = (size_t)(&text[symbcnt - 1] - lines[j].index);
        }
    }

    return;
}


void assembler_init( assembler_t *asmb )
{
    FILE *source_file = fopen(source_file_name, "r");
    if (source_file == NULL)
    {
        printf("source file error\n");
        return;
    }

    asmb->source = NULL;
    size_t symbcnt = input_text_from_file(source_file_name, &asmb->source);

    asmb->string_cnt = str_amount(asmb->source, symbcnt);

    asmb->lines = (line*)calloc(asmb->string_cnt, sizeof(line));

    if (asmb->lines == NULL)
    {
        fprintf(stderr, "cant allocate memory\n");
        return;
    }
    divide_text(asmb->source, symbcnt, asmb->lines, asmb->string_cnt);

    ssize_t source_size = file_size(source_file_name);
    asmb->byte_code = (byte*)calloc((size_t)source_size, sizeof(byte));

    asmb->ip = 0;
    asmb->value = 0;
    for (int i = 0; i < 8; i++) asmb->command[i] = 0;
    for (int i = 0; i < LABELS_CNT; i++) *(asmb->labels.Labels + i) = 0;

    fclose(source_file);

    return;
}
//TODO
void place_label( assembler_t *asmb )
{//printf("{PISUN}\n");
    char label[MAX_LABEL_LEN] = {};
    sscanf(asmb->lines[asmb->line_number].index, "%s", label);

    asmb->labels.Keys[asmb->labels.labels_cnt] = djb2(label + 1, MAX_LABEL_LEN - 1);
    asmb->labels.Labels[asmb->labels.labels_cnt++] = (int)asmb->ip;

//printf("label %s %d\n\n", label, (int)asmb->ip);

    return;
}

void get_label( assembler_t *asmb )
{
    char label[MAX_LABEL_LEN] = {};
    sscanf(asmb->lines[asmb->line_number].index, "%s %s", asmb->command, label);

    if (label[0] == ':')
    {//printf("start binary\n");
        int n = binary_search_label(asmb, djb2(label + 1, MAX_LABEL_LEN - 1));
//for (int i = 0; i < asmb->labels.labels_cnt; i++) if (djb2(label + 1, MAX_LABEL_LEN - 1) == asmb->labels.Keys[i]) put_value(asmb->labels.Labels[i], asmb->byte_code + asmb->ip);

//printf("start binary\n");
        put_value(asmb->labels.Labels[n], asmb->byte_code + asmb->ip);
        asmb->ip += sizeof(stackelem_t);
    }
    else
    {
        int i = -1;
        do{
            i++;
            sscanf(asmb->lines[asmb->line_number].index + i, "%c", &asmb->reg);
        } while (!isdigit(asmb->reg));

        put_value(atoi(asmb->lines[asmb->line_number].index + i), asmb->byte_code + asmb->ip);
        asmb->ip += sizeof(stackelem_t);
    }
    return;
}

int binary_search_label( assembler_t *asmb, hash_t key )
{
    int left = 0;
    int right = asmb->labels.labels_cnt;
//printf("binary search for hash %lu from %d\n", key, asmb->labels.labels_cnt);
    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (asmb->labels.Keys[mid] == key)
        {
        //printf("binary found for hash %lu\n\n", asmb->labels.Keys[mid]);
            return mid;
        }
        else if (asmb->labels.Keys[mid] < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;
}

void push( assembler_t *asmb )
{
    if (sscanf(asmb->lines[asmb->line_number].index, "%*s [%s + " stack_specifier"]", asmb->command, &asmb->value) == 2 ||
        sscanf(asmb->lines[asmb->line_number].index, "%*s [" stack_specifier" + %s]", &asmb->value, asmb->command) == 2)
    {
        asmb->byte_code[asmb->ip++] = PUSH | KONST | RX | RAM;
        put_value(asmb->value, asmb->byte_code + asmb->ip);
        asmb->ip += sizeof(stackelem_t);
        if (isdigit(asmb->command[1])) asmb->byte_code[asmb->ip++] = (byte)atoi(asmb->command + 1);
        else                           asmb->byte_code[asmb->ip++] = (byte)(asmb->command[0] - 'A');
    }
    else if (sscanf(asmb->lines[asmb->line_number].index, "%*s %s + " stack_specifier, asmb->command, &asmb->value) == 2 ||
             sscanf(asmb->lines[asmb->line_number].index, "%*s" stack_specifier" + %s", &asmb->value, asmb->command) == 2)
    {
        asmb->byte_code[asmb->ip++] = PUSH | KONST | RX;
        put_value(asmb->value, asmb->byte_code + asmb->ip);
        asmb->ip += sizeof(stackelem_t);
        if (isdigit(asmb->command[1])) asmb->byte_code[asmb->ip++] = (byte)atoi(asmb->command + 1);
        else                           asmb->byte_code[asmb->ip++] = (byte)(asmb->command[0] - 'A');

    }
    else if (sscanf(asmb->lines[asmb->line_number].index, "%*s" stack_specifier, &asmb->value) > 0)
    {
        asmb->byte_code[asmb->ip++] = PUSH | KONST;
        put_value(asmb->value, asmb->byte_code + asmb->ip);
        asmb->ip += sizeof(stackelem_t);
    }
    else if (sscanf(asmb->lines[asmb->line_number].index, "%*s [" stack_specifier"]", &asmb->value) > 0)
    {
        asmb->byte_code[asmb->ip++] = PUSH | RAM | KONST;
        put_value(asmb->value, asmb->byte_code + asmb->ip);
        asmb->ip += sizeof(stackelem_t);
    }
    else if (sscanf(asmb->lines[asmb->line_number].index, "%*s [%s]", asmb->command) > 0)
    {
        asmb->byte_code[asmb->ip++] = PUSH | RAM | RX;
        if (isdigit(asmb->command[1])) asmb->byte_code[asmb->ip++] = (byte)atoi(asmb->command + 1);
        else                           asmb->byte_code[asmb->ip++] = (byte)(asmb->command[0] - 'A');

    }
    else if (sscanf(asmb->lines[asmb->line_number].index, "%*s %s", asmb->command) > 0)
    {
        asmb->byte_code[asmb->ip++] = PUSH | RX;
        if (isdigit(asmb->command[1])) asmb->byte_code[asmb->ip++] = (byte)atoi(asmb->command + 1);
        else                           asmb->byte_code[asmb->ip++] = (byte)(asmb->command[0] - 'A');

    }



    return;
}

void pop( assembler_t *asmb )
{
//printf("pop to %s \n", asmb->lines[asmb->line_number].index + 3);
    if (sscanf(asmb->lines[asmb->line_number].index, "%*s [%s + " stack_specifier"]", asmb->command, &asmb->value) == 2 ||
        sscanf(asmb->lines[asmb->line_number].index, "%*s [" stack_specifier" + %s]",  &asmb->value, asmb->command) == 2)
    {
        asmb->byte_code[asmb->ip++] = POP | RAM | RX | KONST;
        put_value(asmb->value, asmb->byte_code + asmb->ip);
        asmb->ip += sizeof(stackelem_t);
        if (isdigit(asmb->command[1])) asmb->byte_code[asmb->ip++] = (byte)atoi(asmb->command + 1);
        else                           asmb->byte_code[asmb->ip++] = (byte)(asmb->command[0] - 'A');
    }
    else if (sscanf(asmb->lines[asmb->line_number].index, "%*s [" stack_specifier"]", &asmb->value) > 0)
    {
        asmb->byte_code[asmb->ip++] = POP | RAM | KONST;
        put_value(asmb->value, asmb->byte_code + asmb->ip);
        asmb->ip += sizeof(stackelem_t);
    }
    else if (sscanf(asmb->lines[asmb->line_number].index, "%*s [%s]", asmb->command) > 0)
    {
        asmb->byte_code[asmb->ip++] = POP | RAM | RX;
        if (isdigit(asmb->command[1])) asmb->byte_code[asmb->ip++] = (byte)atoi(asmb->command + 1);
        else                           asmb->byte_code[asmb->ip++] = (byte)(asmb->command[0] - 'A');
//printf("%zu assembling pop [%s], result %d %d\n", asmb->ip, asmb->command, asmb->byte_code[asmb->ip - 2], asmb->byte_code[asmb->ip - 1]);
    }
    else if (sscanf(asmb->lines[asmb->line_number].index, "%*s %s", asmb->command) > 0)
    {
        asmb->byte_code[asmb->ip++] = POP | RX;
        if (isdigit(asmb->command[1])) asmb->byte_code[asmb->ip++] = (byte)atoi(asmb->command + 1);
        else                           asmb->byte_code[asmb->ip++] = (byte)(asmb->command[0] - 'A');
//printf("%zu assembling pop %s, result %d %d\n", asmb->ip, asmb->command, asmb->byte_code[asmb->ip - 2], asmb->byte_code[asmb->ip - 1]);
    }

    return;
}

hash_t djb2( void *ptr, size_t size )
{
    hash_t hash = 0xEDA;
    for (size_t i = 0; i < size; i++)
    {
        hash = ((hash << 5) + hash) + *((unsigned char*)ptr + i);
    }

    return hash;
}

void put_value( stackelem_t value, byte *ptr )
{
    memcpy(ptr, &value, sizeof(stackelem_t));

    return;
}

