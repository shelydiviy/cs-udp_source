#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <vector>
#include <pthread.h>
#include <time.h>
#include <stdio.h>

#define SMALL_STACK 16384 //16K for threads stack-minimum

using namespace std;


// culori
char cyan[]="\033[1;36m";
char mov[]="\033[1;34m";
char red[]="\033[1;31m";
char yellow[]="\033[1;33m";
char alb[]="\033[0m";
char quote[]="\"";
// culori F

int sock;
int auto_cfg;

// variabile
char versiune[] = "winner";
char game_description[] = "Counter-Strike";
char ip[16],harta[32],hostname[64];
int port,jucatori,sloturi;
char ip_evidenta[]="188.215.51.20";

int developer = 0; // daca vine 1 prin argument printam si INPUT OUTPUT :)

char owner[32],drop[32];
// variabile F

vector<string> boti;


void steam_challange_response(sockaddr_in client);
void valve_challange_response(sockaddr_in client);


void connect(sockaddr_in client);
void server_details_valve(sockaddr_in client);
void server_details_steam(sockaddr_in client);
void players_details(sockaddr_in client);
void register_in_masters();


// functii ajutatoare
int random(int min, int max)
{
	return (int)(rand() % (max + 1 - min) + min);
}

long byteToLong(char b[])
{
	long int l;
	memcpy( &l, b, sizeof b );
	return l;
}

char *shortToByte(short a)
{
	char* where = (char*)malloc(10);
	where[0] = *((char*)(&a) + 0);
	where[1] = *((char*)(&a) + 1);
	return where;
}

void StringExplode(string str, string separator, vector<string>* results)
{
	int found;
	found = str.find_first_of(separator);
	while(found != string::npos)
	{
		if(found > 0)
		{
			results->push_back(str.substr(0,found));
		}
		str = str.substr(found+1);
		found = str.find_first_of(separator);
	}
	if(str.length() > 0)
	{
		results->push_back(str);
	}
}

string char2hex( char dec )
{
	char dig1 = (dec&0xF0)>>4;
	char dig2 = (dec&0x0F);
	if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48inascii
	if (10<= dig1 && dig1<=15) dig1+=97-10; //a,97inascii
	if ( 0<= dig2 && dig2<= 9) dig2+=48;
	if (10<= dig2 && dig2<=15) dig2+=97-10;

	string r;
	r.append( &dig1, 1);
	r.append( &dig2, 1);
	return r;
}

string urlencode(const string &c)
{
	string escaped="";
	int max = c.length();
	for(int i=0; i<max; i++)
	{
		if ( (48 <= c[i] && c[i] <= 57) ||//0-9
		(65 <= c[i] && c[i] <= 90) ||//abc...xyz
		(97 <= c[i] && c[i] <= 122)  //ABC...XYZ
		)
		{
			escaped.append( &c[i], 1);
		}
		else
		{
			escaped.append("%");
			escaped.append( char2hex(c[i]) );//converts char 255 to string "ff"
		}
	}
	return escaped;
}
// functii ajutatoare F


//evidenta CsServers.ro
void evidenta_alive()
{
	char cmd[512];

	int browser = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	struct sockaddr_in evidenta;
	evidenta.sin_family=AF_INET;
	
	evidenta.sin_port = htons(80);
	evidenta.sin_addr.s_addr = inet_addr(ip_evidenta);

	connect(browser,(sockaddr *)&evidenta,sizeof(struct sockaddr));

	std::string tq(owner);
	string cp = urlencode(tq);
	char *names = (char *)&cp[0];

	sprintf(cmd,"GET /red3/alive.php?owner=%s&ip=%s&port=%d&drop=%s&versiune=%s HTTP/1.1%c%cHost:%s%c%cConnection: close%c%c%c%c",names,ip,port,drop,versiune,'\r','\n',"www.csservers.ro",'\r','\n','\r','\n','\r','\n');
	send(browser,cmd,strlen(cmd),0);
	close(browser);
}

