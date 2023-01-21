#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

/*
 * Definisco TRUE e FALSE per una maggiore facilità nel
 * creare espressioni booleane e per un maggiore leggibilità
 */
#define TRUE 1
#define FALSE 0

/*
 * Definisco MAX e MIN sempre per un maggiore leggibilità
 * del codice. Potrei anche scrivere direttamente due funzioni
 * per svolgere questo compito ma credo che svilupparlo così
 * lo renda più leggibile e compatto
 */
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// Definisco la struttura dati stat per evitare errori di ridondanza
typedef struct stat Stat;

// Funzione che wrappa strcmp per far ritornare un booleano completo
int strcmp_bool(char *string1, char *string2);

// Funzione che apre i file in modo sicuro gestendo gli errori
FILE *open_file(char *file);

// Funzione che controlla se un stringa inizia con un determinato carattere
int starts_with_char(char *string, char character);

// Funzione per verificare se l'input preso è corretto e senza mancanze
FILE *validation(int argc, char *argv[], int args[]);

// Funzione per calcolare il numero di righe in un file
int count_rows(FILE *file);

// Funzione che controlla se i due file sono vuoti
void check_empty(char *namefile1, char *namefile2, int args[], FILE *output);

// Bufferizzo le N righe oppure le righe fino all'EOF
void buffer_n_rows(char *rows[], int max_lines, FILE *file);

// Funzione che si occupa di gestire tutte le subroutine passate in input per rilevare le differenze tra due file
void exec_subroutine(int args[], FILE *infile1, FILE *infile2, char *namefile1, char *namefile2, int max, FILE *output);

/*
 * Funzione che quando F1 e F2 sono diversi in almeno una riga
 * restituisce la stringa ”F1 ed F2 sono differenti"
 */
int subroutine_g(int comparison, char *namefile1, char *namefile2, FILE *output);

/*
 * Funzione che quando F1 e F2 sono uguali in tutte le righe
 * restituisce la stringa “F1 ed F2 sono uguali”
 */
int subroutine_s(int comparison);

// Funzione che restituisce i numeri delle righe differenti
void
subroutine_d(int comparison, char *string1, char *string2, char *namefile1, char *namefile2, int line, int specified,
             FILE *output);

// Funzione che restituisce i numeri delle righe uguali
void
subroutine_u(int comparison, char *string1, char *string2, char *namefile1, char *namefile2, int line, int specified,
             FILE *output);

int main(int argc, char *argv[]) {
    FILE *infile1, *infile2, *output;

    /*
     * Array che include gli argomenti nelle seguenti posizioni
     * 0: -g; 1: -s; 2: -d; 3: -u; 4: -v; 5: -o;
     */
    int args[] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

    // Check sugli argomenti passati
    output = validation(argc, argv, args);

    // Apertura in lettura dei due file
    infile1 = open_file(argv[1]);
    infile2 = open_file(argv[2]);

    // Check nel caso ci sia un file vuoto o entrambi
    check_empty(argv[1], argv[2], args, output);

    // Trovo il file più corto
    int lenfile1, lenfile2, maxLines;
    lenfile1 = count_rows(infile1);
    lenfile2 = count_rows(infile2);

    maxLines = MIN(lenfile1, lenfile2);

    // Riporto il cursore del file all'inizio
    rewind(infile1);
    rewind(infile2);

    // Eseguo tutte le opzioni passate in input
    exec_subroutine(args, infile1, infile2, argv[1], argv[2], maxLines, output);

    //Chiusura di entrambi i file
    fclose(infile1);
    fclose(infile2);

    return 0;
}

// Funzione che wrappa strcmp per far ritornare un booleano
int strcmp_bool(char *string1, char *string2) {

    // Se le due stringhe sono uguali restituisce TRUE, altrimenti FALSE
    if (strcmp(string1, string2) == 0)
        return TRUE;

    return FALSE;
}

// Funzione che apre i file in modo safe
FILE *open_file(char *file) {

    // Apro il file in lettura
    FILE *input = fopen(file, "r");

    /*
     * Se input è positivo tutto è andato a buon fine
     * e restituisco il riferimento al file
     */
    if (input)
        return input;

    // Altrimenti segnalo l'errore all'utente e fermo l'esecuzione del programma
    errno = ENOENT;
    perror("File non esistente");
    exit(EXIT_FAILURE);
}

// Funzione che controlla se un stringa inizia con un determinato carattere
int starts_with_char(char *string, char character) {

    // Controllo che gli argomenti siano stati passati
    if (!string || !character) {
        errno = EIO;
        perror("Gli argomenti passati sono NULL\n");
        exit(EXIT_FAILURE);
    }

    /*
     * Controllo che il primo carattere della stringa
     * si uguale al carattere passato come argomento
     */
    if (string[0] == character) {
        errno = EIO;                // Segnalo l'errore
        return TRUE;                // E ritorno TRUE
    }

    // Altrimenti
    return FALSE;
}

