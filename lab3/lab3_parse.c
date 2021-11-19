#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long long to_seconds(char *str);
void find_5xx_requests_cmd(void);
void find_5xx_requests_file(void);
void find_max_requests(FILE *access_log_file, long long sec_window);



int main(int argc, char** argv) {


    if(argc == 1){
        printf("Use lab3_parse.exe -h \t:)");
    }

    if(strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0){
        printf("Usage:\n");
        printf("lab3_parse.exe [-h]                         | HELP INFORMATION\n");
        printf("lab3_parse.exe [-e] [input_file]            | SHOWS 5XX STATUS CODE REQUESTS\n");
        printf("lab3_parse.exe [-ef] [input_file]           | WRITE ALL 5XX REQUESTS IN A FILE\n");
        printf("lab3_parse.exe [-w] [input_file] [time_window (sec)]     | SHOWS MAX AMOUNT OF REQUESTS IN TIME WINDOW\n");
    }
    else if(strcmp(argv[1],"-e") == 0)
        find_5xx_requests_cmd();
    
    else if(strcmp(argv[1],"-ef") == 0)
        find_5xx_requests_file();
        

    else if(strcmp(argv[1],"-w") == 0){
        FILE* access_log_file;
        access_log_file = fopen(argv[2], "r");

        int sec_window = -1;
        sec_window = atoi(argv[3]);
        if (sec_window != -1)
            find_max_requests(access_log_file, sec_window);
        
    }
    return 0;
}



void find_5xx_requests_cmd(void){
    FILE* access_log_file;
    access_log_file = fopen(_argv[2], "r");

    if(access_log_file == NULL){
		printf("Cannot open the file :c\n");
        exit(EXIT_FAILURE);
	}

    

    char remote_addr[507]; // 253*2byte + 1; 253 - max domain length (including dots etc)
    char local_time[27]; // time consisnts of 27 chars
    char request[2048]; 
    int status;
    char bytes_send[8];

    char c = fgetc(access_log_file);
    while(c != EOF)
        c = fgetc(access_log_file);

    
    rewind(access_log_file);

    int num = 0;
    int amount_of_requests = 0;

    
    while(!feof(access_log_file)){
        fscanf(access_log_file, "%s - - [%[^]]] \"%[^\"]\" %d %d", remote_addr, local_time, request, &status, &bytes_send);    
        if (status >= 500) {
            num++;
            printf("%d %s\n", num, request);
            amount_of_requests += 1;
        }
    }

    printf("\n[+] Total amount of 5XX requests: %d", amount_of_requests);
}

void find_5xx_requests_file(void){

    FILE* access_log_file;
    FILE* output_file;
    access_log_file = fopen(_argv[2], "r");
    output_file = fopen("error_output.txt","w");

    if(access_log_file == NULL){
		printf("Cannot open the file :c\n");
        exit(EXIT_FAILURE);
	}

    

    char remote_addr[507]; // 253*2 + 1; 253 - max domain length (including dots etc)
    char local_time[27]; // time consisnts of 27 chars
    char request[2048];
    int status;
    char bytes_send[8];

    char c = fgetc(access_log_file);
    while(c != EOF)
        c = fgetc(access_log_file);

    rewind(access_log_file);
      

    int num = 0;
    int amount_of_requests = 0;

    
    while(!feof(access_log_file)){
        fscanf(access_log_file, "%s - - [%[^]]] \"%[^\"]\" %d %d", remote_addr, local_time, request, &status, &bytes_send);    
        if(status >= 500){
            num++;
            fprintf(output_file, "%d %s\n", num, request);
            amount_of_requests += 1;
        }
    }

    fprintf(output_file, "\n[+] Total amount of 5XX requests: %d", amount_of_requests);
    printf("Requsts was written in file error_output.txt.\n");
}


long long to_seconds(char *str) {
    const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
                                     "Aug", "Sep", "Oct", "Nov", "Dec"};

    struct tm time = {
            .tm_year = atoi(str + 7) - 1900,
            .tm_mday = atoi(str),
            .tm_hour = atoi(str + 12),
            .tm_min = atoi(str + 15),
            .tm_sec = atoi(str + 18)
    };

    str[6] = '\0';

    int month;

    for(month = 0; month < 12; month++){
        if(!strcmp(str + 3, months[month])){
            break;
        }
    }

    time.tm_mon = month;

    return mktime(&time);
}

void find_max_requests(FILE *access_log_file, long long sec_window) {
    

    long long max_requests = 0;
    long long start_time, end_time = 1;

    long long count = 0;
    long long requests = 1;

    long long time_size = 1000;

    long long* times = malloc(time_size * sizeof(long long));

    char* line = NULL;
    size_t size = 0;

    while((size = getline(&line, &size, access_log_file)) != -1){
        char* time_ptr = strtok(line, "[");

        int part = 0;

        while(time_ptr != NULL){
            part++;

            if (part == 2) {
                if (count >= time_size) {
                    times = realloc(times, sizeof(long long) * time_size * 2);
                    time_size *= 2;
                }
                times[count] = to_seconds(time_ptr);
            }

            time_ptr = strtok(NULL, "]");
        }

        if(count > 0){
            for(long long i = count - 1; i >= 0; i--) {
                if (times[count] - times[i] <= sec_window && times[count] - times[i] >= 0) {
                    requests += 1;
                } else break;
            }
        }

        if(requests > max_requests) {
            max_requests = requests;
        }

        requests = 1;
        count++;
    }

    free(times);

    printf("Window: %lld seconds\n", sec_window);
    printf("Requests: %lld \n", max_requests);

}