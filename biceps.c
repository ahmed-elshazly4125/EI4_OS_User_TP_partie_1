#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

char *fabrique_prompt(void)
{
    char *user;
    char *prompt;
    size_t taille;

    user = getenv("USER");
    if (user == NULL) {
        user = "user";
    }

    taille = strlen(user) + 3;
    prompt = malloc(taille);
    if (prompt == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    snprintf(prompt, taille, "%s$ ", user);
    return prompt;
}

int main(void)
{
    char *ligne;
    char *prompt;

    while (1) {
        prompt = fabrique_prompt();
        ligne = readline(prompt);
        free(prompt);

        if (ligne == NULL) {
            putchar('\n');
            break;
        }

        printf("Commande saisie : [%s]\n", ligne);
        free(ligne);
    }

    return 0;
}