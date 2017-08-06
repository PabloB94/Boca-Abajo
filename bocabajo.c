#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sysexits.h>
#include <stdarg.h>
#include <unistd.h>


int main(int argc, const char * argv[]) {
    int i, j, numFiles = argc - 1;
    long k;
    FILE** files = malloc(sizeof(FILE*) * (numFiles));

    size_t maxLineLenght = 2048;
    FILE* currentFile;
    long maxPossibleNumberOfLines = 0;
    char** fileLines = NULL;

    if(argc == 2 && ((strncmp(argv[1], "-h", sizeof(char) * 2) == 0) || (strncmp(argv[1], "--help", sizeof(char) * 6) == 0))) {
        printf("bocabajo: Uso: bocabajo [ fichero... ]\nbocabajo: Invierte el orden de las lı́neas de los ficheros (o de la entrada).\n");
        exit(EX_OK);
    }

    //Check if files exist and are readable
    for (i=1; i<argc; i++){
  		int exists, is_readable;
  		const char *filename = argv[i];
  		exists = access (filename, F_OK);
  		is_readable = access (filename, R_OK);
  		if ((exists != 0) || (is_readable != 0)){
        fprintf(stderr,"bocabajo: Error(EX_NOINPUT), uso incorrecto del mandato. \"Success\"\n");
        fprintf(stderr,"bocabajo+ El fichero \"%s\" no puede ser leido\n",argv[i]);
        exit(EX_NOINPUT);
      }
  	}

    // Open all files
    for (i = 1; i < argc; i++) {
        files[i-1] = fopen(argv[i], "r");
    }


    for (i = numFiles-1; i>=0; i--) {
        // Calculate file's maximum possible number of lines
        currentFile = files[i];
        fseek(currentFile, 0L, SEEK_END);
        maxPossibleNumberOfLines = ftell(currentFile) / sizeof(char);
        fseek(currentFile, 0L, SEEK_SET);

        // Read all file lines
        free(fileLines);    // Free previous allocated memory
        fileLines = malloc((sizeof(char) * 2048) * maxPossibleNumberOfLines);
        if(fileLines == NULL){
          fprintf(stderr,"%s: Error(EX_OSERR), uso incorrecto del mandato. \"Success\"\n",argv[0]);
          fprintf(stderr,"%s+ No se puede ubicar la memoria dinamica necesaria\n",argv[0]);
          exit(EX_OSERR);
        }
        j=0;
        while (getline(&fileLines[j], &maxLineLenght, currentFile) != -1) {
            j++;
        }

        // Write file lines in reverse order
        for (k = j-1; k>=0; --k) {
            printf("%s", fileLines[k]);
        }
    }
    exit(EX_OK);
}
