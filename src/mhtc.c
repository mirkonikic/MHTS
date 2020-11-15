#include "main.h"

void ferr(char* msg);
void *send_msg(void* arg);
void *recv_msg(void* arg);
void parseArg(int argc, char** argv);

char buffer[MSG_SIZE], msg[MSG_SIZE-22], letter[MSG_SIZE];
int fd, recv_length=1, flag=0, hide=0;
int messageIsOn = 0;
pthread_t send_msg_t, recieve_msg_t;
char name[NAME_SIZE];

int main(int argc, char** argv)
{
	struct sockaddr_in serv_addr;

	parseArg(argc, argv);

	if((fd=socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		ferr("Nisam uspeo da kreiram socket!");	
	}


	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	inet_pton(AF_INET, IP_ADRESA, &serv_addr.sin_addr);
	memset(&(serv_addr.sin_zero), '\0', 8);

	if((connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))==-1)
	{
		ferr("\tServer down!");
	}
	
	system("clear");

//Bot auth
	send(fd, "B_NOT", strlen("B_NOT"), 0);

//Name authentication
	recv_length = recv(fd, &buffer, MSG_SIZE, 0);  //Prima KOD(U_ERR ili U_OK)
	if(strcmp(buffer, "U_OK") == 0)  //Ako je kod U_OK
	{
		recv(fd, &name, NAME_SIZE, 0);  //Prima ime
		printf("user: %s\n", name);
		printf("zdravo!\n");
	}
	else if(strcmp(buffer, "U_ERR") == 0)  //Ako je kod U_ERR
	{
		printf("Username: ");  
		scanf("%s", name); //Unesite novo ime 
		send(fd, name, sizeof(name), 0);  //Salje novo ime
		recv_length = recv(fd, &buffer, MSG_SIZE, 0);  //Prima nov kod
		while(strcmp(buffer, "U_ERR") == 0)  //Ako je opet primljen U_ERR
		{
			printf("\t[!]Greska: %s username je zauzet, probaj drugi\n", name);
			sleep(2);
			system("clear");
			printf("Username: ");
			scanf("%s", name);  //Unesite novo ime
			send(fd, name, sizeof(name), 0);  //Salje novo ime
			recv_length = recv(fd, &buffer, MSG_SIZE, 0);
		}
		system("clear");
		recv(fd, &name, NAME_SIZE, 0);  //Ako nije primljen U_ERR kod onda ceka da mu se posajle ime
		printf("user: %s\n", name);
		printf("zdravo!\n");
	}
	else
	{
		printf("%s\n", buffer);
		ferr("Stigao je cudan paket!");
	}
	
	
//TCP INTRO
	recv_length = recv(fd, &buffer, MSG_SIZE, 0);
	while(1)
	{
		if(strcmp(buffer, "U_END")==0)
			break;
		printf(BOLD "%s" RESET, buffer);
		recv_length = recv(fd, &buffer, MSG_SIZE, 0);
	}
	
//PRIMAJ I SALJI PORUKE!!!
    if (pthread_create(&send_msg_t, NULL, send_msg, NULL) != 0) {
        printf ("Create pthread error!\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&recieve_msg_t, NULL, recv_msg, NULL) != 0) {
        printf ("Create pthread error!\n");
        exit(EXIT_FAILURE);
    }

	while(flag != 1)
	{
		sleep(2);
	}

	pthread_cancel(recieve_msg_t);
	pthread_cancel(send_msg_t);

	close(fd);

	return 0;
}

void *send_msg(void* arg)
{
    while (1) {
		fgets(msg, MSG_SIZE-22, stdin);  //Bolje od scanf je cita i space a zaustavlja overflow
		msg[strlen(msg)-1] = '\0';  //Brisem \n sa kraja jer fgets i to procita...

//Proveri da li je komanda prvo
			if(strcmp(msg, "/exit") == 0){
				remove(HIDDEN_DAT);
				break;}
			else if(strcmp(msg, "/clear") == 0){
				system("clear");
				continue;} 
			else if(strcmp(msg, "/hide") == 0){
				system("clear");
				char hostname[1024];
				gethostname(hostname, 1024);
				printf(COLORUSER "%s@%s:" COLORTILDA "~" RESET "$ ", name, hostname);
				FILE *hdn = fopen(HIDDEN_DAT, "w");
				fclose(hdn);
				hide = 1;
				continue;}
			else if(strcmp(msg, "/show") == 0 && hide == 1){
				system("clear");
				printf("\t[+]--------MIRKO's hidden tcpServer--------[+]\n");
				sleep(1);
				system("clear");
				printf("user: %s\nzdravo!\n\n", name);

				FILE *hdn = fopen(HIDDEN_DAT, "r");
				while(fgets(msg, MSG_SIZE, hdn)!=NULL)
				{
					printf("%s", msg);
				}
				fclose(hdn);

				hide = 0;
				continue;}
			else if(strcmp(msg, "/msg0") == 0){
				messageIsOn = 0;
				continue;}
			else if(strcmp(msg, "/msg1") == 0){
				messageIsOn = 1;
				continue;}
			else if(strcmp(msg, "/msg2") == 0){
				messageIsOn = 2;
				continue;}
			else if(strcmp(msg, "/scan") == 0){
				printf(BOLDWHITE"\t_(scan_finished)_\n"RESET);
				strcpy(letter, "C_SCAN");
				encrypt(letter, strlen(letter));
				send(fd, letter, strlen(letter), 0);
				continue;}
			else if(strcmp(msg, "/poke") == 0){
				//printf(BOLDWHITE"\t_(scan_finished)_\n"RESET);
				//strcpy(letter, "C_POKE");
				//encrypt(letter, strlen(letter));
				//send(fd, letter, strlen(letter), 0);
				printf(BOLDWHITE"\t(soon to be implemented)\n"RESET);
				continue;}
			else if(msg[0] == '/'){
				printf(BOLDWHITE"\tnije prepoznata komanda..\n"RESET);
				continue;}



//Nije komanda naci poruka je
		strcpy(letter, name);
		strcat(letter, ": ");
		strcat(letter, msg);
		encrypt(letter, strlen(letter));
	if(hide!=1)	
        	send(fd, letter, strlen(letter), 0);
    }
	flag=1;
	return NULL;
}

void *recv_msg(void* arg)
{
    while ((recv_length = recv(fd, buffer, MSG_SIZE, 0))!=0) {
		while(hide == 1)
		{
			FILE *hdn = fopen(HIDDEN_DAT, "a+");
			decrypt(buffer, recv_length);
			buffer[recv_length] = '\0';
			fprintf(hdn, "%*s\n", recv_length, buffer);
			fclose(hdn);
			recv_length = recv(fd, buffer, MSG_SIZE, 0);
		}
		decrypt(buffer, recv_length);
		for(int i = 0; i<recv_length; i++)
		{
			printf("%c", buffer[i]);
		}
		if(messageIsOn == 1)
		{
			if(fork() == 0)
			{
				buffer[recv_length] = '\0';
				if(buffer[0] == '\t')
					buffer[0]=' ';
				execl("/bin/xmessage", "/bin/xmessage", "-nearmouse", buffer, (char *)NULL);
				exit(0);
			}
			//wait(NULL);
		}
		else if(messageIsOn == 2)
		{
			printf("\a");
		}
		printf("\n");
    }
	return NULL;
}

void ferr(char* msg)
{
	char k[100];
	strcpy(k, "[!]Error: ");
	strncat(k, msg, 90);
	strcat(k, "\n");
  	fprintf(stderr, "%s", k);
	exit(1);
}

void parseArg(int argc, char** argv)
{
	if(argc > 1)
	{
		if(argv[1][0] == '-')
		{
            if(strcmp(argv[1], "-msg") == 0)
            {
				if(argc > 2)
				{
					if(strcmp(argv[2], "1") == 0)
					{
						messageIsOn = 1;
					}
					else if(strcmp(argv[2], "0") == 0)
					{
						messageIsOn = 0;
					}
					else if(strcmp(argv[2], "2") == 0)
					{
						messageIsOn = 2;
					}
					else
					{
						fprintf(stderr, "Error: Vrednost nije prepoznata!\n");
						exit(1);
					}
				}
				else
				{
					fprintf(stderr, "Error: Nije prosledjena vrednost zadataom argumentu!\n");
					exit(1);
				}
            }
            else if(strcmp(argv[1], "--help") == 0)
            {
                
                exit(1);
            }
            else
            {
                fprintf(stderr, "Error: Argument nije prepoznat!\n");
                exit(1);
            }
		}
		else
		{
			fprintf(stderr, "Error: Argument mora poceti sa -\n");
			exit(1);
		}
	}
}