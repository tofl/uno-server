#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MYPORT 3490     // Port du point de connexion
#define BACKLOG 10      // Combien de connexion en attente autorise-t-on ?
#define MAXDATASIZE 100 // Nombre maximal d'octets à envoyer en une fois

using namespace std;

int main() {
    unsigned int sock_fd, new_fd;  // sock_fd : point de connexion, new_fd : socket de communication
    struct sockaddr_in my_addr;    // Adresse de transport de la socket côté serveur
    struct sockaddr_in their_addr; // Adresse de transport de la socket côté client
    unsigned int sin_size;         // Taille d'une adresse de transport
    char buffer[MAXDATASIZE];      // Message recu
    int length;                    // Taille du message reçu
    int yes = 1;
    char* date_string;             // Chaine de caractères contenant la date
    struct timeval time_value;

    struct socketAddr {};

    // Creation et initialisation du point de connexion
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int));

    // Configuration de l'adresse de transport
    my_addr.sin_family = AF_INET;         // type de la socket
    my_addr.sin_port = htons(MYPORT);     // port, converti en reseau
    my_addr.sin_addr.s_addr = INADDR_ANY; // adresse, devrait être converti en reseau mais est egal à 0
    bzero(&(my_addr.sin_zero), 8);        // mise a zero du reste de la structure

    // Demarrage du point de connexion : on ajoute l'adresse de transport dans la socket
    // bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
    // cout << typeid(bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))).name() << '\n';
    ::bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));

    // Attente sur le point de connexion
    listen(sock_fd, BACKLOG);

    // Boucle principale de traitement des demandes client
    while(1)
    {
        sin_size = sizeof(struct sockaddr_in);

        // Accept est bloquant : le processus n'avance pas tant que personne ne se connecte
        new_fd = accept(sock_fd, (struct sockaddr *)&their_addr, &sin_size);

        // Une connexion a eu lieu : on recupere l'adresse du client
        printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr)); // inet_ntoa : net to ascii

       while(1) {
           if ((length = recv(new_fd, buffer, MAXDATASIZE, 0)) == -1)
           {
               // S'il y eu une erreur, c'est certainement que le client s'est déconnecté
               // On affiche l'erreur
               perror("Reception d'un message : ");
               // On ferme la socket de communication
               if (close(new_fd) == -1)
                   perror("Fermeture de la socket de communication : ");
               // Et on quitte la boucle de traitement des messages du client
               break;
           }

           // Suppression des retours chariots (\n et \r)
           while (length > 0 && (buffer[length - 1] == '\n' || buffer[length - 1] == '\r'))
               length--;
           // Ajout de backslash zero a la fin pour en faire une chaine de caracteres
           if (length >= 0 && length < MAXDATASIZE)
               buffer[length] = '\0';
           // Affichage du message
           printf("Message recu : %s \n", buffer);
           // On recupere l'heure et la date

           // time(&time_value);
           // time_info = localtime(&time_value);
           // Traitement du message reçu
           if (strcmp(buffer, "DATE") == 0)
           {
               // strftime(date_string, MAXDATASIZE, "%e/%m/%Y", time_info);
               char returnString[] = "my name is tom";
               // On envoie l'heure et la date
               if (send(new_fd, returnString, strlen(date_string), 0) == -1)
                   perror("Envoi d'un message : ");
           }
           else if (strcmp(buffer, "MOIS") == 0)
           {
               // strftime(date_string, MAXDATASIZE, "%B", time_info);
               char returnString[] = "my name is tom";
               // On envoie l'heure et la date
               if (send(new_fd, returnString, strlen(date_string), 0) == -1)
                   perror("Envoi d'un message : ");
           }
           else if (strcmp(buffer, "QUITTER") == 0)
           {
               // On ferme la socket de communication
               if (close(new_fd) == -1)
                   perror("Fermeture de la socket de communication : ");
               // Et on quitte la boucle de traitement des messages du client
               break;
           }

       }
    }

    return 0;
}