// Funzione per verificare la correttezza dei parametri
FILE *validation(int argc, char *argv[], int args[]) {

    // Output di default
    FILE *output = stdout;

    /*
     * Controllo argomenti obligatori da passare
     * Primo argomento: chiamata al programma
     * Secondo argomento: primo file
     * Terzo argomento: secondo file
     * Quarto argomento: un'operando qualsiasi
     */
    if (argc < 3) {

        // Segnalo l'errore
        errno = EIO;
        perror("Argomenti minimi non presenti");
        exit(EXIT_FAILURE);
    }

    // Serie di check sulla sintassi del comando
    if (starts_with_char(argv[1], '-') || starts_with_char(argv[2], '-')) {

        // Segnalo l'errore
        perror("Manca un file in input");
        exit(EXIT_FAILURE);
    }

    // Check delle funzioni passate
    for (int i = 3; i < argc; i++) {

        // Controllo per inserire -g come opzione
        if (strcmp_bool(argv[i], "-g")) {
            args[0] = TRUE;
        }

        // Controllo per inserire -s come opzione
        if (strcmp_bool(argv[i], "-s")) {
            args[1] = TRUE;
        }

        // Controllo per vedere se -d può essere presente
        if (strcmp_bool(argv[i], "-d")) {

            // Controllo se -u è già presente
            if (args[3]) {

                // Segnalo l'errore
                errno = EIO;
                perror("-u è già presente");
                exit(EXIT_FAILURE);
            }

            args[2] = TRUE;
        }

        // Controllo per vedere se -u può essere presente
        if (strcmp_bool(argv[i], "-u")) {

            // Controllo se -d è già presente
            if (args[2]) {

                // Segnalo l'errore
                errno = EIO;
                perror("-d è già presente");
                exit(EXIT_FAILURE);
            }

            args[3] = TRUE;
        }

        // Controllo per inserire -v come opzione
        if (strcmp_bool(argv[i], "-v")) {
            args[4] = TRUE;
        }

        // Controllo per vedere se -o è stato scritto correttamente
        if (strcmp_bool(argv[i], "-o")) {

            // Controllo che posso effettivamente entrare in argv[i+1]
            if (i + 1 < argc) {

                // Controllo che dopo -o ci sia il file su cui scrivere l'output
                if (starts_with_char(argv[i + 1], '-')) {
                    perror("Manca il nome del file");
                    exit(EXIT_FAILURE);
                }

                // Sennò apro il file e metto output a TRUE
                args[5] = TRUE;
                output = fopen(argv[i + 1], "wb");

                // Sorpasso sopra il nome del file di output
                i = i + 1;

            } else {

                // FALSE perchè manca il nome del file
                errno = EIO;
                perror("Manca il nome del file\n");
                exit(EXIT_FAILURE);
            }

        }

    }

    /*
     * Controllo per vedere se dobbiamo
     * effettivamente svolgere qualche confronto
     */
    if (!args[0] && !args[1] && !args[2] && !args[3])
        exit(0);

    /*
     * Tutti i controlli sono stati eseguiti e passati
     * quindi il programma è pronto per essere eseguito
     */
    return output;
}

// Funzione per calcolare il numero di righe di un file
int count_rows(FILE *file) {
    char check;
    int rows = 0;

    // Conta le righe fino alla fine del file
    while (!feof(file)) {

        // Legge il prossimo carattere
        check = fgetc(file);

        // Per ogni carattere \n fa un incremento unitario
        if (check == '\n') {
            rows++;
        }
    }

    return rows;
}

void check_empty(char *namefile1, char *namefile2, int args[], FILE *output) {
    Stat st1, st2;

    // Controllo se stat  è andata a buon fine
    if (stat(namefile1, &st1) != 0)
        exit(EXIT_FAILURE);

    // Controllo se stat  è andata a buon fine
    if (stat(namefile2, &st2) != 0)
        exit(EXIT_FAILURE);

    // Controllo se entrambi i file sono vuoti
    if (st1.st_size == 0 && st2.st_size == 0) {

        /*
         * In base alle opzioni scelgo cosa printare
         * e con output l'indirizzo su stdout o su un eventuale file
         */
        if (args[1])
            fprintf(output, "%s e %s sono uguali\n", namefile1, namefile2);

        exit(0);
    }

    if (st1.st_size == 0 || st2.st_size == 0) {

        if (args[0])
            fprintf(output, "%s e %s sono diversi\n", namefile1, namefile2);

        exit(0);
    }
}

// Bufferizzo le N righe oppure le righe fino all'EOF
void buffer_n_rows(char *rows[], int max_lines, FILE *file) {
    int lines = 0;

    char *buffer;        // Creo un buffer temporaneo
    size_t len = 0;     // Lunghezza zero che verrà riallocata dalla getline()

    do {

        // Metto buffer che punta a NULL
        buffer = NULL;

        // Metto la stringa dentro al buffer, le stringhe devono avere meno di 200 caratteri
        getline(&buffer, &len, file);

        // Alloco la dimensione esatta della stringa più uno per evitare sprechi
        rows[lines] = (char *) calloc(strlen(buffer) + 1, sizeof(char));

        // Copio la stringa dentro al buffer alla posizione i-esima (lines) nell'array
        strcpy(rows[lines], buffer);

        // Incremento il numero delle righe
        lines++;

        // Dealloco lo spazio da buffer per riallocarlo in seguito
        free(buffer);

    } while (!feof(file) && lines < max_lines);
}


