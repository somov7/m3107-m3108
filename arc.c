#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define Count_args 4
#define Max_filename 256
#define count_files 1000
#define Radix 10
//==================================== program_info
void Commands_Info(){
    printf("commands:"
           "\t\narc.exe --file archive.arc --create 123.docx 555.xlsx 222.bmp - creates an archive.arc file which consists of 123.docx 555.xlsx 222.bmp\n"
           "\t\narc.exe --file archive.arc --list - prints the contents of the archive to the console archive.arc\n"
           "\t\narc.exe --file archive.arc --extract - extracts all files in archive archive.arc\n"
            );
}


//==================================== arc_create
void Create(char* arcname, int filecount, char* files[]){
    FILE *out = fopen(arcname, "wb"); 
    FILE *in;
    int file_inf[count_files];   
    fprintf(out, "%d\n", filecount - Count_args); 
    for (int i = 0; i < filecount - Count_args; i++) {
        in = fopen(files[i + Count_args], "rb"); 
        if (in != NULL) {
        fseek(in, 0, SEEK_END);
        file_inf[i] = ftell(in); 
        fseek(in, 0, SEEK_SET);
        fprintf(out, "%s - %d\n", files[i + Count_args], file_inf[i]);   
        fclose(in);
        }
    }
  for (int i = 0; i < filecount - Count_args; i++) {
        in = fopen(files[i + Count_args], "rb");
        char* temp = malloc(file_inf[i] * sizeof(char));
        fread(temp, sizeof(char), file_inf[i], in);
        fwrite(temp, sizeof(char), file_inf[i], out); 
        fclose(in);
    }
    fclose(out);
}
//============================================== files_in_arc
void List(char *file1) {
       FILE *arc_file = fopen(file1, "rb");
    char temp_file_info[count_files];
    char count[10], *trash;
    fscanf(arc_file, "%s", count);
    int count_files_in_arc = strtol(count, &trash, Radix);

    for (int i = 0; i < count_files_in_arc; i++) {
        fscanf(arc_file, "%s", temp_file_info); 
        printf("%s\n", temp_file_info); 
        fscanf(arc_file, "%s", temp_file_info);
        fscanf(arc_file, "%s", temp_file_info);
    }
    fclose(arc_file);
}
//============================================== extract_files_in_direct
void Extract(char* arcname){
  FILE *arc_file = fopen(arcname, "rb");

    char count[10], *trash;
    fscanf(arc_file, "%s", count);
    int count_files_in_arc = strtol(count, &trash, Radix); 
    int file_start_pos = ftell(arc_file);

    char nameof_file[Max_filename];
    char scrumble[count_files], temp_info[count_files];
    int cur_pos = 0;

    int sizeof_file, bytes_from_start = 0;
    int each_flile_start = ftell(arc_file);

    fseek(arc_file, file_start_pos, SEEK_SET);

    for (int i = 0; i < count_files_in_arc; i++) {
        fscanf(arc_file, "%s%s%d", nameof_file, scrumble, &sizeof_file); 

        bytes_from_start = ftell(arc_file);

        FILE *new_file = fopen(nameof_file, "wb"); 

        fseek(arc_file, each_flile_start, SEEK_SET); 

        char* temp_info = malloc(sizeof_file * sizeof(char*));
        fread(temp_info, sizeof(char), sizeof_file, arc_file);
        fwrite(temp_info, sizeof(char), sizeof_file, new_file); 

        each_flile_start = each_flile_start + sizeof_file; 

        fseek(arc_file, bytes_from_start, SEEK_SET); 

        fclose(new_file);
    }

    fclose(arc_file);
}


//============================================= main_program(parse_arg)
int main(int argc, char *argv[]) {

    char *arc_name;

    for (int i = 0; i < argc; i++) { 
        
        if (!strcmp("--file", argv[i]))
            arc_name = argv[i + 1];

        if (!strcmp("--create", argv[i])) {
            Create(arc_name, argc, argv);
            break;
        }

        if (!strcmp("--extract", argv[i]))
            Extract(arc_name);

        if (!strcmp("--info", argv[i]))
            Commands_Info();

        if (!strcmp("--list", argv[i]))
            List(arc_name);
    }

    return 0;
}