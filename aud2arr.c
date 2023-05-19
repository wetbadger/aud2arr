//Parses a wav file and prints an array of 16-bit integers to the console

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int fd;
size_t dsize;
size_t fsize;

void parse_header(char* header) {
    //RIFF
    printf("%.*s\n", 4, header);
    header+=4;
    //File size
    fsize = (size_t)*((int*)header);
    printf("File size: %ld\n", fsize);
    header+=4;
    //WAVE
    printf("%.*s\n", 4, header);
    header+=4;
    //fmt
    printf("%.*s\n", 4, header);
    header+=4;
    //Length of format data
    printf("Len of fmt: %d\n", *((int*)header));
    header+=4;
    //Type of format (1 is PCM) – 2 byte integer
    printf("Type of format: %d\n", *((short*)header));
    header+=2;
    //Number of Channels
    printf("Number of channels: %d\n", *((short*)header));
    header+=2;
    //Sample Rate
    printf("Sample rate: %d\n", *((int*)header));
    header+=10;
    //Bits per sample
    printf("Bits per sample: %d\n", *((short*)header));
    header+=2;
    //data
    printf("%.*s\n", 4, header);
    header+=4;
    //File size
    dsize = (size_t)*((unsigned int*)header);
    printf("%ld\n", dsize);

    
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Error: No file specified.\n");
        return EXIT_FAILURE;
    }
    int opt;
    char compress = 0;
    while ((opt = getopt(argc, argv, "c")) != -1) {
        switch (opt) {
            case 'c':
                //compress the array to 8-bit samples
                compress = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-c] [file...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (compress) {
        fd = open(argv[2], O_RDONLY);
    } else {
        fd = open(argv[1], O_RDONLY);
    }
    char* header = malloc(44);
    read(fd, header, 44);
    parse_header(header);
    free(header);
    short *arr = calloc(2, dsize);
    read(fd, arr, dsize);
    char * comma = "";
    for (int i = 0; i < dsize/2; i++) {
        short sample = (short)arr[i];
        if (compress) {
            sample = sample / 128;
        }
        printf("%s%d", comma, sample);
        comma = ", ";
    }
    printf("\n");
    free(arr);
    close(fd);
    return EXIT_SUCCESS;
}