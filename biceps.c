#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <readline/readline.h>

static char **Mots;
static int NMots;

char *copyString(char *s)
{
    char *copie;
    size_t taille;

    taille = strlen(s) + 1;
    copie = malloc(taille);
    if (copie == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    strcpy(copie, s);
    return copie;
}

void libereAnalyse(void)
{
    int i;

    for (i = 0; i < NMots; i++) {
        free(Mots[i]);
    }

    free(Mots);
    Mots = NULL;
    NMots = 0;
}

int analyseCom(char *b)
{
    char *travail;
    char *courant;
    char *mot;

    libereAnalyse();

    travail = copyString(b);
    courant = travail;

    mot = strsep(&courant, " \t\n");

    if (mot == NULL) {
        free(travail);
        return 0;
    }

    if (*mot == '\0') {
        free(travail);
        return 0;
    }

    Mots = malloc(sizeof(char *));
    if (Mots == NULL) {
        perror("malloc");
        free(travail);
        exit(EXIT_FAILURE);
    }

    Mots[0] = copyString(mot);
    NMots = 1;

    free(travail);
    return NMots;
}

char *fabrique_prompt(void)
{
    char *user;
    char hostname[HOST_NAME_MAX + 1];
    char *prompt;
    char fin_prompt;
    size_t taille;

    user = getenv("USER");
    if (user == NULL) {
        user = "user";
    }

    if (gethostname(hostname, sizeof(hostname)) != 0) {
        strcpy(hostname, "machine");
    }
    hostname[sizeof(hostname) - 1] = '\0';

    if (geteuid() == 0) {
        fin_prompt = '#';
    } else {
        fin_prompt = '$';
    }

    taille = strlen(user) + strlen(hostname) + 4;
    prompt = malloc(taille);
    if (prompt == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    snprintf(prompt, taille, "%s@%s%c ", user, hostname, fin_prompt);
    return prompt;
}

int main(void)
{
    char *ligne;
    char *prompt;

    Mots = NULL;
    NMots = 0;

    while (1) {
        prompt = fabrique_prompt();
        ligne = readline(prompt);
        free(prompt);

        if (ligne == NULL) {
            putchar('\n');
            break;
        }

        if (analyseCom(ligne)) {
            printf("Commande : %s\n", Mots[0]);
        }

        free(ligne);
    }

    libereAnalyse();
    return 0;
}