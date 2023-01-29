#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

void pexit(char *errmsg) {
	fprintf(stderr, "%s\n", errmsg);
	exit(1);
}

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char buffer[1025];
    time_t ticks; 

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		pexit("socket() error.");
		
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(buffer, '0', sizeof(buffer)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int port = 4999;
	do {
		port++;
    	serv_addr.sin_port = htons(port); 
    } while (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0);
	printf("bind() succeeds for port #%d\n", port);

    if (listen(listenfd, 10) < 0)
		pexit("listen() error.");

	int counter=0;
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
		counter++;
		printf("connected to client %d.\n", counter);
		if (fork() > 0)
			continue;
		//hardcode to send "ls" command output 
        // snprintf(buffer, sizeof(buffer), "Client %d: %.24s\r\n", counter, ctime(&ticks));
		//FILE *fcommand = popen("ls -l", "r");
		
		int n;
		//get the output of that command and forward it to client
		// while ((n = fread(buffer, 1, sizeof(buffer), fcommand)) > 0)
        // 	write(connfd, buffer, n); 
		int wins = 0;
		int loses = 0;
		char question[] = "Heads or Tails?";
		write(connfd,question,sizeof(question));
		//read a line from the client and write it back to the client
		while ((n = read(connfd, buffer, sizeof(buffer))) > 0) 
		{
			//do something based on buffer and send that instead?
			//simply increment each character!
			//for(int i=0; i<n; i++)
				//buffer[i]++;
			//write(connfd, buffer, n);
			for(int i = 0;i<strlen(buffer);i++)
			{
				buffer[i] = tolower(buffer[i]);
			}
			char heads = strstr(buffer,"heads");
			char tails = strstr(buffer,"tails");
			char not[] = "Not a Guess";
			int ran = rand()%2;
			char r[100];
			char state[1000];
			if(heads)
			{
				int guess = 0;
				if(heads)
				{
					++wins;
					sprintf(state,"Whoa you won %d times and lost %d times\nHeads or Tails?",wins,loses);
					write(connfd,state,sizeof(state));
				}
				else
				{		
					++loses;
					sprintf(state,"Whoa you won %d times and lost %d times\nHeads or Tails?",wins,loses);
					write(connfd,state,sizeof(state));
				}
			}
			else if(tails)
			{
				int guess = 1;
				if(guess == ran)
				{
					++wins;
					sprintf(state,"Whoa you won %d times and lost %d times\nHeads or Tails?",wins,loses);
					write(connfd,state,sizeof(state));
				}
				else
				{
					++loses;
					sprintf(state,"Whoa you won %d times and lost %d times\nHeads or Tails?",wins,loses);
					write(connfd,state,sizeof(state));
				}
			}
			else
			{
				write(connfd,not,sizeof(not));
			}
		//	sprintf(r,"%d",ran);
		//	write(connfd,r,sizeof(r));
		}
		printf("served client %d.\n", counter);
		//pclose(fcommand);
        close(connfd);
        //sleep(1);
		exit(0); //this is child server process. It is done!
     }
}
