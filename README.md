# MQ in disastrOS

> Message Queues in disastrOS ---> [x1] [C98]  
> implement an IPC system based on message queues in disastrOS to allow (blocking) communication between processes;  
> the starting point is the version of disastrOS presented in class -  
> available [here](https://gitlab.com/grisetti/sistemi_operativi_2019_20/-/tree/master/source/08_disastrOS/disastrOS_04_resources).
 
## System calls fondamentali da implementare:
* Crea coda: crea la coda dentro le strutture.
* Unlink coda: elimina la coda dalle strutture. 
* Apri coda: restituisce un descrittore della coda tramite cui si potrà poi leggere o scrivere. 
* Chiudi coda: elimina il descrittore.
* Scrivi messaggio: prende il descrittore della coda aperta e ci scrive sopra. Se la coda è piena si blocca. Il processo che tenta di scrivere in una coda piena viene messo a dormire. 
* Leggi messaggio: prende il descrittore della coda aperta e legge un messaggio. Se la coda è vuota si blocca. Il processo che tenta di leggere da una coda vuota viene messo a dormire. 

Il processo che manda il messaggio rimarrà bloccato (a dormire) fino a quando non riceverà un ACK che gli comunica che un altro processo in ascolto sulla queue ha letto il messaggio.

