#include iostream // pentru COUT
#include sstream
#include string
#include fstream	// pentru OFSTREAM OUTFILE
#include stdlib.h // pentru SYSTEM
#include stdio.h	// pentru SPRINTF
#include string.h // pentru BZERO
#include unistd.h // pentru Sleep
#include time.h 
#define VERSION linux_hlds_v2.8

using namespace std;

string maps = maps.txt;
const char* steamclient = /root/.steam/sdk32/steamclient.so;
string maps_list[256], _copy_map, map_name_to_verify, dns, owner, dns_winner, mode, _start_cmd, bara = ========================================================================================;
int nr_redirecte, port, maxplayers, fakeplayers, sloturi_redirect, length_of_maps_array = 0,shadow = 0, gametracker_cvar = 0, winner_drop = 0, winner_count = 0, only_winner = 0, system_reponse;
char gametracker_stealth, shadow_evidence, enable_vac, nohup, redirect_suplimentar;

void _c(string name);
bool if_file_exist(string file_name);
int read_file(string file_name);
void write_dproto(int answer_type);
void get_winner_dns();

int main()
{
	time_t timenow = time(NULL);
	get_winner_dns();

	if (!if_file_exist(maps)) 
	{ 
		cout  Cannot load file:   maps; cin.get();  return 0; 
	}
	else
	{ 
		read_file(maps); 
	}
	if (!if_file_exist(steamclient)) { system_reponse = system(mkdir /root/.steam); system_reponse = system(mkdir /root/.steam/sdk32); system_reponse = system(cp steamclient.so /root/.steam/sdk32/); }

	//copy all maps if not found
	for (int i = 0; i  length_of_maps_array; i++)
	{
		map_name_to_verify = cstrike/maps/map_default_file cstrike/maps/ + string(maps_list[i]) + .bsp;
		if (!if_file_exist(map_name_to_verify))
		{
			_copy_map = cp -u cstrike/maps/map_default_file cstrike/maps/ + string(maps_list[i]) + .bsp;
			system_reponse = system(_copy_map.c_str());
		}
	}

	_c(blue); cout  bara;
	_c(orange);  cout  endl     ctime(&amp;timenow)  endl;
	_c(0);

	if (dns_winner==)
	{
		mode = offline;
		cout   - mod redirect: ; _c(red); cout  mode;
	}
	else
	{
		mode = online;
		cout   - mod redirect: ; _c(green); cout  mode  endl;
		_c(0);  cout   - castigator concurs dropuri: ; _c(green); cout  dns_winner;
	}

	_c(0);
	cout  endl   - versiune redirect: ; _c(red); cout  VERSION; _c(0); cout   - ; _c(orange); cout  dproto.so;
	cout  endl  endl;
	_c(0);
	cout   [; _c(red); cout   ./start; _c(0); cout  \tbuild: ; _c(green); cout   31 decembrie 2018 ; _c(0); cout  ];
	cout  endl;
	_c(0);
	cout   [; _c(red); cout   hlds; _c(0); cout  \t\tbuild: ; _c(green); cout   24 Mai 2018 ; _c(0); cout        ];

	_c(blue); cout  endl  bara  endl  endl; _c(0);

	if (mode == online)
	{
		// WINNER
		cout   VREI SA DONEZI DROPURI PENTRU CONCURS ? \t (cmd: ; _c(cyan); cout  0-20; _c(0); cout  ): \t\t;
		_c(cyan);
		cin  winner_count;
		cin.ignore();

		if (winner_count = 1)
		{
			if (winner_count  20) winner_count = 20;
			winner_drop = 1;
			_c(green); cout    *** Ai ales sa donezi dropuri pentru concurs ; _c(cyan); cout  winner_drop; _c(0); cout   din ; _c(cyan); cout  winner_count  endl  endl;
			if (winner_count == 1) only_winner = 1;
		}
		else
		{
			_c(red); cout    *** Ai ales sa nu donezi nici un drop pentru concurs ; _c(cyan); cout  winner_drop; _c(0); cout  :; _c(cyan); cout  winner_count  endl  endl;
		}

		_c(0);
	}
	
	//OWNER REDIRECTE
	_c(0); cout   NUME OWNER REDIRECTE ? max 16 chars: az09-_ ; _c(0); cout  \t (cmd: ; _c(cyan); cout  alex-; _c(0); cout  ): \t\t; _c(cyan);
	getline(cin, owner);

	//DNS
	_c(0);
	if (winner_count == 1)
	{
		cout   DNS SERVER DESTINATIE DROPURI ? \t\t (cmd: ; _c(cyan); cout  cs.dns.ro; _c(0); cout  ): \t; _c(green); cout  dns_winner  endl;
		dns = dns_winner;
	}
	else
	{
		cout   DNS SERVER DESTINATIE DROPURI ? \t\t (cmd: ; _c(cyan); cout  cs.dns.ro; _c(0); cout): \t; _c(cyan);
		getline(cin, dns);
	}

	//NR REDIRECTE
	_c(0);
	cout   CATE REDIRECTE DESCHIZI ? \t\t\t (cmd: ; _c(cyan); cout  15; _c(0); cout  ): \t\t; _c(cyan);
	cin  nr_redirecte;
	cin.ignore();
	nr_redirecte = (int)nr_redirecte;
	if (nr_redirecte  64) nr_redirecte = 64;
	if (nr_redirecte  1) nr_redirecte = 1;

	//PORT
	_c(0);
	cout   PORT START REDIRECTE ? \t\t\t (cmd: ; _c(cyan); cout 29000; _c(0); cout ): \t\t; _c(cyan);
	cin  port;
	cin.ignore();
	port = (int)port;
	if (port  5000) port = 5000;
	if (port  49000) port = 49000;
	int init_port = port;

	//MAXPLAYERS
	_c(0);
	cout   NUMAR SLOTURI SERVERE REDIRECT ? \t\t (cmd: ; _c(cyan); cout  24; _c(0); cout ): \t\t; _c(cyan);
	cin  maxplayers;
	cin.ignore();
	maxplayers = (int)maxplayers;

	//FAKEPLAYERS
	_c(0);
	cout   NUMAR JUCATORI PENTRU [; _c(red); cout  maxplayers; _c(0); cout  ] SLOTURI ?    \t\t (cmd: ; _c(cyan); cout  int(maxplayers - 5); _c(0); cout  ): \t\t; _c(cyan);
	cin  fakeplayers;
	cin.ignore();
	fakeplayers = (int)fakeplayers;

	//REDIRECT 27015
	_c(0);
	cout   DESCHIDEM REDIRECT SUPLIMENTAR , PORT ; _c(red); cout  27015; _c(0); cout   ?   (cmd: ; _c(cyan); cout  y; _c(0); cout  /; _c(cyan); cout  n; _c(0); cout  ): \t\t; _c(cyan);
	cin  redirect_suplimentar;
	cin.ignore();
	if (redirect_suplimentar == 'y')
	{
		nr_redirecte++;
	}

	//SHADOW
	_c(0);
	if (winner_count == 1)
	{
		cout   EVIDENTA SHADOW(; _c(gray); cout  ***.***.***.***; _c(0); cout  ) ? \t\t (cmd: ; _c(cyan); cout  y; _c(0); cout  /; _c(cyan); cout  n; _c(0); cout  ): \t\t; _c(green); cout y  endl;
		shadow_evidence = 'y';
	}
	else
	{
		cout   EVIDENTA SHADOW(; _c(gray); cout  ***.***.***.***; _c(0); cout  ) ? \t\t (cmd: ; _c(cyan); cout  y; _c(0); cout  /; _c(cyan); cout  n; _c(0); cout  ): \t\t; _c(cyan);
		cin  shadow_evidence;
		cin.ignore();
	}
	if (shadow_evidence == 'y') shadow = 1;

	//GAMETRACKER STEALTH
	_c(0);
	cout   ENABLE; _c(purple); cout   GAMETRACKER; _c(0); cout   STEALTH ? \t\t\t (cmd: ; _c(cyan); cout  y; _c(0); cout  /; _c(cyan); cout  n; _c(0); cout  ): \t\t; _c(cyan);
	cin  gametracker_stealth;
	cin.ignore();
	if (gametracker_stealth == 'y') gametracker_cvar = 1;

	//VAC
	_c(0);
	cout   Valve Anti-Cheat ; _c(gray); cout  VAC; _c(0); cout   ? \t\t\t (cmd: ; _c(cyan); cout  y; _c(0); cout  /; _c(cyan); cout  n; _c(0); cout  ): \t\t; _c(cyan);
	cin  enable_vac;
	cin.ignore();

	//NOHUP
	_c(0);
	cout   NOHUP in loc de SCREEN ? \t\t\t (cmd: ; _c(cyan); cout  y; _c(0); cout  /; _c(cyan); cout  n; _c(0); cout  ): \t\t; _c(cyan);
	cin  nohup;
	cin.ignore();

	//WRITE CVARS
	ofstream write_redirect_configuration(cstrike/addons/amxmodx/configs/amxx.cfg);
	if (write_redirect_configuration.is_open())
	{
		write_redirect_configuration  amx_addfake   fakeplayers  \r\n;
		write_redirect_configuration  amx_fakenum   fakeplayers  \r\n;
		write_redirect_configuration  ff_delay 3  \r\n;
		write_redirect_configuration  redirect_server \  dns  \  \r\n;
		write_redirect_configuration  redirect_server_winner \  dns_winner  \\r\n;
		write_redirect_configuration  redirect_owner \  owner  \  \r\n;
		write_redirect_configuration  shadow \  shadow  \  \r\n;
		write_redirect_configuration  gametracker_stealth   gametracker_cvar  \r\n;
		write_redirect_configuration  winner_drop winner_drop \r\n;
		write_redirect_configuration  winner_count winner_count \r\n;
		write_redirect_configuration  log off;
		write_redirect_configuration.close();
	}
	else
	{
		return false;
	}

	cout  endl  endl; _c(red); cout   Pornesc [; _c(0);  cout  nr_redirecte; _c(red); cout  ] redirecte:  endl; _c(0);

	// CITESTE IP PE CARE PORNIM REDIRECT
	string linie_fisier, ip;
	ifstream fisier_ipuri;
	fisier_ipuri.open(ipuri.txt);
	// CITESTE IP PE CARE PORNIM REDIRECT

	while (getline(fisier_ipuri, linie_fisier))
	{
		port = init_port;
		ip = linie_fisier;
		
		for (int i = 0; i  nr_redirecte; i++)
		{
			if (i == (nr_redirecte - 1) &amp;&amp; redirect_suplimentar == 'y') port = 27015;

			_start_cmd = ;

			if (nohup == 'y') _start_cmd += nohup; else _start_cmd += screen -A -m -d;
			if (enable_vac == 'y') _start_cmd +=  ./hlds_run -game cstrike -secure; else _start_cmd +=  ./hlds_run -game cstrike -insecure;

			_start_cmd +=  +sv_lan 0 -noipx +ip  + ip;
			_start_cmd +=  +maxplayers  + to_string(maxplayers);
			_start_cmd +=  +port  + to_string(port);
			_start_cmd +=  +map  + maps_list[i];
			_start_cmd +=  +exec config/s+to_string(i)+.cfg;
			if (nohup == 'y') _start_cmd +=   /dev/null 2&amp;1 &amp;;

			const char *cmd_start = _start_cmd.c_str();

			string hostname_from_file;
			ifstream myfile(config/s+to_string(i)+.cfg);
			getline(myfile, hostname_from_file);
			hostname_from_file.replace(0,9,);

			_c(0);

			if (shadow_evidence == 'y')
			{
				cout  endl  \t... task: [; _c(yellow); cout  am pornit redirectul   i + 1; _c(0); cout  ]  endl;
				cout  \t\t nume server:   ; _c(cyan); cout  hostname_from_file  endl; _c(0);
				cout  \t\t adresa: \t; _c(cyan); cout  ip+:+to_string(port)  endl; _c(0);
				cout  \t\t harta: \t; _c(cyan); cout  maps_list[i]  endl; _c(0);
				cout  \t\t jucatori: \t; _c(cyan); cout  fakeplayers  /  maxplayers  endl; _c(0);
			}
			else
			{
				cout  endl  \t... task: [; _c(yellow); cout  am pornit redirectul   i + 1; _c(0); cout  ]  endl;
				cout  \t\t nume server:   ; _c(cyan); cout  hostname_from_file; cout  endl; _c(0);
				cout  \t\t adresa: \t; _c(cyan); cout  ip+:+to_string(port); cout  endl; _c(0);
				cout  \t\t harta: \t; _c(cyan); cout  maps_list[i]; cout  endl; _c(0);
				cout  \t\t jucatori: \t; _c(cyan); cout  fakeplayers  /  maxplayers; cout  endl; _c(0);
			}

			system_reponse = system(cmd_start);
			port++;
			sleep(1);
		}
	}

	_c(0);
	cout  endl  endl   Evidenta redirectelor apare in 5 minute pe: ; _c(cyan); cout  http://www.csservers.ro/evidenta/  dns; _c(0); cout  endl;
	cout  endl   Va recomandam sa vizitati si sectiunea tehnica a site-ului nostru pentru o functionare corecta a serverelor redirect:   endl; _c(cyan); cout   http://www.csservers.ro/tehnic;
	cout  endl  endl  endl; _c(0); cout   Comenzi locale: ; _c(0); cout  endl;
	cout  endl; _c(red); cout   ./stop \t\t; _c(0); cout  - oprire redirecte;
	cout  endl; _c(red); cout   ps x | grep hlds \t; _c(0); cout  - lista redirecte active;

	_c(blue); cout  endl  bara  endl; _c(0);

	return 1;
}
bool if_file_exist(string file_name)
{
	ifstream infile(file_name);
	return infile.good();
}
int read_file(string file_name)
{
	ifstream file;
	file.open(file_name);
	string line = ;

	unsigned int line_number = 0;
	while (getline(file, line))
	{
		if (string(file_name).compare(maps.txt) == 0)
		{
			maps_list[line_number] = line;
			length_of_maps_array++;
		}
		line_number++;
	}
	return 1;
}
void color(int color)
{
	switch (color)
	{
		case 5:
			cout  \e[1;35;40m;
			break;
		case 6:
			cout  \e[0;33;40m;
			break;
		case 7:
			cout  \e[0;37;40m;
			break;
		case 8:
			cout  \e[1;30;40m;
			break;
		case 9:
			cout  \e[0;34;40m;
			break;
		case 10:
			cout  \e[0;32;40m;
			break;
		case 11:
			cout  \e[0;36;40m;
			break;
		case 12:
			cout  \e[1;31;40m;
			break;
		case 14:
			cout  \e[1;33;40m;
			break;
		case 15:
			cout  \033[0m;
			break;
	}
}
void get_winner_dns()
{
	FILE *cmd = popen(wget -q -O - http://www.csservers.ro/_winner_dns.php, r);
	char result[1024];

	while (fgets(result, sizeof(result), cmd) != NULL)
	{
		stringstream ss;
		ss  result;
		dns_winner = ss.str();
	}
	pclose(cmd);
}
void _c(string name)
{
	if (name == purple) color(5);
	else if (name == orange) color(6);
	else if (name == silver) color(7);
	else if (name == gray) color(8);
	else if (name == blue) color(9);
	else if (name == green) color(10);
	else if (name == cyan) color(11);
	else if (name == red) color(12);
	else if (name == yellow) color(14);
	else color(15);
