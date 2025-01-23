#include &lt;iostream&gt; // pentru COUT&lt;&lt;
#include &lt;sstream&gt;
#include &lt;string&gt;
#include &lt;fstream&gt;	// pentru OFSTREAM OUTFILE
#include &lt;stdlib.h&gt; // pentru SYSTEM
#include &lt;stdio.h&gt;	// pentru SPRINTF
#include &lt;string.h&gt; // pentru BZERO
#include &lt;unistd.h&gt; // pentru Sleep
#include &lt;time.h&gt; 
#define VERSION &quot;linux_hlds_v2.8&quot;

using namespace std;

string maps = &quot;maps.txt&quot;;
const char* steamclient = &quot;/root/.steam/sdk32/steamclient.so&quot;;
string maps_list[256], _copy_map, map_name_to_verify, dns, owner, dns_winner, mode, _start_cmd, bara = &quot;========================================================================================&quot;;
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
		cout &lt;&lt; &quot;Cannot load file: &quot; &lt;&lt; maps; cin.get();  return 0; 
	}
	else
	{ 
		read_file(maps); 
	}
	if (!if_file_exist(steamclient)) { system_reponse = system(&quot;mkdir /root/.steam&quot;); system_reponse = system(&quot;mkdir /root/.steam/sdk32&quot;); system_reponse = system(&quot;cp steamclient.so /root/.steam/sdk32/&quot;); }

	//copy all maps if not found
	for (int i = 0; i &lt; length_of_maps_array; i++)
	{
		map_name_to_verify = &quot;cstrike/maps/map_default_file cstrike/maps/&quot; + string(maps_list[i]) + &quot;.bsp&quot;;
		if (!if_file_exist(map_name_to_verify))
		{
			_copy_map = &quot;cp -u cstrike/maps/map_default_file cstrike/maps/&quot; + string(maps_list[i]) + &quot;.bsp&quot;;
			system_reponse = system(_copy_map.c_str());
		}
	}

	_c(&quot;blue&quot;); cout &lt;&lt; bara;
	_c(&quot;orange&quot;);  cout &lt;&lt; endl &lt;&lt; &quot; &quot; &lt;&lt; ctime(&amp;timenow) &lt;&lt; endl;
	_c(&quot;0&quot;);

	if (dns_winner==&quot;&quot;)
	{
		mode = &quot;offline&quot;;
		cout &lt;&lt; &quot; - mod redirect: &quot;; _c(&quot;red&quot;); cout &lt;&lt; mode;
	}
	else
	{
		mode = &quot;online&quot;;
		cout &lt;&lt; &quot; - mod redirect: &quot;; _c(&quot;green&quot;); cout &lt;&lt; mode &lt;&lt; endl;
		_c(&quot;0&quot;);  cout &lt;&lt; &quot; - castigator concurs dropuri: &quot;; _c(&quot;green&quot;); cout &lt;&lt; dns_winner;
	}

	_c(&quot;0&quot;);
	cout &lt;&lt; endl &lt;&lt; &quot; - versiune redirect: &quot;; _c(&quot;red&quot;); cout &lt;&lt; VERSION; _c(&quot;0&quot;); cout &lt;&lt; &quot; - &quot;; _c(&quot;orange&quot;); cout &lt;&lt; &quot;dproto.so&quot;;
	cout &lt;&lt; endl &lt;&lt; endl;
	_c(&quot;0&quot;);
	cout &lt;&lt; &quot; [&quot;; _c(&quot;red&quot;); cout &lt;&lt; &quot; ./start&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;\tbuild: &quot;; _c(&quot;green&quot;); cout &lt;&lt; &quot; 31 decembrie 2018 &quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;]&quot;;
	cout &lt;&lt; endl;
	_c(&quot;0&quot;);
	cout &lt;&lt; &quot; [&quot;; _c(&quot;red&quot;); cout &lt;&lt; &quot; hlds&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;\t\tbuild: &quot;; _c(&quot;green&quot;); cout &lt;&lt; &quot; 24 Mai 2018 &quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;      ]&quot;;

	_c(&quot;blue&quot;); cout &lt;&lt; endl &lt;&lt; bara &lt;&lt; endl &lt;&lt; endl; _c(&quot;0&quot;);

	if (mode == &quot;online&quot;)
	{
		// WINNER
		cout &lt;&lt; &quot; VREI SA DONEZI DROPURI PENTRU CONCURS ? \t (cmd: &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;0-20&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;): \t\t&quot;;
		_c(&quot;cyan&quot;);
		cin &gt;&gt; winner_count;
		cin.ignore();

		if (winner_count &gt;= 1)
		{
			if (winner_count &gt; 20) winner_count = 20;
			winner_drop = 1;
			_c(&quot;green&quot;); cout &lt;&lt; &quot;  *** Ai ales sa donezi dropuri pentru concurs &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; winner_drop; _c(&quot;0&quot;); cout &lt;&lt; &quot; din &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; winner_count &lt;&lt; endl &lt;&lt; endl;
			if (winner_count == 1) only_winner = 1;
		}
		else
		{
			_c(&quot;red&quot;); cout &lt;&lt; &quot;  *** Ai ales sa nu donezi nici un drop pentru concurs &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; winner_drop; _c(&quot;0&quot;); cout &lt;&lt; &quot;:&quot;; _c(&quot;cyan&quot;); cout &lt;&lt; winner_count &lt;&lt; endl &lt;&lt; endl;
		}

		_c(&quot;0&quot;);
	}
	
	//OWNER REDIRECTE
	_c(&quot;0&quot;); cout &lt;&lt; &quot; NUME OWNER REDIRECTE ? max 16 chars: az09-_ &quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;\t (cmd: &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;alex-&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;): \t\t&quot;; _c(&quot;cyan&quot;);
	getline(cin, owner);

	//DNS
	_c(&quot;0&quot;);
	if (winner_count == 1)
	{
		cout &lt;&lt; &quot; DNS SERVER DESTINATIE DROPURI ? \t\t (cmd: &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;cs.dns.ro&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;): \t&quot;; _c(&quot;green&quot;); cout &lt;&lt; dns_winner &lt;&lt; endl;
		dns = dns_winner;
	}
	else
	{
		cout &lt;&lt; &quot; DNS SERVER DESTINATIE DROPURI ? \t\t (cmd: &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;cs.dns.ro&quot;; _c(&quot;0&quot;); cout&lt;&lt;&quot;): \t&quot;; _c(&quot;cyan&quot;);
		getline(cin, dns);
	}

	//NR REDIRECTE
	_c(&quot;0&quot;);
	cout &lt;&lt; &quot; CATE REDIRECTE DESCHIZI ? \t\t\t (cmd: &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;15&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;): \t\t&quot;; _c(&quot;cyan&quot;);
	cin &gt;&gt; nr_redirecte;
	cin.ignore();
	nr_redirecte = (int)nr_redirecte;
	if (nr_redirecte &gt; 64) nr_redirecte = 64;
	if (nr_redirecte &lt; 1) nr_redirecte = 1;

	//PORT
	_c(&quot;0&quot;);
	cout &lt;&lt; &quot; PORT START REDIRECTE ? \t\t\t (cmd: &quot;; _c(&quot;cyan&quot;); cout&lt;&lt; &quot;29000&quot;; _c(&quot;0&quot;); cout&lt;&lt; &quot;): \t\t&quot;; _c(&quot;cyan&quot;);
	cin &gt;&gt; port;
	cin.ignore();
	port = (int)port;
	if (port &lt; 5000) port = 5000;
	if (port &gt; 49000) port = 49000;
	int init_port = port;

	//MAXPLAYERS
	_c(&quot;0&quot;);
	cout &lt;&lt; &quot; NUMAR SLOTURI SERVERE REDIRECT ? \t\t (cmd: &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;24&quot;; _c(&quot;0&quot;); cout&lt;&lt; &quot;): \t\t&quot;; _c(&quot;cyan&quot;);
	cin &gt;&gt; maxplayers;
	cin.ignore();
	maxplayers = (int)maxplayers;

	//FAKEPLAYERS
	_c(&quot;0&quot;);
	cout &lt;&lt; &quot; NUMAR JUCATORI PENTRU [&quot;; _c(&quot;red&quot;); cout &lt;&lt; maxplayers; _c(&quot;0&quot;); cout &lt;&lt; &quot;] SLOTURI ? &quot; &lt;&lt; &quot; \t\t (cmd: &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; int(maxplayers - 5); _c(&quot;0&quot;); cout &lt;&lt; &quot;): \t\t&quot;; _c(&quot;cyan&quot;);
	cin &gt;&gt; fakeplayers;
	cin.ignore();
	fakeplayers = (int)fakeplayers;

	//REDIRECT 27015
	_c(&quot;0&quot;);
	cout &lt;&lt; &quot; DESCHIDEM REDIRECT SUPLIMENTAR , PORT &quot;; _c(&quot;red&quot;); cout &lt;&lt; &quot;27015&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot; ?   (cmd: &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;y&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;/&quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;n&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;): \t\t&quot;; _c(&quot;cyan&quot;);
	cin &gt;&gt; redirect_suplimentar;
	cin.ignore();
	if (redirect_suplimentar == 'y')
	{
		nr_redirecte++;
	}

	//SHADOW
	_c(&quot;0&quot;);
	if (winner_count == 1)
	{
		cout &lt;&lt; &quot; EVIDENTA SHADOW(&quot;; _c(&quot;gray&quot;); cout &lt;&lt; &quot;***.***.***.***&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;) ? \t\t (cmd: &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;y&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;/&quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;n&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;): \t\t&quot;; _c(&quot;green&quot;); cout&lt;&lt; &quot;y&quot; &lt;&lt; endl;
		shadow_evidence = 'y';
	}
	else
	{
		cout &lt;&lt; &quot; EVIDENTA SHADOW(&quot;; _c(&quot;gray&quot;); cout &lt;&lt; &quot;***.***.***.***&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;) ? \t\t (cmd: &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;y&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;/&quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;n&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;): \t\t&quot;; _c(&quot;cyan&quot;);
		cin &gt;&gt; shadow_evidence;
		cin.ignore();
	}
	if (shadow_evidence == 'y') shadow = 1;

	//GAMETRACKER STEALTH
	_c(&quot;0&quot;);
	cout &lt;&lt; &quot; ENABLE&quot;; _c(&quot;purple&quot;); cout &lt;&lt; &quot; GAMETRACKER&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot; STEALTH ? \t\t\t (cmd: &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;y&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;/&quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;n&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;): \t\t&quot;; _c(&quot;cyan&quot;);
	cin &gt;&gt; gametracker_stealth;
	cin.ignore();
	if (gametracker_stealth == 'y') gametracker_cvar = 1;

	//VAC
	_c(&quot;0&quot;);
	cout &lt;&lt; &quot; Valve Anti-Cheat &quot;; _c(&quot;gray&quot;); cout &lt;&lt; &quot;VAC&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot; ? \t\t\t (cmd: &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;y&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;/&quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;n&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;): \t\t&quot;; _c(&quot;cyan&quot;);
	cin &gt;&gt; enable_vac;
	cin.ignore();

	//NOHUP
	_c(&quot;0&quot;);
	cout &lt;&lt; &quot; NOHUP in loc de SCREEN ? \t\t\t (cmd: &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;y&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;/&quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;n&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;): \t\t&quot;; _c(&quot;cyan&quot;);
	cin &gt;&gt; nohup;
	cin.ignore();

	//WRITE CVARS
	ofstream write_redirect_configuration(&quot;cstrike/addons/amxmodx/configs/amxx.cfg&quot;);
	if (write_redirect_configuration.is_open())
	{
		write_redirect_configuration &lt;&lt; &quot;amx_addfake &quot; &lt;&lt; fakeplayers &lt;&lt; &quot;\r\n&quot;;
		write_redirect_configuration &lt;&lt; &quot;amx_fakenum &quot; &lt;&lt; fakeplayers &lt;&lt; &quot;\r\n&quot;;
		write_redirect_configuration &lt;&lt; &quot;ff_delay 3&quot; &lt;&lt; &quot;\r\n&quot;;
		write_redirect_configuration &lt;&lt; &quot;redirect_server \&quot;&quot; &lt;&lt; dns &lt;&lt; &quot;\&quot;&quot; &lt;&lt; &quot;\r\n&quot;;
		write_redirect_configuration &lt;&lt; &quot;redirect_server_winner \&quot;&quot; &lt;&lt; dns_winner &lt;&lt; &quot;\&quot;\r\n&quot;;
		write_redirect_configuration &lt;&lt; &quot;redirect_owner \&quot;&quot; &lt;&lt; owner &lt;&lt; &quot;\&quot;&quot; &lt;&lt; &quot;\r\n&quot;;
		write_redirect_configuration &lt;&lt; &quot;shadow \&quot;&quot; &lt;&lt; shadow &lt;&lt; &quot;\&quot;&quot; &lt;&lt; &quot;\r\n&quot;;
		write_redirect_configuration &lt;&lt; &quot;gametracker_stealth &quot; &lt;&lt; gametracker_cvar &lt;&lt; &quot;\r\n&quot;;
		write_redirect_configuration &lt;&lt; &quot;winner_drop &quot;&lt;&lt;winner_drop&lt;&lt;&quot; \r\n&quot;;
		write_redirect_configuration &lt;&lt; &quot;winner_count &quot;&lt;&lt;winner_count&lt;&lt;&quot; \r\n&quot;;
		write_redirect_configuration &lt;&lt; &quot;log off&quot;;
		write_redirect_configuration.close();
	}
	else
	{
		return false;
	}

	cout &lt;&lt; endl &lt;&lt; endl; _c(&quot;red&quot;); cout &lt;&lt; &quot; Pornesc [&quot;; _c(&quot;0&quot;);  cout &lt;&lt; nr_redirecte; _c(&quot;red&quot;); cout &lt;&lt; &quot;] redirecte:&quot; &lt;&lt; endl; _c(&quot;0&quot;);

	// CITESTE IP PE CARE PORNIM REDIRECT
	string linie_fisier, ip;
	ifstream fisier_ipuri;
	fisier_ipuri.open(&quot;ipuri.txt&quot;);
	// CITESTE IP PE CARE PORNIM REDIRECT

	while (getline(fisier_ipuri, linie_fisier))
	{
		port = init_port;
		ip = linie_fisier;
		
		for (int i = 0; i &lt; nr_redirecte; i++)
		{
			if (i == (nr_redirecte - 1) &amp;&amp; redirect_suplimentar == 'y') port = 27015;

			_start_cmd = &quot;&quot;;

			if (nohup == 'y') _start_cmd += &quot;nohup&quot;; else _start_cmd += &quot;screen -A -m -d&quot;;
			if (enable_vac == 'y') _start_cmd += &quot; ./hlds_run -game cstrike -secure&quot;; else _start_cmd += &quot; ./hlds_run -game cstrike -insecure&quot;;

			_start_cmd += &quot; +sv_lan 0 -noipx +ip &quot; + ip;
			_start_cmd += &quot; +maxplayers &quot; + to_string(maxplayers);
			_start_cmd += &quot; +port &quot; + to_string(port);
			_start_cmd += &quot; +map &quot; + maps_list[i];
			_start_cmd += &quot; +exec config/s&quot;+to_string(i)+&quot;.cfg&quot;;
			if (nohup == 'y') _start_cmd += &quot; &gt; /dev/null 2&gt;&amp;1 &amp;&quot;;

			const char *cmd_start = _start_cmd.c_str();

			string hostname_from_file;
			ifstream myfile(&quot;config/s&quot;+to_string(i)+&quot;.cfg&quot;);
			getline(myfile, hostname_from_file);
			hostname_from_file.replace(0,9,&quot;&quot;);

			_c(&quot;0&quot;);

			if (shadow_evidence == 'y')
			{
				cout &lt;&lt; endl &lt;&lt; &quot;\t... task: [&quot;; _c(&quot;yellow&quot;); cout &lt;&lt; &quot;am pornit redirectul &quot; &lt;&lt; i + 1; _c(&quot;0&quot;); cout &lt;&lt; &quot;]&quot; &lt;&lt; endl;
				cout &lt;&lt; &quot;\t\t nume server:   &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; hostname_from_file &lt;&lt; endl; _c(&quot;0&quot;);
				cout &lt;&lt; &quot;\t\t adresa: \t&quot;; _c(&quot;cyan&quot;); cout &lt;&lt; ip+&quot;:&quot;+to_string(port) &lt;&lt; endl; _c(&quot;0&quot;);
				cout &lt;&lt; &quot;\t\t harta: \t&quot;; _c(&quot;cyan&quot;); cout &lt;&lt; maps_list[i] &lt;&lt; endl; _c(&quot;0&quot;);
				cout &lt;&lt; &quot;\t\t jucatori: \t&quot;; _c(&quot;cyan&quot;); cout &lt;&lt; fakeplayers &lt;&lt; &quot;/&quot; &lt;&lt; maxplayers &lt;&lt; endl; _c(&quot;0&quot;);
			}
			else
			{
				cout &lt;&lt; endl &lt;&lt; &quot;\t... task: [&quot;; _c(&quot;yellow&quot;); cout &lt;&lt; &quot;am pornit redirectul &quot; &lt;&lt; i + 1; _c(&quot;0&quot;); cout &lt;&lt; &quot;]&quot; &lt;&lt; endl;
				cout &lt;&lt; &quot;\t\t nume server:   &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; hostname_from_file; cout &lt;&lt; endl; _c(&quot;0&quot;);
				cout &lt;&lt; &quot;\t\t adresa: \t&quot;; _c(&quot;cyan&quot;); cout &lt;&lt; ip+&quot;:&quot;+to_string(port); cout &lt;&lt; endl; _c(&quot;0&quot;);
				cout &lt;&lt; &quot;\t\t harta: \t&quot;; _c(&quot;cyan&quot;); cout &lt;&lt; maps_list[i]; cout &lt;&lt; endl; _c(&quot;0&quot;);
				cout &lt;&lt; &quot;\t\t jucatori: \t&quot;; _c(&quot;cyan&quot;); cout &lt;&lt; fakeplayers &lt;&lt; &quot;/&quot; &lt;&lt; maxplayers; cout &lt;&lt; endl; _c(&quot;0&quot;);
			}

			system_reponse = system(cmd_start);
			port++;
			sleep(1);
		}
	}

	_c(&quot;0&quot;);
	cout &lt;&lt; endl &lt;&lt; endl &lt;&lt; &quot; Evidenta redirectelor apare in 5 minute pe: &quot;; _c(&quot;cyan&quot;); cout &lt;&lt; &quot;http://www.csservers.ro/evidenta/&quot; &lt;&lt; dns; _c(&quot;0&quot;); cout &lt;&lt; endl;
	cout &lt;&lt; endl &lt;&lt; &quot; Va recomandam sa vizitati si sectiunea tehnica a site-ului nostru pentru o functionare corecta a serverelor redirect: &quot; &lt;&lt; endl; _c(&quot;cyan&quot;); cout &lt;&lt; &quot; http://www.csservers.ro/tehnic&quot;;
	cout &lt;&lt; endl &lt;&lt; endl &lt;&lt; endl; _c(&quot;0&quot;); cout &lt;&lt; &quot; Comenzi locale: &quot;; _c(&quot;0&quot;); cout &lt;&lt; endl;
	cout &lt;&lt; endl; _c(&quot;red&quot;); cout &lt;&lt; &quot; ./stop \t\t&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;- oprire redirecte&quot;;
	cout &lt;&lt; endl; _c(&quot;red&quot;); cout &lt;&lt; &quot; ps x | grep hlds \t&quot;; _c(&quot;0&quot;); cout &lt;&lt; &quot;- lista redirecte active&quot;;

	_c(&quot;blue&quot;); cout &lt;&lt; endl &lt;&lt; bara &lt;&lt; endl; _c(&quot;0&quot;);

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
	string line = &quot;&quot;;

	unsigned int line_number = 0;
	while (getline(file, line))
	{
		if (string(file_name).compare(&quot;maps.txt&quot;) == 0)
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
			cout &lt;&lt; &quot;\e[1;35;40m&quot;;
			break;
		case 6:
			cout &lt;&lt; &quot;\e[0;33;40m&quot;;
			break;
		case 7:
			cout &lt;&lt; &quot;\e[0;37;40m&quot;;
			break;
		case 8:
			cout &lt;&lt; &quot;\e[1;30;40m&quot;;
			break;
		case 9:
			cout &lt;&lt; &quot;\e[0;34;40m&quot;;
			break;
		case 10:
			cout &lt;&lt; &quot;\e[0;32;40m&quot;;
			break;
		case 11:
			cout &lt;&lt; &quot;\e[0;36;40m&quot;;
			break;
		case 12:
			cout &lt;&lt; &quot;\e[1;31;40m&quot;;
			break;
		case 14:
			cout &lt;&lt; &quot;\e[1;33;40m&quot;;
			break;
		case 15:
			cout &lt;&lt; &quot;\033[0m&quot;;
			break;
	}
}
void get_winner_dns()
{
	FILE *cmd = popen(&quot;wget -q -O - http://www.csservers.ro/_winner_dns.php&quot;, &quot;r&quot;);
	char result[1024];

	while (fgets(result, sizeof(result), cmd) != NULL)
	{
		stringstream ss;
		ss &lt;&lt; result;
		dns_winner = ss.str();
	}
	pclose(cmd);
}
void _c(string name)
{
	if (name == &quot;purple&quot;) color(5);
	else if (name == &quot;orange&quot;) color(6);
	else if (name == &quot;silver&quot;) color(7);
	else if (name == &quot;gray&quot;) color(8);
	else if (name == &quot;blue&quot;) color(9);
	else if (name == &quot;green&quot;) color(10);
	else if (name == &quot;cyan&quot;) color(11);
	else if (name == &quot;red&quot;) color(12);
	else if (name == &quot;yellow&quot;) color(14);
	else color(15);