/*
 * Funzione che quando F1 e F2 sono diversi in almeno una riga
 * restituisce la stringa ”F1 ed F2 sono differenti"
 */
int subroutine_g(int comparison, char *namefile1, char *namefile2, FILE *output) {

    // Se i due file sono diversi
    if (!comparison) {
        fprintf(output, "%s e %s sono differenti\n", namefile1, namefile2);
        return FALSE;
    }
    return TRUE;
}

/*
 * Funzione che quando F1 e F2 sono uguali in tutte le righe
 * restituisce la stringa “F1 ed F2 sono uguali”-
 */
int subroutine_s(int comparison) {

    // Se i due file sono diversi
    if (!comparison) {
        return FALSE;
    }
    return TRUE;
}

// Funzione che restituisce i numeri delle righe differenti
void
subroutine_d(int comparison, char *string1, char *string2, char *namefile1, char *namefile2, int line, int specified,
             FILE *output) {
    // Se le due righe sono diverse
    if (!comparison) {
        // Se l'opzione -v è stata impostata
        if (specified) {
            fprintf(output, "Riga %d: %s -> %s\n", line, namefile1, string1);
            fprintf(output, "Riga %d: %s -> %s\n", line, namefile2, string2);
            return;
        }
        // Altrimenti
        fprintf(output, "%d\n", line);
        return;
    }
}

// Funzione che restituisce i numeri delle righe uguali
void
subroutine_u(int comparison, char *string1, char *string2, char *namefile1, char *namefile2, int line, int specified,
             FILE *output) {
    // Se le due righe sono uguali
    if (comparison) {
        // Se l'opzione -v è stata impostata
        if (specified) {
            fprintf(output, "Riga %d: %s -> %s\n", line, namefile1, string1);
            fprintf(output, "Riga %d: %s -> %s\n", line, namefile2, string2);
            return;
        }
        // Altrimenti
        fprintf(output, "%d\n", line);
        return;
    }
}

void
exec_subroutine(int args[], FILE *infile1, FILE *infile2, char *namefile1, char *namefile2, int max, FILE *output) {

    /*
     * Array che include gli argomenti nelle seguenti posizioni
     * 0: -g; 1: -s; 2: -d; 3: -u; 4: -v; 5: -o;
     */

    // Buffer che memorizzano solo 50 righe alla volta
    char *buffer1[50], *buffer2[50];

    int comparison;     // Variabile booleana che ha il risultato del confronto tra due righe

    /*
     * Lines serve per tenere il conto delle righe totali lette
     * iter serve per capire quante righe sono state lette e quante ne vanno iterate
     */
    int lines = max, iter;

    do {
        /*
         * Linee da leggere se non si arriva la massimo (50)
         * se ne legge il numero massimo che il file contiene
         */
        if (lines > 50) {
            buffer_n_rows(buffer1, 50, infile1);
            buffer_n_rows(buffer2, 50, infile2);

            iter = 50;
            lines -= iter;
        } else {
            buffer_n_rows(buffer1, lines, infile1);
            buffer_n_rows(buffer2, lines, infile2);

            iter = lines;
            lines -= iter;
        }

        /*
         * Itera per il numero di righe lette in precedenza
         * Le subroutine vengono utilizzate solo se sono settate a TRUE e nel mentre della lettura
         * posso anche fermarsi se hanno raggiunto la fine prima del termine per evitare sprechi di
         * cicli. Unica eccezione viene fatta sulla -s dove ss indica anche lo stato del file (se uguale o diverso)
         */
        for (int i = 0; i < iter; i++) {

            // Confronto tra le due i-esime righe
            comparison = strcmp_bool(buffer1[i], buffer2[i]);

            // -g
            if (args[0]) {
                args[0] = subroutine_g(comparison, namefile1, namefile2, output);
            }

            // -s
            if (args[1]) {
                args[1] = subroutine_s(comparison);
            }

            // -d
            if (args[2]) {
                subroutine_d(comparison, buffer1[i], buffer2[i], namefile1, namefile2, (max - (lines + iter) + i + 1),
                             args[4], output);
            }

            // -u
            if (args[3]) {
                subroutine_u(comparison, buffer1[i], buffer2[i], namefile1, namefile2, (max - (lines + iter) + i + 1),
                             args[4], output);
            }
        }

    } while (lines > 0);

    // A fine file si fa il controllo per i file uguali
    if (args[1]) {
        fprintf(output, "%s e %s sono uguali\n", namefile1, namefile2);
        return;
    }
}
