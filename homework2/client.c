#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Includo le strutture definite da me
#include "utilities.h"

// Definisco PORT come 8080
#define PORT 8080

// Definisco TRUE e FALSE
#define TRUE 1
#define FALSE 0

/*
 * Definisco le strutture per evitare di
 * scrivere ogni volta struct nomeStruct 
 * per inizializare le variabili
 */
typedef struct sockaddr SA;
typedef struct sockaddr_in SA_IN;

int sockfd;

// Funzione che svolge le operazioni
void do_operations(int sockfd);

// Funzione che gestisce i segnali di interrupt
void interruption_handler(int n);

int main(void) {
    
    // Struttura dati del server
    SA_IN server_addr;

    // Signal handler per SIGINT
    signal(SIGINT, interruption_handler);

    // Creiamo la socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Creazione del socket fallita");
        exit(EXIT_FAILURE);
    }

    // Assegnamo TIPO, IP e PORTA
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port        = htons(PORT);

    // Adesso ci colleghiamo con il server
    if(connect(sockfd, (SA *)&server_addr, sizeof(server_addr)) != 0) {
        perror("Connessione con il server fallita");
        exit(EXIT_FAILURE);
    }

    printf("Connesso con il server!\n");

    // Faccio partire la funzione per prendere in input le operazioni da fare
    do_operations(sockfd);

    // Chiudo il socket
    close(sockfd);

    return EXIT_SUCCESS;
}

// Funzione che gestisce i segnali di interrupt
void do_operations(int sockfd) {
    
    Operation op;
    Response res;

    while(TRUE) {

        printf("Inserisci l'operatione da fare: ");

        // Controllo se la lettura è andata a buon fine
        if(scanf("%f %c %f", &op.first_value, &op.operator, & op.second_value) != 3) {
            
            printf("Operazione non valida!\n");

            // Svuoto il stdin
            while(getchar() != '\n');

            // Continuo ad eseguire il while
	        continue;
            
        }

		if(op.second_value != 0 && (op.operator == '+' || op.operator == '-' || op.operator == '*' || op.operator == '/')) {

        	// Mando l'operazione da fare al server
        	send(sockfd, &op, sizeof(Operation), 0);

        	// Prendo la risposta dal server
        	if(recv(sockfd, &res, sizeof(Response), 0) <= 0) {
				printf("Il server ha chuso la connessione\n");

				return;
			}

        	// Metto la risposta in out come risultato e tempo impiegato
    		printf("Risultato: %f \t Tempo: %ld ns\n", res.result, res.invio - res.ricezione);

    	} else {
			
			printf("Operazione non valida!\n");
		}
	}
}

// Funzione che gestisce i segnali di interrupt
void interruption_handler(int n) {
    /*
     * Uso la write indirizzata allo stdout per 
     * evitare problemi con il gestore del segnale
     * dato che la printf potrebbe non essere garantita
     */
    write(STDOUT_FILENO, "\nChiudo la connesione\n", 22);
    
    // Chiudo la socket e finisco l'esecuzione del programma
    close(sockfd);
    
    // Esco dall'esecuzione del programma
    exit(EXIT_SUCCESS);
}