void evidenta_new(char *jucator,char ip_jucator[15])
{
	char cmd[512];

	int browser = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	struct sockaddr_in evidenta;
	evidenta.sin_family=AF_INET;
	
	evidenta.sin_port = htons(80);
	evidenta.sin_addr.s_addr = inet_addr(ip_evidenta);

	connect(browser,(sockaddr *)&evidenta,sizeof(struct sockaddr));

	std::string tq(owner);
	string cp = urlencode(tq);
	char *names = (char *)&cp[0];

	std::string tq1(jucator);
	string cp1 = urlencode(tq1);
	char *namesc = (char *)&cp1[0];

	sprintf(cmd,"GET /red3/new.php?owner=%s&ip=%s&port=%d&drop=%s&jucator=%s&ip_jucator=%s&versiune=%s HTTP/1.1%c%cHost:%s%c%cConnection: close%c%c%c%c",names,ip,port,drop,namesc,ip_jucator,versiune,'\r','\n',"www.csservers.ro",'\r','\n','\r','\n','\r','\n');
	send(browser,cmd,strlen(cmd),0);
	close(browser);
}
//evidenta CsServers.ro


void finalizare_register_in_master(unsigned long challenge,struct sockaddr_in master)
{
	char query[2];
	bzero(&query,strlen(query));
	query[0]=0x30;
	query[1]=0x0a;
	char query2[348];

	strcpy(query2,"\\protocol\\7");
	// sprintf(query2,"%s%s",query2,"\\protocol\\7");
	char c[50];
	sprintf(c,"\\challenge\\%ld",challenge);
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\players\\%d",jucatori);
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\max\\%d",sloturi);
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\bots\\%d",0);
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\gamedir\\cstrike");
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\map\\%s",harta);
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\password\\0");
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\os\\l");
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\lan\\0");
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\region\\255");
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\gameport\\%d",port);
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\specport\\0");
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\appid\\10");
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\type\\d");
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\secure\\0");
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\version\\1.1.2.6");
	sprintf(query2,"%s%s",query2,c);
	sprintf(c,"\\product\\cstrike");
	sprintf(query2,"%s%s",query2,c);
	char gsd[]={0x0a,0x00};
	char fin[2+strlen(query2)+2];
	memcpy(fin,query,2);
	strcpy(fin+2,query2);
	memcpy(fin+2+strlen(query2),gsd,2);  

	sendto(sock,fin,sizeof(fin),0,(struct sockaddr *)&master,(unsigned int)sizeof(sockaddr));
}

void *evidenta_si_mastere(void *c)
{
	while(1)
	{
		register_in_masters();
		evidenta_alive();
		sleep(300);
	}
}


