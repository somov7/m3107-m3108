#include <stdio.h>
#include <string.h>
#define Count_args 4
#define Max_filename_size 128

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
    int i;
    int temp;

    FILE* arcfile = fopen(arcname, "wb");
    FILE* file;
    unsigned long long int file_info[Max_filename_size]; //size and name
        for (i = Count_args; i < filecount; i++){
        file = fopen(files[i], "rb");
        if (file == NULL)
            continue;
        fseek(file, 0, SEEK_END);
            file_info[i - Count_args] = ftell(file);
        fseek(file, 0, SEEK_SET);
        fclose(file);
    }
    for (i = 0; i < filecount - Count_args; i++)
        fprintf(arcfile, "< %s : %llu >", files[i + Count_args], file_info[i]);
    fprintf(arcfile, "\n");
    for (i = Count_args; i < filecount; i++){
        file = fopen(files[i], "rb");
        if (file == NULL){
            printf("\tlox, try again\n");
            continue;
        }
        else{
            printf("\tAdd Done!\n");
        }
        while ((temp = getc(file)) != EOF)
            putc(temp, arcfile);
        fclose(file);
    }
    printf("\t\n  Zip done!  \n");
}

//============================================== files_in_arc
void List(char* arcname){
    int count = 1;
    FILE* file = fopen(arcname, "rb");
    unsigned long long int  filesize;
    char name_of_file [Max_filename_size] = {0};
    while (fscanf(file, "< %s : %llu >", name_of_file, &filesize) != 0) {
        printf("\tFile %d:\n", count);
        printf("\t\t%s\n", name_of_file);
        count ++;
    }
        printf("\t\n%d Files in archive!\n", count - 1);
        printf("\t\nDone!\n");
    fclose(file);
}
//============================================== extract_files_in_direct
void Extract(char* arcname){
    FILE* arch = fopen(arcname, "rb+wb");
    unsigned long long int  now_pos = 0;
    unsigned long long int  start_pos = 0;
    int c;
    while ((c = getc(arch)) != EOF) {
        start_pos++;
        if (c == '\n')
            break;
    }
    fseek(arch, 0, SEEK_SET);
    char name_of_file[Max_filename_size] = {0};
    unsigned long long int  size_of_file;
    FILE *file;
    while (fscanf(arch, "< %s : %llu >", name_of_file, &size_of_file) != 0) {
        file = fopen(name_of_file, "wb");
        if (file == NULL){
            printf("You are using a shitty file");
            break;
        }
        now_pos = ftell(arch);
        fseek(arch, start_pos, SEEK_SET);
        start_pos += size_of_file;
        while (size_of_file-- > 0)
            putc((c = getc(arch)), file);
        fseek(arch, now_pos, SEEK_SET);
        fclose(file);
    }
    printf("\t\n Unzip done! \n");
}
//============================================= main_program(parse_arg)
int main(int argc, char *argv[]) {

    char *arc_file_name;

    for (int i = 0; i < argc; i++) { //parse input

        if (!strcmp("--file", argv[i]))
            arc_file_name = argv[i + 1];

        if (!strcmp("--create", argv[i])) {
            Create(arc_file_name, argc, argv);
            break;
        }

        if (!strcmp("--extract", argv[i]))
            Extract(arc_file_name);

        if (!strcmp("--info", argv[i]))
            Commands_Info();

        if (!strcmp("--list", argv[i]))
            List(arc_file_name);
    }

    return 0;
}
