#include "main.h"

#define MAX 10

void ferr(char* msg); //Izbacuje error poruku u stderr
void c_rewind(FILE *fp);
void intro(int t);
void tcpIntro(int t, int sfd);
const char* returnTimeNow();
void scan(int c, int i);

char ip_adr[MAX][INET_ADDRSTRLEN], ip_adr_tmp[INET_ADDRSTRLEN]; //Cuva adrese
char name[MAX][NAME_SIZE], name_tmp[NAME_SIZE]; //Cuva imena
int bot[MAX], bot_num=0;
char buf[MSG_SIZE], buf_tmp[MSG_SIZE-20];

int main(int argc, char** argv)
{
    system("clear");

    fd_set master;
    fd_set read_fds;
    int fdmax;

    int listener;
    int newfd;

    struct sockaddr_in client_addr, server_addr;

    int recv_length, yes=1;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    memset(&(server_addr.sin_zero), '\0', 8);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

//ANIMACIJA
    intro(100000);
    printf("\t[+]Otvaram socket na portu %d\n", PORT);
	system("./loading.sh 8");

    if((listener=socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		ferr("Neuspelo kreiranje socketa!");
	}

    if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		ferr("Neuspelo postavljanje opcija socketu!");
	}

//ANIMACIJA_2
    sleep(rand()%2);
   	printf("\t[+]Bindujem se na port %d\n", PORT);
	system("./loading.sh 2");

    if(bind(listener, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		ferr("Ne mogu da se bindujem na port!");
	}

//ANIMACIJA_3
    sleep(rand()%3);
	printf("\t[+]Cekam na konekcije...\n");

    if(listen(listener, 3) == -1)
	{
		ferr("Nesto mi ne da da slusam!");
	}

    FD_SET(listener, &master);

    fdmax = listener;

    while(1)
    {
        read_fds = master;
        if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
        {
            ferr("Select()...");
        }

        for(int i = 0; i<= fdmax; i++)
        {
            if(FD_ISSET(i, &read_fds))
            {
                if(i == listener && fdmax<(MAX+4)) // AKO JE LISTENER NASAO NOV SOCKET
                {
                    socklen_t addrlen = sizeof(struct sockaddr_in);
                    newfd = accept(listener, (struct sockaddr *)&client_addr, &addrlen);
                    if(newfd == -1)
                    {
                        ferr("Nova konekcija je pukla...");
                    }
                    
                        FD_SET(newfd, &master);
                        if(newfd > fdmax)
                        {
                            fdmax = newfd;
                        }
                        inet_ntop(AF_INET, &(client_addr.sin_addr), ip_adr[newfd], INET_ADDRSTRLEN);
            			printf("\n--------[!]Nova konekcija klijent: %s, %d\n\n", ip_adr[newfd], newfd);

//PRIMI FAJL!! mozda...?


//Prepoznaj nc jer clijent salje kod prvo za auth
                        recv_length = recv(newfd, buf, MSG_SIZE, 0);
                        buf[recv_length] = '\0';
                        if(strcmp(buf, "B_NOT") == 0)
                        {
                            //printf("NIJE NC BOT %d %s\n", recv_length, buf);
                            //PROVERI IME!!  
                            bot[newfd] = 0;       
                            FILE *nm;
                            if((nm=fopen(NICK_DAT, "r+")) == 0)
                            {
                                ferr("Fali nick.dat fajl!\n");
                            }
                            
                            int addreseIsInFile = 0;
                            c_rewind(nm);
                            printf("\t[?]Pokusaj identifikacije klijenta:\n");
                            while(fscanf(nm, "%[^:]:%s\n", ip_adr_tmp, name_tmp) != EOF)  
                            {
                                printf("\t   %s %s == %s?\n", name_tmp, ip_adr_tmp, ip_adr[newfd]);
                                if(strcmp(ip_adr[newfd], ip_adr_tmp) == 0)
                                {
                                    printf("\t[+]User: %s, Address: %s == %s\n", name_tmp, ip_adr_tmp, ip_adr[newfd]);
                                    strcpy(name[newfd], name_tmp);
                                    addreseIsInFile = 1;
                                    break;
                                }	
                            }
                            printf("\n");

    //AKO JESTE I NIJE U FAJLU!!!
                            if(addreseIsInFile == 0)
                            {
                                int nameIsInFile = 0;
                                send(newfd, "U_ERR", sizeof("U_ERR"), 0); //SALJE DA NE POSTOJI IME ZA DATU IP ADDRESU, OVO CE SE CUVATI U FAJLU!
                        //Upisujem u logs.dat            
                                FILE *lg = fopen(LOGS_DAT, "a");
                                fprintf(lg, "%s | [!]Novi user! U_ERR -> %s!\n", returnTimeNow(),  ip_adr[newfd]);
                                fclose(lg);

                                recv_length = recv(newfd, &name[newfd], NAME_SIZE, 0); //CEKA ODGOVOR I NOVO IME

            //Spremi fajl za opet pretragu sa c_rewind
                                c_rewind(nm);
                                printf("\t[?]Pretrazujem databazu za postojeca imena:\n");
                                while(fscanf(nm, "%[^:]:%s\n", ip_adr_tmp, name_tmp) != EOF)  
                                {
                                    printf("\t   %s %s == %s?\n", ip_adr_tmp, name_tmp, name[newfd]);
                                    if(strcmp(name[newfd], name_tmp) == 0) //Uporedjuje da li postoji ime u fajlu vec
                                    {
                                        printf("\t[+]User: %s, Address: %s == %s\n", name_tmp, ip_adr_tmp, name[newfd]);
                                        nameIsInFile = 1;
                                        break;
                                    }	
                                }
                                printf("\n");

                                while(nameIsInFile == 1)  //AKO VEC POSTOJI SALJIU ERROR
                                {
                                    printf("\t[!]Saljem U_ERR flag -> %s je zauzeto!\n\n", name[newfd]);
                                    
                        //Upisujem u logs.dat            
                                    lg = fopen(LOGS_DAT, "a");
                                    fprintf(lg, "%s | [!]Saljem U_ERR flag -> %s, %s je zauzeto!\n", returnTimeNow(),  ip_adr[newfd], name[newfd]);
                                    fclose(lg);

                                    send(newfd, "U_ERR", sizeof("U_ERR"), 0);

                                    recv_length = recv(newfd, &name[newfd], NAME_SIZE, 0);
                                    c_rewind(nm);
                                    nameIsInFile = 0;
                                    printf("\t[?]Pretrazujem bazu za postojeca imena:\n");
                                    while(fscanf(nm, "%[^:]:%s\n", ip_adr_tmp, name_tmp) != EOF)  
                                    {
                                        printf("\t   %s %s == %s?\n", ip_adr_tmp, name_tmp, name[newfd]);
                                        if(strcmp(name[newfd], name_tmp) == 0) //Uporedjuje da li postoji ime u fajlu vec
                                        {
                                            printf("\t[+]User: %s, Address: %s == %s\n", name_tmp, ip_adr_tmp, name[newfd]);
                                            nameIsInFile = 1;
                                            break;
                                        }	
                                    }
                                    printf("\n");
                                }

                                printf("\t[!]Upisujem username u fajl...\n");
                                fprintf(nm, "%s:%s\n", ip_adr[newfd], name[newfd]);
                                printf("\t[+]Novi user upisan u databazu: %s -> %s\n", name[newfd], ip_adr[newfd]);
                    //Upisujem u logs.dat            
                                lg = fopen(LOGS_DAT, "a");
                                fprintf(lg, "%s | [+]Novi user upisan u databazu: %s -> %s\n", returnTimeNow(), name[newfd], ip_adr[newfd]);
                                fclose(lg);
                            }
                            else if(addreseIsInFile != 1)
                            {
                                ferr("Problem sa imenom je nastao!");
                            }

                            printf("\n\t[+]Identifikovao sam klijenta %s, %s\n", ip_adr[newfd], name[newfd]);
                            send(newfd, "U_OK", sizeof("U_OK"), 0);
                            sleep(1);
                            send(newfd, name[newfd], sizeof(name[newfd]), 0);
                            sleep(1);
                            fclose(nm);

    //INTRO SEND
                            tcpIntro(100000, newfd);  //Salje animaciju klijentu
                            
                        }
//JESTE BOT NC
                        else
                        {
                            bot[newfd] = 1;
                            bot_num++;
                            printf("\t[n]Korisnik nc-a se prikljucio : %s\n", ip_adr[newfd]);
                            strcpy(name[newfd], "nc_bot");
                            sprintf(name_tmp, "%d", newfd);
                            strncat(name[newfd], name_tmp, NAME_SIZE-strlen(name[newfd]));
                            //tcpIntro(100000, newfd);
                            //printf("JESTE NC BOT %d %s\n", recv_length, buf);
                        }

                        //send(newfd, "U_END", sizeof("U_END"), 0);
                        printf("--------[!]Prihvacena konekcija klijent: %s, %s, %d\n\n", name[newfd], ip_adr[newfd], newfd);
            //Upisujem u logs.dat            
                        FILE *lg = fopen(LOGS_DAT, "a");
                        fprintf(lg, "%s | [!]Prihvacena konekcija klijent: %s, %s, %d\n", returnTimeNow(), name[newfd], ip_adr[newfd], newfd);
                        fclose(lg);

                        //printf("%d je 1 bot ili 0 nije\n", bot[newfd]);

                        if(bot[newfd] != 1)
                            scan(1, newfd);

                        strcpy(buf, "\t[!]");
                        strcat(buf, name[newfd]);
                        strcat(buf, " se konektovao!");
                        encrypt(buf, strlen(buf));
                        for(int j = 0; j<fdmax+1; j++)
                        {
                            if(FD_ISSET(j, &master))
                            {
                                if(j!=listener && j!=newfd)
                                {   
                                    if(send(j, buf, strlen(buf), 0) == -1)
                                    {
                                        ferr("Nmg da saljem!");
                                    }
                                }
                            }
                        }
                }
                else  //HANDLE DATA FROM CLIENT
                {
                    recv_length = recv(i, buf_tmp, MSG_SIZE, 0);
                    if(recv_length<=0)
                    {
                        if(recv_length == 0)
                        {
                            printf("\t[-]Klijent: %s, fd->%d, ip->%s je prekinuo konekciju\n",name[i], i, ip_adr[i]);

                    //Upisujem u logs.dat            
                                FILE *lg = fopen(LOGS_DAT, "a");
                                fprintf(lg, "%s | [-]Klijent: %s, fd->%d, ip->%s je prekinuo konekciju\n", returnTimeNow(),  name[i], i, ip_adr[i]);
                                fclose(lg);

                            if(bot[i] != 1)
                                scan(2, i);
                            else
                                bot_num--;
                            

                            strcpy(buf, "\t[!]");
                            strcat(buf, name[i]);
                            strcat(buf, " je napustio chat!");
                            encrypt(buf, strlen(buf));
                            usleep(100);
                            for(int j = 0; j<fdmax+1; j++)
                            {
                                if(FD_ISSET(j, &master))
                                {
                                    if(j!=listener && j!=i)
                                    {
                                        if(send(j, buf, strlen(buf), 0) == -1)
                                        {
                                            ferr("Nmg da saljem!");
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            ferr("Recv()...");
                        }
                        close(i);
                        FD_CLR(i, &master);
                    }
                    else
                    {
                        decrypt(buf_tmp, recv_length);
                        buf_tmp[recv_length] = '\0';
                        if(strcmp(buf_tmp, "C_SCAN") == 0)
                        {
                            printf("\t[?]Klijent %s trazi da vidi ko je aktivan sa adrese %s\n", name[i], ip_adr[i]);
                            FILE *lg = fopen(LOGS_DAT, "a");
                                fprintf(lg, "%s | [?]Klijent %s trazi da vidi ko je aktivan sa adrese %s\n", returnTimeNow(),  name[i], ip_adr[i]);
                            fclose(lg);
                            scan(0, i);
                        }
                        else
                        {
                            encrypt(buf_tmp, recv_length);
                            printf("\t[!]Dobio sam poruku od %s, %d, %d bajta: ", ip_adr[i], i, recv_length);
                        
                //Upisujem u logs.dat            
                                FILE *lg = fopen(LOGS_DAT, "a");
                                fprintf(lg, "%s | [m]Dobio sam poruku od %s, %s, %d bajta\n", returnTimeNow(),  ip_adr[i], name[i], recv_length);
                                fclose(lg);
//AKO BOT NC POSALJE PORUKU
                            if(bot[i] == 1)
                            {
                                strcpy(buf, name[i]);
                                strcat(buf, ": ");
                                buf_tmp[recv_length-1] = '\0'; //Skidam \n znak
                                strncat(buf, buf_tmp, recv_length);  //Koristim za prosledjivanje
                                int st = (recv_length-1) + strlen(name[i]) + 2;
                                for(int z = 0; z<st; z++)
                                {
                                    printf("%c", buf[z]);
                                }
                                printf("\n");
                                FILE *ms = fopen(MESSAGES_DAT, "a");
                                fprintf(ms, "%s | %s\n", returnTimeNow(), buf);
                                fclose(ms);
                                encrypt(buf, st);
                                for(int j = 0; j<fdmax+1; j++)
                                {
                                    if(FD_ISSET(j, &master))
                                    {
                                        if(j!=listener && j!=i)
                                        {
                                            if(bot[j]!=1)
                                            {
                                                if(send(j, buf, st, 0) == -1)
                                                {
                                                    ferr("Nmg da saljem!");
                                                }
                                            }
                                            else
                                            {
                                                decrypt(buf, st);
                                                if(send(j, buf, st, 0) == -1)
                                                {
                                                    ferr("Nmg da saljem!");
                                                }
                                                encrypt(buf, st);
                                            }
                                        }
                                    }
                                }
                            }
    /*AKO JE TCPC KLIJENT*/ else
                            {
                                strcpy(buf, buf_tmp);  //Koristim za prosledjivanje
                                decrypt(buf_tmp, recv_length); //Koristim za ispis poruke i upis u fajl        

                                    for(int z = 0; z<recv_length; z++)
                                    {
                                        printf("%c", buf_tmp[z]);
                                    }
                                    printf("\n");
                                    for(int j = 0; j<fdmax+1; j++)
                                    {
                                        if(FD_ISSET(j, &master))
                                        {
                                            if(j!=listener && j!=i)
                                            {
                                                //int st = recv_length + strlen(name[i]) + 2;
                                                if(send(j, buf, recv_length, 0) == -1)
                                                {
                                                    ferr("Nmg da saljem!");
                                                }
                                            }
                                        }
                                    }
                                    FILE *ms = fopen(MESSAGES_DAT, "a");
                        buf_tmp[recv_length] = '\0';
                                    fprintf(ms, "%s | %s\n", returnTimeNow(), buf_tmp);
                                    fclose(ms);
                            }
                        }
                    }
                }
            }
        }



    }

    return 0;
}

const char* returnTimeNow()
{
    static char a[25];

    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strcpy(a, asctime (timeinfo));
    a[strlen(a)-1] = '\0'; //Da skinem \n znak sa kraja

    return a;
}

void ferr(char* msg)
{
	char k[100];
	strcpy(k, "Error: ");
	strncat(k, msg, 90);
	strcat(k, "\n");
	fprintf(stderr, k);
	exit(1);
}

void c_rewind(FILE *fp)
{
	if ( fseek(fp, 0L, SEEK_SET) != 0 ) { 
			ferr("Nisam uspeo da se vratim na pocetak fajla");
			/* Handle repositioning error */
	}
}

void intro(int t)
{
	sleep(rand()%10);
	printf("\n\t       .-------------------."); 	//top of computer
	usleep(t);
	printf("\n\t       |.-----------------.|"); 		//top of display
	usleep(t);	
	printf("\n\t       ||usr:$./mirko's   ||");
	usleep(t);	
	printf("\n\t       ||>hidden          ||");
	usleep(t);
	printf("\n\t       ||>tcpServer       ||");		//running first instruction
	usleep(t);
	printf("\n\t       ||                 ||");
	usleep(t);
	printf("\n\t       ||                 ||");
	usleep(t);
	printf("\n\t       ||                 ||");	
	usleep(t);
	printf("\n\t       |\"-----------------\'|");
	usleep(t);
	printf("\n\t       |^-.-----------------^-.");
	usleep(t);
	printf("\n\t        ^-. ^-  _______________ ^-.");
	usleep(t);
	printf("\n\t            ^-.| -------~     kALi|");
	usleep(t);
	printf("\n\t               \"------------------\'\n\n\n");
	usleep(t*2);
	printf("\t[+]--------MIRKO's hidden tcp chat--------[+]\n\n\n");
}

void tcpIntro(int t, int sfd)
{
	sleep(rand()%5);
	send(sfd, "\n\t       .-------------------.\n", sizeof("\n\t       .-------------------.\n"), 0); 	//top of computer
	usleep(t);
	send(sfd, "\t       |.-----------------.|\n", sizeof("\t       |.-----------------.|\n"), 0); 		//top of display
	usleep(t);	
	send(sfd, "\t       ||usr:$./mirko's   ||\n", sizeof("\t       ||usr:$./mirko's   ||\n"), 0);
	usleep(t);	
	send(sfd, "\t       ||>hidden          ||\n", sizeof("\t       ||>hidden          ||\n"), 0);
	usleep(t);
	send(sfd, "\t       ||>tcpServer       ||\n", sizeof("\t       ||>tcpServer       ||\n"), 0);		//running first instruction
	usleep(t);
	send(sfd, "\t       ||                 ||\n", sizeof("\t       ||                 ||\n"), 0);
	usleep(t);
	send(sfd, "\t       ||                 ||\n", 30, 0);
	usleep(t);
	send(sfd, "\t       ||                 ||\n", sizeof("\t       ||                 ||\n"), 0);	
	usleep(t);
	send(sfd, "\t       |\"-----------------\'|\n", sizeof("\t       |\"-----------------\'|\n"), 0);
	usleep(t);
	send(sfd, "\t       |^-.-----------------^-.\n", sizeof("\t       |^-.-----------------^-.\n"), 0);
	usleep(t);
	send(sfd, "\t        ^-. ^-  _______________ ^-.\n", sizeof("\t        ^-. ^-  _______________ ^-.\n"), 0);
	usleep(t);
	send(sfd, "\t            ^-.| -------~     kALi|\n", sizeof("\t            ^-.| -------~     kALi|\n"), 0);
	usleep(t);
	send(sfd, "\t               \"------------------\'\n\n", sizeof("\t               \"------------------\'\n\n"), 0);
	usleep(t*2);
	send(sfd, "\n\t[+]--------MIRKO's hidden tcp chat--------[+]\n\n", sizeof("\n\t[+]--------MIRKO's hidden tcp chat--------[+]\n\n"), 0);
	usleep(t);
	send(sfd, "U_END", sizeof("U_END"), 0);
}

void scan(int c, int i) //0 je za traznju, 1 je za aktivan je, 2 je za diskonnekt
{
    int pos;
    char disc[2];
    if(c == 0) //korisnik trazi da vidi ko je aktivan i kad je bio posl put
    {
        FILE *ac;
        if((ac = fopen(ACTIVE_DAT, "r")))
        {
            while(fscanf(ac, "%[^;];%[^;];%s\n ", name_tmp, disc, buf_tmp) != EOF)
            {
                if(strcmp(disc, "c")==0)
                {
                    //printf("|||%s je online|||\n", name_tmp);
                    strcpy(buf, "\t * ");
                    strcat(buf, name_tmp);
                    strcat(buf, " je online");
                    //printf("|||%s|||\n", buf);
                    encrypt(buf, strlen(buf));
                    send(i, buf, strlen(buf), 0);
                    usleep(100000);
                }
                else if(strcmp(disc, "d")==0)
                {
                    //printf("|||%s je vidjen posl put %s|||\n", name_tmp, buf_tmp);
                    strcpy(buf, "\t * ");
                    strcat(buf, name_tmp);
                    strcat(buf, " poslednji put vidjen: ");
                    strcat(buf, buf_tmp); //iskoristio sam ip adr tmp kao timestamp
                    //printf("%s\n", buf);
                    encrypt(buf, strlen(buf));
                    send(i, buf, strlen(buf), 0);
                    usleep(100000);
                }
                else
                {
                    printf("Greska neka...\n");
                }
            }
            fclose(ac);
            sprintf(buf, "\t * broj nc botova online: %d", bot_num);
            encrypt(buf, strlen(buf));
            send(i, buf, strlen(buf), 0);
        }
        else
        {
            printf("\t[!]Nema fajla active.dat ...\n");
        }
    }
    else if(c == 1) //korisnik se sad prijavio i treba upisati u fajl active
    {

//Open the file in the mode a+
//fseek() to the start of the file
//Before reading the next line, use ftell() to note the start of the line
//Read the line
//If it's the line you want, fseek() again with the result from ftell() and use fwrite() to overwrite it.

        FILE *ac = fopen(ACTIVE_DAT, "r+");
        if(ac)
        {
            fseek(ac, 0, SEEK_SET);
            pos = ftell(ac);
            int j = 0;
            while(fscanf(ac, "%[^;];%[^;];%s\n ", name_tmp, disc, buf_tmp) != EOF)  
            {
                //printf("|||%s=?%s|||\n", name[i], name_tmp);
                if(strcmp(name[i], name_tmp)==0)
                {
                    fseek(ac, pos, SEEK_SET);//move to beginning of line 
                    time_t t = time(NULL);
                    struct tm tm = *localtime(&t);
                    //now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec
                    sprintf(buf_tmp, "%02d/%02d/%d-%02d:%02d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min);   
                    fprintf(ac, "%s;c;%s\n", name[i], buf_tmp);
                    j=1;
                }
                pos = ftell(ac);
            }
            fclose(ac);
            if(j==0)
            {
                FILE *ac = fopen(ACTIVE_DAT, "a"); 
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                //now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec
                sprintf(buf_tmp, "%02d/%02d/%d-%02d:%02d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min);   
                fprintf(ac, "%s;c;%s\n", name[i], buf_tmp);
                fclose(ac);
            }         
        }
        else
        {
            FILE *ac = fopen(ACTIVE_DAT, "w");
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            //now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec
            sprintf(buf_tmp, "%02d/%02d/%d-%02d:%02d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min);   
            fprintf(ac, "%s;c;%s\n", name[i], buf_tmp);
            fclose(ac);
        }
    }
    else if(c == 2) //korisnik se diskonektovao i treba upisati u fajl vreme napustanja
    {
        FILE *ac = fopen(ACTIVE_DAT, "r+");
        if(ac)
        {
            fseek(ac, 0, SEEK_SET);
            pos = ftell(ac);
            int j = 0;
            while(fscanf(ac, "%[^;];%[^;];%s\n ", name_tmp, disc, buf_tmp) != EOF)  
            {
                //printf("%s %s ", name[i], name_tmp);
                if(strcmp(name[i], name_tmp)==0)
                {
                    fseek(ac, pos, SEEK_SET);//move to beginning of line 
                    time_t t = time(NULL);
                    struct tm tm = *localtime(&t);
                    //now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec
                    sprintf(buf_tmp, "%02d/%02d/%d-%02d:%02d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min);   
                    fprintf(ac, "%s;d;%s\n", name[i], buf_tmp);
                    j=1;
                }
                pos = ftell(ac);
            }
            fclose(ac);
            if(j==0)
            {
                FILE *ac = fopen(ACTIVE_DAT, "a"); 
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                //now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec
                sprintf(buf_tmp, "%02d/%02d/%d-%02d:%02d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min);   
                fprintf(ac, "%s;d;%s\n", name[i], buf_tmp);
                fclose(ac);
            }         
        }
        else
        {
            FILE *ac = fopen(ACTIVE_DAT, "w");
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            //now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec
            sprintf(buf_tmp, "%02d/%02d/%d-%02d:%02d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min);   
            fprintf(ac, "%s;d;%s\n", name[i], buf_tmp);
            fclose(ac);
        }
    }
    else
    {
        ferr("Greska kod vrednosti prosledjenoj scan funkciji");
    }
}