int main(int argc, char *argv[])
{
	// argumente
	strcpy(hostname,argv[1]);
	strcpy(ip,argv[2]);
	port = atoi(argv[3]);
	strcpy(harta,argv[4]);
	jucatori = atoi(argv[5]);
	sloturi = atoi(argv[6]);
	strcpy(owner,argv[7]);
	strcpy(drop,argv[8]);
	
	if(argv[9])
	developer = atoi(argv[9]);
	// argumente 

	
	// trebuie sa citim botii din fisier
	ifstream fisier_boti("boti.cfg");
	for(int i=0;i<32;i++)
	{
		char nume_bot[50];
		fisier_boti.getline(nume_bot,50);
		boti.push_back(nume_bot);
	}
	fisier_boti.close();
	// trebuie sa citim botii din fisier F

	if(developer==1)
	{
	cout<<endl
		<<yellow<<" [START REDIRECT SERVER] "<<endl<<endl
		<<red<<" Hostname: "<<alb<<hostname<<endl
		<<red<<" IP: "<<alb<<ip<<endl
		<<red<<" Port: "<<alb<<port<<endl
		<<red<<" Harta: "<<alb<<harta<<endl
		<<red<<" Jucatori: "<<alb<<jucatori<<endl
		<<red<<" Sloturi: "<<alb<<sloturi<<endl
		<<red<<" Owner: "<<alb<<owner<<endl
		<<red<<" Drop2: "<<alb<<drop<<endl;
	}

	struct sockaddr_in client;
	struct sockaddr_in server;

	char request[2048];

	sock = socket(AF_INET,SOCK_DGRAM,0);

	bzero(&server,sizeof(server));
	
	server.sin_family=AF_INET;

	server.sin_addr.s_addr=inet_addr(ip);
	server.sin_port = htons(port);

	bind(sock,(sockaddr *)&server,sizeof(server));

	unsigned int client_length = sizeof(struct sockaddr_in);


	// pthread
	pthread_t changer;

	pthread_attr_t thread_attr;
	pthread_attr_init(&thread_attr);
	pthread_attr_setstacksize(&thread_attr, SMALL_STACK);

	pthread_t registerer;
	int x2;

	x2=pthread_create(&registerer,&thread_attr,evidenta_si_mastere,NULL);
	
	// pthread F

	evidenta_alive();

	while(1)
	{
		if(recvfrom(sock,request,2048,0,(struct sockaddr *)&client,&client_length))
		{
			if(developer==1)
			{
				cout<<endl<<mov<<" <- "<<alb<<"["<<mov<<inet_ntoa(client.sin_addr)<<alb<<"]: "<<request;
			}

			char client_ip[15];

			sprintf(client_ip,"%s",inet_ntoa(client.sin_addr));

			if(request[17]=='s' && request[18]=='t' && request[19]=='e') // steam
			{
				steam_challange_response(client);
			}
	
			if(request[17]=='v' && request[18]=='a' && request[19]=='l') // valve
			{
				valve_challange_response(client);
			}

			if(request[4]=='c' && request[5]=='o' && request[6]=='n' && request[7]=='n') // connect 47/48
			{
				if(request[12]=='4' && request[13]=='7') // protocol 47
				{
					// e acelasi connect pt ambii
				}
				if(request[12]=='4' && request[13]=='8') // protocol 48
				{
					// e acelasi connect pt ambii	
				}
				
	
				char *name_ptr = strstr(request,"name\\");
				char n2[500];
				strcpy(n2,name_ptr+5);

				//  char name[strlen(n2)-strlen(rr)+1];
				char ns[50];
				memset(ns,0x00,sizeof(ns));
				int x=0;
				while(n2[x]!='"'&&n2[x]!='\\'&&x<strlen(n2))
				{
					ns[x]=n2[x];
					x++;
				}

			
				evidenta_new(ns,client_ip);
				
				connect(client);
				
			}

			if(request[4]=='d' && request[5]=='e' && request[6]=='t' && request[7]=='a' && request[8]=='i' && request[9]=='l' && request[10]=='s') // server details VALVE
			{
				server_details_valve(client);
			}
			
			
			// ÿÿÿÿUÿÿÿÿce Engine Query
			if(request[4]=='U') // players details  
			{
				players_details(client);
			}

			// ÿÿÿÿTSource Engine Query
			if(request[4]=='T' && request[5]=='S' && request[6]=='o' && request[7]=='u' && request[8]=='r' && request[9]=='c' && request[10]=='e') // server details STEAM (TSource Engine Query)
			{
				server_details_steam(client);
			}

			


			if(request[4]=='N') // register in master server
			{
			}

			if(request[4]=='R')
			{
				//eroarea de la master: UDP heartbeats are no longer supported.  Please update your server..
			}

			if(request[4]=='s')
			{
				char challenge[4];
				challenge[0]=request[6];
				challenge[1]=request[7];
				challenge[2]=request[8];
				challenge[3]=request[9];
				
				//printam challenge
				if(developer==1)
				{
					cout<<endl<<endl<<red<<" -> "<<alb<<"["<<red<<inet_ntoa(client.sin_addr)<<alb<<"]: "<<byteToLong(challenge)<<endl;
				}

				finalizare_register_in_master(byteToLong(challenge),client);
			}
		}

		sleep(0.7);
	}

	pthread_detach( registerer );
	if(auto_cfg==1)
	pthread_detach( changer );

	return 0;
}

void steam_challange_response(sockaddr_in client)
{
	char cmd[] = { 0xff, 0xff, 0xff, 0xff, 'A', '0' , '0' , '0' , '0' , '0' , '0' , '0' , '0' , 0x20, '2', '3', '6', '2', '9', '6', '1', '4', '3', '8', 0x20, '3', 0x20, '1', 'm', 0x20, '3' };
	sendto(sock,cmd,sizeof(cmd),0,(struct sockaddr *)&client,(unsigned int)sizeof(sockaddr));
}

void valve_challange_response(sockaddr_in client)
{
	char cmd[] = { 0xff, 0xff, 0xff, 0xff, 'A', '0' , '0' , '0' , '0' , '0' , '0' , '0' , '0' , 0x20, '5', '6', '9', '3', '3', '1', '0', '8', '6', 0x20, '2' };
	sendto(sock,cmd,sizeof(cmd),0,(struct sockaddr *)&client,(unsigned int)sizeof(sockaddr));
}

