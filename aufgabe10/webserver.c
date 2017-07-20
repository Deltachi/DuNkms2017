#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int listenSocket;

void startServer()
{
    /* Hier soll ein Socket erstellt und konfiguriert werden, der auf
     * eingehende Anfragen wartet. Die Reihenfolge der benötigten syscalls
     * (bind, listen, socket) und deren Verwendung
     * finden Sie im Anhang der Vorlesungsfolien zum Thema Rechnernetze.
     * Die folgenden Zeilen sind zu erweitern und in die richtige Reihenfolge zu bringen.
     */

    struct sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(8080);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if((listenSocket = socket(my_addr.sin_family,SOCK_STREAM, 0)) == -1)
	printf("Fehler! Kann Socket nicht erstellen.\n");

    bind(listenSocket, (struct sockaddr*)&my_addr ,sizeof(my_addr));
    listen(listenSocket, 5);


}

void handleConnections()
{
    /* Nachdem ein Socket erstellt wurde, soll in dieser Funktion auf eingehende
     * Anfragen gewartet werden. Antworten Sie auf die Anfrage mit einem fixen String.
     *
     * Zum Debuggen ist das Tool telnet sehr nützlich.
     */

    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    int connection_f = accept(listenSocket, (struct sockaddr*)&client, &len);

    send(connection_f, "Hello World!", 12, 0);

     /*
      * Wenn eine einfache Anfrage funktioniert, erweitern Sie das Programm,
      * sodass der HTTP-Header ausgewertet wird. Der HTTP-Header der Anfrage lässt sich mit
      * recv() und strtok() parsen.
      * Öffnen Sie dann die angeforderte Datei und senden Sie sie an den Client. (Vorher muss
      * ein HTTP-Header gesendet werden.)
      */
     /*recv(connection_f, ..., ..., ...);
     fopen();
     send();*/

     /*
      * Und fertig ist der Webserver! Das war ja einfach! :-)
      */
}

int main()
{
    startServer();
    handleConnections();
    close(listenSocket);
    return 0;
}
