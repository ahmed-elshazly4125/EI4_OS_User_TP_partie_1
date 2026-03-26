#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

int main(void)
{
    char *ligne;

    while (1) {
        ligne = readline("biceps$ ");
        if (ligne == NULL) {
            putchar('\n');
            break;
        }

        printf("Commande saisie : [%s]\n", ligne);
        free(ligne);
    }

    return 0;
}