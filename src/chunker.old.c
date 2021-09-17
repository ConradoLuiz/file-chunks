#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char*   create_output_folder    (int argc, char *argv[]);
int     create_chunks           (FILE* file, char* output_folder, long unsigned int chunk_size);
void    create_log              (char* output_folder, int chunks, char* original_name);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Make sure to run like:\n\tchunker.exe <path/to/filename>");
        return 1;
    }

    // ----------------------------------------------------------
    // ----------------------------------------------------------
    // ----------------------------------------------------------
    long unsigned int chunk_size = 25 * 1024 * 1024;
    char *filename = argv[1];
    
    printf("Filename:\t%s\n", filename);

    FILE* original_file = fopen(filename, "rb");

    fseek(original_file, 0, SEEK_END);
    long int len = ftell(original_file);

    if (len <= chunk_size){ // lenght of file is less than 25mb
        printf("File is already less than 25mb");
        return 1;
    }
    fseek(original_file, 0, SEEK_SET);

    printf("Length:\t\t%d kb\n", len / 1024 );

    // ----------------------------------------------------------
    // ----------------------------------------------------------
    // ----------------------------------------------------------
    
    char* output_folder = create_output_folder(argc, argv);

    // ----------------------------------------------------------
    // ----------------------------------------------------------
    // ----------------------------------------------------------

    int chunks = create_chunks(original_file, output_folder, chunk_size);

    create_log(output_folder, chunks, filename);

    printf("Chunks:\t\t%d", chunks);

    // ----------------------------------------------------------
    // ----------------------------------------------------------
    // ----------------------------------------------------------
    fclose(original_file);

    return 0;
}

int create_chunks(FILE* file, char* output_folder, long unsigned int chunk_size){
    int chunk_id = 0;
    char chunk_name[10000];
    char* buffer = (char *) malloc(chunk_size + 1);

    while(fread(buffer, chunk_size, 1, file)) {
        sprintf(chunk_name, "%s/c_%d.fc", output_folder, chunk_id);
        chunk_id++;

        FILE* chunk_file = fopen(chunk_name, "w");

        fwrite(buffer, 1, chunk_size, chunk_file);

        fclose(chunk_file);
    }   
    free(buffer);

    return chunk_id;
}

void create_log(char* output_folder, int chunks, char* original_name){
    char aux[1000];
    sprintf(aux, "%s/fc.log", output_folder);
    FILE* file = fopen(aux, "w");

    fwrite(original_name, 1, strlen(original_name), file);
    fwrite("\n", 1, 1, file);

    sprintf(aux, "%d", chunks);
    fwrite(aux, 1, 1, file);

    fclose(file);
}

char* create_output_folder(int argc, char *argv[]) {
    char *output_folder;

    if (argc > 2) {
        output_folder = argv[2];
    }
    else {
        output_folder = "output";
    }

    char aux[1000];
    sprintf(aux, "mkdir %s", output_folder);
    system(aux);

    return output_folder;
}