void connect(sockaddr_in client)
{
	char cmd[50];
	memset(cmd,0x00,sizeof(cmd));

	char start[] = { 0xff, 0xff, 0xff, 0xff, 'L', 0x20 };
	char stop[] = { 0x22, 0x20, 0x31, 0x00 };
	
	memcpy(cmd,start,sizeof(start));
	memcpy(cmd+sizeof(start),drop,sizeof(drop));
	memcpy(cmd+(sizeof(start)+sizeof(drop)),stop,sizeof(stop));

	sendto(sock,cmd,sizeof(cmd),0,(struct sockaddr *)&client,(unsigned int)sizeof(sockaddr));
}

void server_details_valve(sockaddr_in client)
{
	char query[5];
	char num[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1c,0x1d,0x1e,0x1f,0x20};
	bzero(&query,strlen(query));
	query[0]=0xff;
	query[1]=0xff;
	query[2]=0xff;
	query[3]=0xff;
	query[4]=0x6d;
	char cmd[200];
	int len=0;
	bzero(&cmd,strlen(cmd));
	memcpy(cmd,query,5);
	len+=5;
	char buf[100];
	sprintf(buf,"127.0.0.1:%d",port);
	memcpy(cmd+len,buf,strlen(buf));
	len+=strlen(buf);
	char sp6[1]={0x00};
	memcpy(cmd+len,sp6,1);
	len+=1;
	char sp5[2]={0x01,0x01};
	memcpy(cmd+len,sp5,2);
	len+=2;
	sprintf(buf,"%s",hostname);
	memcpy(cmd+len,buf,strlen(buf));
	len+=strlen(buf);
	memcpy(cmd+len,sp6,1);
	len+=1;
	sprintf(buf,"%s",harta);
	memcpy(cmd+len,buf,strlen(buf));
	len+=strlen(buf);
	memcpy(cmd+len,sp6,1);
	len+=1;
	sprintf(buf,"%s","cstrike");
	memcpy(cmd+len,buf,strlen(buf));
	len+=strlen(buf);
	memcpy(cmd+len,sp6,1);
	len+=1;
	sprintf(buf,"%s",game_description);
	memcpy(cmd+len,buf,strlen(buf));
	len+=strlen(buf);
	// free(sp2);
	char sp3[23]={0x00,num[jucatori],num[sloturi],0x2f,0x64,0x6c,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,0x00};
	memcpy(cmd+len,sp3,23);
	len+=23;

	sendto(sock,cmd,sizeof(cmd),0,(struct sockaddr *)&client,(unsigned int)sizeof(sockaddr));
}

void server_details_steam(sockaddr_in client)
{
	char query[6];
	char num[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1c,0x1d,0x1e,0x1f,0x20};
	bzero(&query,strlen(query));
	query[0]=0xff;
	query[1]=0xff;
	query[2]=0xff;
	query[3]=0xff;
	query[4]=0x49;
	query[5]=0x30;
	char cmd[348];
	bzero(&cmd,strlen(cmd));
	int len=0;
	memcpy(cmd+len,query,6);
	len+=6;
	//char sp[2]= {0xc2,0x80};
	char sp[2]= {0x01,0x01}; 
	memcpy(cmd+len,sp,2);
	len+=2;
	char buf[100];
	sprintf(buf,"%s",hostname);
	memcpy(cmd+len,buf,strlen(buf));
	len+=strlen(buf);
	char sp2[1]={0x00};
	memcpy(cmd+len,sp2,1);
	len+=1;
	sprintf(buf,"%s",harta);
	memcpy(cmd+len,buf,strlen(buf));
	len+=strlen(buf);
	memcpy(cmd+len,sp2,1);
	len+=1;
	sprintf(buf,"%s","cstrike");
	memcpy(cmd+len,buf,strlen(buf));
	len+=strlen(buf);
	memcpy(cmd+len,sp2,1);
	len+=1;
	sprintf(buf,"%s",game_description);
	memcpy(cmd+len,buf,strlen(buf));
	len+=strlen(buf);
	memcpy(cmd+len,sp2,1);
	len+=1;
	char sp3[]={0x0a,0x00,num[jucatori],num[sloturi],0x00,0x64,0x6c,0x00,0x01};
	memcpy(cmd+len,sp3,sizeof(sp3));
	len+=sizeof(sp3);
	sprintf(buf,"%s","1.1.2.6/Stdio");
	memcpy(cmd+len,buf,strlen(buf));
	len+=strlen(buf);
	memcpy(cmd+len,sp2,1);
	len+=1;
	char sp4[1]={0x80};
	memcpy(cmd+len,sp4,sizeof(sp4));
	len+=sizeof(sp4);
	char *par = shortToByte((short)port);
	memcpy(cmd+len,par,2);
	len+=2;
	
	sendto(sock,cmd,sizeof(cmd),0,(struct sockaddr *)&client,(unsigned int)sizeof(sockaddr));
}

