#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Includo le strutture definite da me
#include "utilities.h"

/*
 * Definisco la PORT come 8080 
 * E definisco la BACKLOG come 100 prima di rifiutare altri client
 */
#define PORT 8080
#define SERVER_BACKLOG 100

// Definisco TRUE e FALSE
#define TRUE 1
#define FALSE 0

/*
 * Definisco le strutture per evitare di
 * scrivere ogni volta struct nomeStruct 
 * per inizializare le variabili
 */
typedef struct timespec TS;
typedef struct sockaddr SA;
typedef struct sockaddr_in SA_IN;

// File di log
FILE *log_file;

// Struttura dati del client
SA_IN client_addr;

// Mutex per scrittura su file log
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Socket server
int server_socket;

/*
 * Funzione che esegue le operazioni richieste dai 
 * client adattata per funzionare con i thread
 */
void *exec_operations(void *pclient);

/*
 * Funzione che gestisce la scrittura su file
 * di log per le operazioni compiute 
 */
void fill_log(Operation op, Response res, int ip);

// Funzione per la gestione del segnale di interrupt
void interruption_handler(int n);

int main(void) {

    SA_IN server_addr;
    int client_socket, addr_size;

    // Signal handler per SIGINT
    signal(SIGINT, interruption_handler);

    // socket creazione e controllo
    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Creazione socket fallita\n");
        return EXIT_FAILURE;
    }

    // Assegno IP e PORTA
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port        = htons(PORT);

    // Binding della nuova socket all'IP e controllo
    if((bind(server_socket, (SA *)&server_addr, sizeof(server_addr))) != 0) {
        perror("Abbinamento al socket fallito\n");
        return EXIT_FAILURE;
    }

    // Adesso metto il server in ascolto
    if((listen(server_socket, SERVER_BACKLOG)) != 0) {
        perror("Ascolto fallito\n");
        return EXIT_FAILURE;
    }

    // Apertura file di log in append+ (se il file non esiste lo crea)
    if((log_file = fopen("log", "a+")) == NULL) {
        perror("Errore nell'apertura/creazione del file di log\n");
        return EXIT_FAILURE;
    }

    // Loop infinito per accettare nuovi client
    while(TRUE) {   

        printf("Server in ascolto\n");

        addr_size = sizeof(SA_IN);

        // Accetto il pacchetto mandato dal client
        if((client_socket = accept(server_socket, (SA *)&client_addr, (socklen_t *)&addr_size)) < 0) {
            perror("Il server non è riuscito ad accettare\n");
            return EXIT_FAILURE;
        }

        printf("Client accettato!\n");

        pthread_t thread;
        
        /**
         * Creo un puntatore a int per passare l'argomento alla funzione thread
         * questo perchè le pthread vuole che la funzione prenda in input un puntatore
         * di tipo void che poi casteremo a puntatore int per ottenere il valore
         * passato in argomento che sarebbe la client socket
         */
        int *client = malloc(2 * sizeof(int));
        *client = client_socket;

        // Avvio il thread
        pthread_create(&thread, NULL, exec_operations, client);
    }

    return EXIT_SUCCESS;
}

// Funzione che svolge tutte le operazioni
void *exec_operations(void *pclient) {

    /**
     * Castiamo il puntatore void a int per ottenere
     * la socket del client e liberiamo la memoria
     * usata dal puntatore
     */
    int client = *((int *) pclient);
    free(pclient);

    TS spec;
    Operation op;
    Response res;

    while(TRUE) {

        // Riceviamo l'operazione da fare
        if(recv(client, &op, sizeof(Operation), 0) <= 0) {
            printf("Il client ha chiuso la connesione\n");
            close(client);

            return NULL;
        }

        // Prendiamo il tempo alla ricezione
        clock_gettime(CLOCK_REALTIME, &spec);
        res.ricezione = spec.tv_nsec;

        // Controlliamo che operazione dobbiamo fare
        switch(op.operator) {
            case '+':
                res.result = op.first_value + op.second_value;
                break;
            case '-':
                res.result = op.first_value - op.second_value;
                break;
            case '*':
                res.result = op.first_value * op.second_value;
                break;
            case '/':
                res.result = op.first_value / op.second_value;
                break;
            default:
				// Per sicurezza lo lascio
                break;
        }

        // Prendiamo il tempo all'invio
        clock_gettime(CLOCK_REALTIME, &spec);
        res.invio = spec.tv_nsec;

        // Inviamo la risposta al client
        send(client, &res, sizeof(Response), 0);
    
        // Inizio zona critica
        pthread_mutex_lock(&lock);
        
		// Funzione per scrivere il file di log
        fill_log(op, res, 0);
    
        // Fine zona critica
        pthread_mutex_unlock(&lock);
    }
}

// Funzione per scrivere il file di log
void fill_log(Operation op, Response res, int ip) {

    // Converto per ottenere l'IP
    char addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, addr, INET_ADDRSTRLEN);
    unsigned short port = ntohs(client_addr.sin_port);

    // Scrivo il log per il calcolo appena svolto
    fprintf(log_file, "IP: %s:%d \nOperazione: %f %c %f \nRisultato: %f \nTempo inizio: %ld \tTempo fine: %ld \n/--------------------------------------------/\n", addr, port, op.first_value, op.operator, op.second_value, res.result, res.ricezione, res.invio);
}

// Funzione per la gestione del segnale di interrupt
void interruption_handler(int n) {
    
    // Chiudo il file di log
    fclose(log_file);

    // Chiudo la connesione
    close(server_socket);
    
    printf("\n");

    exit(EXIT_SUCCESS);
}
