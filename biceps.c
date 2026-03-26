#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <readline/readline.h>

static char **Mots;
static int NMots;

#define NBMAXC 10

typedef int (*TypeCommande)(int, char **);

typedef struct {
    char *nom;
    TypeCommande fonction;
} CommandeInterne;

static CommandeInterne TabComInt[NBMAXC];
static int NbComInt;

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
    char **nouveau;

    libereAnalyse();

    travail = copyString(b);
    courant = travail;

    while ((mot = strsep(&courant, " \t\n")) != NULL) {
        if (*mot == '\0') {
            continue;
        }

        nouveau = realloc(Mots, (NMots + 2) * sizeof(char *));
        if (nouveau == NULL) {
            perror("realloc");
            free(travail);
            exit(EXIT_FAILURE);
        }

        Mots = nouveau;
        Mots[NMots] = copyString(mot);
        NMots++;
        Mots[NMots] = NULL;
    }

    free(travail);
    return NMots;
}

void ajouteCom(char *nom, TypeCommande fonction)
{
    if (NbComInt >= NBMAXC) {
        fprintf(stderr, "Trop de commandes internes\n");
        exit(EXIT_FAILURE);
    }

    TabComInt[NbComInt].nom = nom;
    TabComInt[NbComInt].fonction = fonction;
    NbComInt++;
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
    int i;

    Mots = NULL;
    NMots = 0;
    NbComInt = 0;

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

            for (i = 1; i < NMots; i++) {
                printf("Parametre %d : %s\n", i, Mots[i]);
            }
        }

        free(ligne);
    }

    libereAnalyse();
    return 0;
}