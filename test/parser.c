#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <kpmalloc.h>

#include "dat.h"
#include "fns.h"

int
parseline(const char *prompt)
{
    char *rawsyntax;
    char *cmd, *initialvarname, *sizestr;

    char *varname;

    char *endptr;
    long int rawsize;
    struct var *thisvar;

    do{
        rawsyntax = readline(prompt);
    }while(strlen(rawsyntax) == 0);

    while(isspace(*rawsyntax)) rawsyntax++;
    cmd = strtok(rawsyntax, DELIMITER);
    if(!cmd) goto badline;

    if(strcmp(cmd, MALLOC_CMD) == 0){
        initialvarname = strtok(NULL, DELIMITER);
        if(!initialvarname) goto badline;
        varname = (char *)malloc((strlen(initialvarname)+1) * sizeof(char));
        if(!varname){
            fprintf(stderr, "\tError: couldn't translate over varname");
            return -1;
        }

        strcpy(varname, initialvarname);
        fprintf(stderr, "configuring for %s..\n", varname);

        sizestr = strtok(NULL, DELIMITER);
        if(!sizestr) goto badline;
        rawsize = strtol(sizestr, &endptr, 10);
        if(rawsize < 1 || rawsize > __INT_MAX__) goto badline;
        if(*sizestr == '\0' || *endptr != '\0') goto badline;

        thisvar = allocvar(varname, (unsigned int)rawsize);
        free(rawsyntax);
        if(thisvar) return 0;
        
        fprintf(stderr, "\tError: failed to set up newvar...\n");
        return -1;

    }
    else if(strcmp(cmd, FREE_CMD) == 0){
        initialvarname = strtok(NULL, DELIMITER);
        if(!initialvarname) goto badline;
        thisvar = findvar(initialvarname);
        if(!thisvar){
            fprintf(stderr, "\tError: couldn't find this var in varlist.\n");
            return -1;
        }

        freevar(thisvar);
        return 0;
    }
    else if(strcmp(cmd, STATUS_CMD) == 0) return printvarlist();
    else if(strcmp(cmd, EXIT_CMD) == 0) exit(0);
    else goto badline;

badline:
    fprintf(stderr, "\tError: invalid command\n");
    return -1;
}