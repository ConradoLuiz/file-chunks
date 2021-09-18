#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char*   create_output_folder    (int argc, char *argv[]);
int     create_chunks           (FILE* file, char* output_folder, long unsigned int chunk_size, long unsigned int filesize);
void    create_log              (char* output_folder, int chunks, char* original_name);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Make sure to run like:\n\tchunker.exe <path> <filename> <output-dir>");
        return 1;
    }

    // ----------------------------------------------------------
    // ----------------------------------------------------------
    // ----------------------------------------------------------
    long unsigned int chunk_size = 24 * 1024 * 1024;
    char *filepath = argv[1];
    char *filename = argv[2];
    
    printf("Filepath:\t%s%s\n", filepath, filename);

    char* aux = (char*) malloc (sizeof(filepath) + sizeof(filename));
    sprintf(aux, "%s%s", filepath, filename);
    
    FILE* original_file = fopen(aux, "rb");
    free(aux);

    // ----------------------------------------------------------
    // ----------------------------------------------------------
    // ----------------------------------------------------------

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

    int chunks = create_chunks(original_file, output_folder, chunk_size, len);

    create_log(output_folder, chunks, filename);

    printf("Chunks:\t\t%d", chunks);

    // ----------------------------------------------------------
    // ----------------------------------------------------------
    // ----------------------------------------------------------
    fclose(original_file);

    return 0;
}

int create_chunks(FILE* file, char* output_folder, long unsigned int chunk_size, long unsigned int filesize){
    int chunk_id = 0;
    char chunk_name[10000];
    char* buffer = (char *) malloc(chunk_size + 1);
    size_t can_read = 1;
    long unsigned int _ftell;

    while(fread(buffer, chunk_size, 1, file)) {

        sprintf(chunk_name, "%s/c_%d.fc", output_folder, chunk_id);
        chunk_id++;

        FILE* chunk_file = fopen(chunk_name, "wb");

        fwrite(buffer, 1, chunk_size, chunk_file);

        fclose(chunk_file);

        _ftell = ftell(file);
    }   

    sprintf(chunk_name, "%s/c_%d.fc", output_folder, chunk_id);
    chunk_id++;
    FILE* chunk_file = fopen(chunk_name, "wb");

    fwrite(buffer, 1, filesize - _ftell, chunk_file);

    fclose(chunk_file);

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
    fwrite(aux, 1, strlen(aux), file);

    fclose(file);
}

char* create_output_folder(int argc, char *argv[]) {
    char *output_folder;

    if (argc > 2) {
        output_folder = argv[3];
    }
    else {
        output_folder = "output";
    }

    char aux[1000];
    sprintf(aux, "mkdir %s", output_folder);
    system(aux);

    printf("Output:\t\t%s\n", output_folder);

    return output_folder;
}