void players_details(sockaddr_in client)
{
	char timp = 0x41;
	char frag = random(1,115);

	char cmd[] = { 
		0xff, 0xff, 0xff, 0xff, 0x44, 0x1d, 0x00, 

		'a', 'l', 'e', 'x',				0x00, frag, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 'A', 0x00, 
		'g', 'i', 'g', 'i', 'z', '!',	0x00, frag, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 'B', 0x00, 	
		'p', 'u', 'l', 'a',				0x00, frag, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 'C', 0x00,
		'm', 'a', 't', 'a',				0x00, frag, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 'D', 0x00,
		'p', 'i', 'z', 'd', 'a',		0x00, frag, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 'E', 0x00
	};

	sendto(sock,cmd,sizeof(cmd),0,(struct sockaddr *)&client,(unsigned int)sizeof(sockaddr));
}

void register_in_masters()
{
	const int nr_mastere = 37;

	char masters[nr_mastere][32] = {
		"31.28.166.117:27010",
		"31.28.170.123:27010",	// 2.1
		//"37.143.8.94:27010",    // 2.1
		"46.4.71.67:27010",
		"46.38.57.227:27010",	// 2.1
		"46.38.63.6:27010",		// 2.1
		"46.149.46.168:27010",	// 2.1
		"46.165.194.14:27010",
		"46.165.194.16:27010",
		"46.165.194.225:27010",
		"46.254.21.202:27010",	// 2.1
		"62.109.30.108:27010",	// 2.1
		"62.109.30.108:27011",  // 2.1
		//"69.28.158.131:27014",  // 2.1
		"78.46.63.214:27010",
		"78.47.20.140:27010",	// 2.1
		"78.47.85.132:27010",	// 2.1
		"78.47.189.126:28010",  // 2.1
		"78.47.189.126:29012",	// 2.1
		"83.243.73.222:27010",	// 2.1
		"85.10.231.175:27010",
		"89.20.147.157:27010",	// 2.1
		"89.28.42.226:27010",   // 2.1
		"89.108.101.63:27015",  // 2.1
		"89.137.241.79:27010",
		"89.137.241.79:27011",	// 2.1
		//"91.150.72.233:27010",	// 2.1
		"95.211.153.82:27010",	// 2.1
		"92.243.95.195:27010",
		"95.211.148.40:27010",	// 2.1
		"176.9.50.13:27010",
		"178.63.65.159:27010",	// 2.1
		"178.236.137.96:27010",
		"188.40.40.201:27010",
		"188.120.245.247:27010",	// 2.1
		"188.127.232.88:27010",		// 2.1
		//"194.106.162.211:27011",	// 2.1
		"208.64.200.39:27011",
		"208.64.200.52:27011",
		"208.64.200.65:27015",
		"208.64.200.117:27011"
		//"208.64.200.118:27011"	
		};

	for(int i=0;i<nr_mastere;i++)
	{
		struct sockaddr_in master;
		master.sin_family=AF_INET;

		vector<string> parts;
		StringExplode(masters[i],":",&parts);
		bzero(&master,sizeof(master));
		
		master.sin_port = htons(atoi((char *)&parts[1][0]));
		master.sin_addr.s_addr = inet_addr((char *)&parts[0][0]);


		char cmd_start[] = { 0x4d, 0xff };
		if(sendto(sock,cmd_start,sizeof(cmd_start),0,(struct sockaddr *)&master,(unsigned int)sizeof(sockaddr))<0)
		{
			cout<<endl<<"nu am putut inscrie serverul in master server"<<endl;
		}

		char cmd_stop[] = { 0x71 };
		sendto(sock,cmd_stop,sizeof(cmd_stop),0,(struct sockaddr *)&master,(unsigned int)sizeof(sockaddr));
	}
}
