#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int     get_log_info    (char* folderpath, char* original_name);
void    join_chunks     (char* folderpath, FILE* output, int chunks, long unsigned int chunk_size);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Make sure to run like:\n\tdechunker.exe <path-to-folder> <output-filename>");
        return 1;
    }

    // ----------------------------------------------------------
    // ----------------------------------------------------------
    // ----------------------------------------------------------
    long unsigned int chunk_size = 25 * 1024 * 1024;
    char *folderpath = argv[1];
    char *output_name = argv[2];

    printf("Folder:\t\t%s\n", folderpath);

    char original_name[1000];

    int chunks = get_log_info(folderpath, original_name);
    if  (chunks < 0) {
        printf("fc.log file not found");
        return -1;
    }

    printf("Main original:\t%s\n", original_name);
    printf("Chunks:\t\t%d\n", chunks);

    // ----------------------------------------------------------
    // ----------------------------------------------------------
    // ----------------------------------------------------------

    FILE* output_file = fopen(original_name, "wb");

    join_chunks(folderpath, output_file, chunks, chunk_size);

    fclose(output_file);
    return 0;
}

int get_log_info(char* folderpath, char* original_name){
    char log_path[1000];
    sprintf(log_path, "%s/fc.log", folderpath);

    FILE* log = fopen(log_path, "r");
    if (log == NULL){
        return -1;
    }

    int i;
    char c;
    char buffer[200];

    for(i = 0; i < 200; i++){
        c = fgetc(log);
        if (c == '\n') { break; }

        buffer[i] = c;
    }
    strcpy(original_name, buffer);

    for(i = 0; i < 200; i++){
        c = fgetc(log);
        if (c == '\n') { break; }

        buffer[i] = c;
    }

    fclose(log);

    return atoi(buffer);
}

void join_chunks(char* folderpath, FILE* output, int chunks, long unsigned int chunk_size){
    int i;
    char aux[10000];
    long unsigned int len;
    char* buffer = (char *) malloc(chunk_size + 1);
    
    for (i = 0; i < chunks; i++){
        sprintf(aux, "%s/c_%d.fc", folderpath, i);

        FILE* chunk_file = fopen(aux, "rb");

        fseek(chunk_file, 0, SEEK_END);
        len = ftell(chunk_file);
        fseek(chunk_file, 0, SEEK_SET);

        fread(buffer, len, 1, chunk_file);
        fwrite(buffer, 1, len, output);

        fclose(chunk_file);
    }
    free(buffer);
}