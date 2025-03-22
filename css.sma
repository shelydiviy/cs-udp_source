#include <amxmodx>
#include <amxmisc>
#include <sockets>
#include <cstrike>
#include <fun>
#include <fakemeta>

#pragma dynamic 32768

#define SERVER_INFO_HOST "www.csservers.ro"
#define REDIRECT_SHORT_VERSION "v2.8"
#define file "../maps.txt"

new _css_debug = 0;
new cvar_fakenum;
new Array: maps_array;
new nr_maps_found;
new map_name_line[64];

stock remove_logs() 
{
    new CurrentDir[128], CurrentFileName[128];
    get_localinfo("amxx_logs", CurrentDir, charsmax(CurrentDir));
    new HandleDir = open_dir(CurrentDir, CurrentFileName, charsmax(CurrentFileName));

    if(HandleDir) 
    { 
        while(next_file(HandleDir, CurrentFileName, charsmax(CurrentFileName)))
        { 
            format(CurrentFileName, charsmax(CurrentFileName), "%s/%s", CurrentDir, CurrentFileName);
            delete_file(CurrentFileName);
        } 
        
        close_dir(HandleDir);
        rmdir(CurrentDir);
    } 
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

	cvar_fakenum = register_cvar("amx_fakenum","");
	
	register_cvar("gametracker_stealth","");
	
	if(file_exists(file))
	{
		server_print("[%s][OK] maps loaded ^"%s^"",get_server_time(),file);
	}
	else
	{
		server_print("[%s][ERROR] maps not loaded ^"%s^" - file missing or damaged",get_server_time(),file);
	}
	
	new line_from_file[64],itextlength,i,map_found;
	
	maps_array = ArrayCreate(64);
	
	do
	{
		map_found = read_file(file,i,line_from_file,1024,itextlength);
		i++;
		ArrayPushString( maps_array, line_from_file[0]);
		
	}while(map_found);
	
	nr_maps_found = i;
	
	set_task(float(23),"css_kick_add_bots", 101, _, _, "b");
	set_task(float(2700),"css_changelevel", 102, _, _, "b"); //2700 <- default
	set_task(float(45),"css_set_frags", _, _, _, "b");
}
public kick_random_players()
{
	new players[32],num,kick_id;
	get_players(players, num,"d");

	for(new i=0;i<num;i++)
	{
		kick_id = get_user_userid(players[i]);
		new prob = random(10);
		if(prob<1)
		{
			server_cmd("kick #%d",kick_id);
		}
	}
}
public css_kick_add_bots()
{
	if(_css_debug==2)
	{
		server_print("am intrat in css_kick_add_bots");
	}
	
	new gametracker_stealth = get_cvar_num("gametracker_stealth");
	if(gametracker_stealth==1)
	{
		new players[32],num,fakenum;
		fakenum = get_pcvar_num(cvar_fakenum);
		get_players(players, num,"d");
		
		if(num<=fakenum)
		{
			for(new z=0;z<random_num(1,3);z++)
			{
				server_cmd("amx_addfake 1");
			}
		}
		
		if(num>fakenum)
		{
			kick_random_players();
		}
	}
	else
	{
		if(_css_debug==2)
		{
			server_print("am dat REMOVE la TASK in css_kick_add_bots");
		}
		remove_task(101);
	}
}
public css_changelevel()
{
	if(_css_debug==2)
	{
		server_print("am intrat in css_changelevel");
	}
	new gametracker_stealth = get_cvar_num("gametracker_stealth");
	
	if(gametracker_stealth==1)
	{
		ArrayGetString( maps_array, random(nr_maps_found), map_name_line, charsmax( map_name_line ) );
		server_cmd("changelevel %s",map_name_line);
		/*    LINUX STYLE! /     */
		//unlink("addons\amxmodx\logs\*.log");

		remove_logs();
	}
	else
	{
		if(_css_debug==2)
		{
			server_print("am dat REMOVE la TASK in css_changelevel");
		}
		remove_task(102);
	}
}
public css_set_frags()
{
	new Players[32];
	new playerCount, player_id;
	get_players(Players, playerCount, "i");
	for (new i=0; i<playerCount; i++)
	{
		player_id = Players[i];
		set_user_frags(player_id,random(50));
		cs_set_user_deaths(player_id,random(50));
	}
}
