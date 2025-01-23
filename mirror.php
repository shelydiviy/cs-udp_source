?php
// fisierul 1
set_time_limit(0);


$ip = &quot;93.115.249.200&quot;;
$port = &quot;40000&quot;;

$socket = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);

socket_bind($socket,$ip,$port);

$from = '';
$port = 0;
$packet = &quot;\xFF\xFF\xFF\xFFLcs.pgl.ro:27015&quot;;

do
{

socket_recvfrom($socket, $buf, 2046, 0, $from, $port);



echo &quot;\nReceived $buf from remote address $from and remote port $port &quot;.date(&quot;G:i:s&quot;).&quot;\n&quot;;

switch(trim($buf[4]))
{
	case &quot;T&quot;: //parse info server string
		//TODO - answertype 0
		//TODO - answertype 1

		$host_name = &quot;redirectPHP&quot;;
		$map = &quot;de_php&quot;;
		$hlversion = &quot;1.1.2.6/Stdio&quot;;

		$primul_pachet = &quot;0xFFFFFFFF&quot;;

		$de_trimis = &quot;\xFF\xFF\xFF\xFF\x49\x30\x50\x47\x4C\x20\x50\x75\x62\x6C\x69\x63\x20\x53\x65\x72\x76\x65\x72\x20\x43\x53\x20\x31\x2E\x36\x00\x64\x65\x5F\x69\x6E\x66\x65\x72\x6E\x6F\x00\x63&quot;.
		&quot;\x73\x74\x72\x69\x6B\x65\x00\x43\x6F\x75\x6E\x74\x65\x72\x2D\x53\x74\x72\x69\x6B\x65\x00\x0A\x00\x15\x16\x00\x64\x6C\x00\x01\x31\x2E\x31\x2E\x32\x2E\x36\x2F\x53\x74\x64\x69\x6F\x00\x91\x87&quot;.
		&quot;\x69\x02\x2C\x26\x52\x37\x0B\x40\x01\x0A\x00\x00\x00\x00\x00\x00\x00&quot;;

		socket_sendto($socket, $de_trimis, strlen($de_trimis), 0, $from, $port);

		break;
	case 'd': //parse info server string OLD STYLE
		//TODO - answertype 2
		break;

	case 'p': //parse players info OLD STYLE
	case 'U': //parse players info
		//TODO
		$de_trimis = &quot;\xFF\xFF\xFF\xFF\x44\x15\x00\x53\x68\x61\x64\x6F\x77\x4B\x69\x6C\x6C\x65\x72\x00\x13\x00\x00\x00\x93\xF9\xAB\x45\x00\x49\x63\x53\x00\x0A\x00\x00\x00\x1F\x12\x90\x45\x00\x4E&quot;.
		&quot;\x6F\x7A\x7A\x00\x0E\x00\x00\x00\xF7\x81\x03\x45\x00\x49\x4E\x53\x00\x18\x00\x00\x00\x8C\xFB\xD5\x44\x00\x50\x61\x63\x45\x73\x74\x69\x4D\x6F\x72\x74\x00\x01\x00\x00\x00\x13\x3B\xB2\x44\x00&quot;.
		&quot;\x52\x6F\x4D\x61\x52\x69\x4F\x00\x23\x00\x00\x00\x99\x2E\xA7\x44\x00\x53\x75\x73\x68\x69\x00\x0A\x00\x00\x00\x5D\x16\x82\x44\x00\x67\x69\x67\x69\x68\x00\x0A\x00\x00\x00\x12\x03\x6A\x44\x00&quot;.
		&quot;\x4D\x65\x74\x68\x61\x6E\x65\x00\x09\x00\x00\x00\xD1\xCB\x42\x44\x00\x66\x61\x76\x73\x00\x05\x00\x00\x00\xA8\x58\x23\x44\x00\x63\x72\x61\x70\x00\x05\x00\x00\x00\x7D\x86\xDF\x43\x00\x57\x65&quot;.
		&quot;\x74\x20\x46\x69\x6E\x67\x65\x72\x73\x00\x05\x00\x00\x00\x0C\x75\xDF\x43\x00\x50\x52\x49\x4E\x43\x45\x24\x24\x3A\x3A\x3A\x3A\x00\x03\x00\x00\x00\x71\xD8\xD5\x43\x00\x6C\x6D\x66\x61\x6F\x00&quot;.
		&quot;\x04\x00\x00\x00\xC5\x4E\xB2\x43\x00\x6C\x6F\x77\x00\x03\x00\x00\x00\x9F\x74\x8B\x43\x00\x7A\x75\x72\x4C\x69\x61\x00\x00\x00\x00\x00\xB9\x44\x05\x43\x00\x70\x49\x52\x49\x00\x01\x00\x00\x00&quot;.
		&quot;\x50\xDF\xDA\x42\x00\x53\x6B\x79\x72\x69\x6D\x4D\x6D\x00\x00\x00\x00\x00\x6E\x37\xD3\x42\x00\x54\x75\x44\x79\x00\x00\x00\x00\x00\x85\xBB\xA5\x42\x00\x72\x69\x70\x63\x6C\x69\x63\x6B\x73\x74&quot;.
		&quot;\x61\x6E\x67\x61\x00\x00\x00\x00\x00\x8D\x24\x9D\x42\x00\x76\x31\x73\x33\x7A\x5A\x00\x01\x00\x00\x00\xDB\xBA\x00\x42&quot;;

		socket_sendto($socket, $de_trimis, strlen($de_trimis), 0, $from, $port);

		break;
	case 'V': //parse rules
		//TODO
		break;
	case 'c':  //parse connect string
		//TODO
		break;

	case 'g': //parse getchallenge buf
		//TODO
		break;

	default:
		echo &quot;aici a ajuns defapt&quot;;
		break;

}

}while($x=1);




// fisierul 2

set_time_limit(0);

function bintohex($str) {
    $hex = &quot;&quot;;
    $i = 0;
    do {
        $hex .= &quot;\x&quot;.strtoupper(bin2hex($str[$i]));
        $i++;
    } while ($i &lt; strlen($str));
    return $hex;
}

function GetChallenge ($port,$buf,$len) {
	$cbuf = '';
	$ctype = 0;
	if($len &lt; 16) return;
	if(strncmp(substr($buf,4,12), &quot;getchallenge&quot;, 12)) return;
	$buf[$len] = 0;
		if(!strcmp(substr($buf,17,5), &quot;valve&quot;)) {
			$ctype = 2;
		}elseif (!strcmp(substr($buf,17,5), &quot;steam&quot;)) {
			$ctype = 3;
		}else return;
			$cbuf.= &quot;\xFF\xFF\xFF\xFFA 00000000 &quot;.(rand() | (rand() &lt;&lt; 16) &amp; 0x7FFFFFFF).&quot; $ctype&quot;;

		switch($ctype){
				case 2:
					$cbuf.= &quot;\n&quot;;
					break;
				case 3:
					$cbuf.= &quot;1m 30819d300d06092a864886f70d010101050003818b0030818702818100b5a614e896036cc9f9bd6d13f2f5c79fbb5f925e8dbb50f0b9ee9a5499f535978fe60c188e4f8872160d86b76b80f1ba82333d586b&quot;.
					&quot;32692ffa31e1dd59a603dc6370004566afa54830898d4ff210c738deb059e0a94a87dd85be28668793681a4ecf647fa1b5294a73927f23ffba0c6a9140922d27002012fed2b4a898aa7811020111&quot;;
					break;
			}
			return $cbuf;
}

function ParseConnect($buf,$len)
{

	/*
	char* buf, int len, int port, SOCKET name_sock,sockaddr_in server_addr) {
	char* cbuf = buf + 4;
	char SBuf[1500];
	in_addr ina;
	int res;
	if (len &lt; 11) return;
	if (strncmp(cbuf, &quot;connect&quot;, 7)) return;
	cbuf = SBuf;
	res = sprintf(cbuf, &quot;%c%c%c%c%c %d \&quot;%s:%d\&quot; %d&quot;, 0xFF, 0xFF, 0xFF, 0xFF, 'B', 1, inet_ntoa(server_addr.sin_addr), port, 0);
	cbuf += res;

	res = cbuf - SBuf;
	sendto(name_sock,(const char *)SBuf, res, 0,(sockaddr  *)  &amp;server_addr, sizeof(server_addr));

	cbuf = SBuf;
	res = sprintf(cbuf, &quot;%c%c%c%c%c%c%c%c&quot;, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0);
	cbuf += res;
	res = sprintf(cbuf, &quot;%cecho \&quot;* www.csservers.ro \&quot;;echo;echo;echo \&quot;* ok \&quot;; %s\n&quot;, 0x9, &quot;Connect cs.pgl.ro:27015&quot;);
	cbuf += res + 1;

	res = cbuf - SBuf;
	while (res % 4) {
		SBuf[res++] = 1;
	}
	Munge2((int*)(SBuf+8), res-8, 0);
	//SendData(SBuf, res, ip, port);
	sendto(name_sock,(const char *)SBuf, res, 0,(sockaddr  *)  &amp;server_addr, sizeof(server_addr));
*/
	$cbuf = '';
	if($len&lt;11) return;
		$cbuf.=sprintf(&quot;%c%c%c%c%c%c%c%c%cecho \&quot;* www.csservers.ro \&quot;;echo;echo;echo \&quot;* ok\&quot;; %s\n&quot;, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x9, &quot;Connect cs.pgl.ro:27015&quot;);
			var_dump($cbuf);
	return;

}

function BuildMSInfo($challenge)
{
	return &quot;0\n\\protocol\\47\\challenge\\$challenge\\players\\22\\max\\32\\bots\\0\\gamedir\\cstrike\\map\\de_dust2\\type\\d\\password\\0\\os\\l\\secure\\0\\lan\\0\\version\\%s\\region\\255\\product\\valve\n&quot;;
}

function getInfo($ip,$port)
{
	$fp	= fsockopen(&quot;udp://$ip&quot;,$port,$errno,$errstr,1);
	stream_set_timeout($fp,1);
	fwrite($fp,&quot;\xFF\xFF\xFF\xFFTSource Engine Query\x00&quot;);
	$getinfo = fread($fp,65536);
	fclose($fp);
	return $getinfo;
	unset($getinfo);
}
	function getPlayers($ip,$port)
{
	$q_player = &quot;\xFF\xFF\xFF\xFF\x55&quot;;
	$fp	= fsockopen(&quot;udp://$ip&quot;,$port,$errno,$errstr,1);
	stream_set_timeout($fp,1);
	fwrite($fp,&quot;\xFF\xFF\xFF\xFF\x55\xFF\xFF\xFF\xFF&quot;);
	$getinfo = substr(fread($fp,65536),5,4);
	fwrite($fp,$q_player.$getinfo);
	$players = fread($fp,2046);
	fclose($fp);
	return $players;
	unset($players);
}
function getRules($ip,$port)
{
	$q_rules  = &quot;\xFF\xFF\xFF\xFF\x56&quot;;
	$fp	= fsockopen(&quot;udp://$ip&quot;,$port,$errno,$errstr,1);
	stream_set_timeout($fp,1);
	fwrite($fp,&quot;\xFF\xFF\xFF\xFF\x55\xFF\xFF\xFF\xFF&quot;);
	$getinfo = substr(fread($fp,65536),5,4);
	fwrite($fp,$q_rules.$getinfo);
	$rules= fread($fp,2046);
	fclose($fp);
	return $rules;
	unset($rules);
}


/* random 32 players times ) */
		$t_32 = array(
			rand(0.0,1000.5), rand(0.0,900.5), rand(0.0,800.5), rand(0.0,900.5),
			rand(0.0,900.5),  rand(0.0,900.5), rand(0.0,900.5), rand(0.0,200.5),
			rand(0.0,900.5),  rand(0.0,600.5), rand(0.0,400.5), rand(0.0,100.5),
			rand(0.0,900.5),  rand(0.0,900.5), rand(0.0,300.5), rand(0.0,200.5),
			rand(0.0,400.5),  rand(0.0,550.5), rand(0.0,600.5), rand(0.0,400.5),
			rand(0.0,1200.5), rand(0.0,900.5), rand(0.0,800.5), rand(0.0,800.5),
			rand(0.0,300.5),  rand(0.0,900.5), rand(0.0,700.5), rand(0.0,900.5),
			rand(0.0,400.5),  rand(0.0,100.5), rand(0.0,500.5), rand(0.0,900.5)
		);
		$t_32_0 = array(
			rand(0.0,12.5), rand(0.0,21.5), rand(0.0,11.5), rand(0.0,9.5),
			rand(0.0,2.5),  rand(0.0,4.5),  rand(0.0,21.5), rand(0.0,7.5),
			rand(0.0,3.5),  rand(0.0,5.5),  rand(0.0,6.5),  rand(0.0,9.5),
			rand(0.0,1.5),  rand(0.0,4.5),  rand(0.0,2.5),  rand(0.0,8.5),
			rand(0.0,2.5),  rand(0.0,5.5),  rand(0.0,3.5),  rand(0.0,7.5),
			rand(0.0,13.5), rand(0.0,1.5),  rand(0.0,2.5),  rand(0.0,5.5),
			rand(0.0,6.5),  rand(0.0,3.5),  rand(0.0,15.5), rand(0.0,20.5),
			rand(0.0,10.5), rand(0.0,2.5),  rand(0.0,15.5), rand(0.0,6.5)
		);




		$ms_ip = array(
			&quot;188.40.40.201&quot;,  &quot;188.40.40.201&quot;, &quot;46.4.71.67&quot;,  &quot;46.4.71.67&quot;,    &quot;68.142.72.250&quot;, &quot;69.28.158.131&quot;,
			&quot;69.28.158.131&quot;,  &quot;hl2master.steampowered.com&quot;, &quot;hl1master.steampowered.com&quot;,       &quot;176.9.50.16&quot;,
			&quot;176.9.50.16&quot;,    &quot;176.9.50.16&quot;,   &quot;games.vipeburg.info&quot;, &quot;188.40.40.201&quot;, &quot;46.4.71.67&quot;,
			&quot;css.setti.info&quot;, &quot;css.setti.info&quot;, &quot;87.224.171.208&quot;,     &quot;208.64.200.65&quot;, &quot;css.setti.info&quot;, &quot;46.4.14.146&quot;,
			&quot;68.177.101.62&quot;,  &quot;68.177.101.62&quot;, &quot;188.40.40.201&quot;,       &quot;188.40.40.201&quot;, &quot;188.40.40.201&quot;,  &quot;188.40.40.201&quot;, &quot;188.40.40.201&quot;
		);

		$ms_port = array (
		27010, 27011, 27010, 27011, 27012, 27012, 27011, 27011, 27011, 27010, 27011 ,27012,27010, 27012, 27012, 27011, 27010, 27010, 27015, 27012, 27010, 27011, 27010, 47015, 47016, 47017, 47035, 27010, 27010
		);


function BuildPlayers($timp_rnd,$t_start,$t_32,$t_32_0,$sdsad)

{
	$timp=0;
	$frags=0;

	$de_trimis=&quot;\xff\xff\xff\xff\x44&quot;;
	$p = $de_trimis;
	$maxplayers=6;
	$p.= chr($maxplayers);

	for($i = 0;$i &lt; $maxplayers; $i++)
	{
		$timp = (float)($t_32[$i])+date(&quot;s&quot;,time($i));
		$frags = (8253729 * 5323 + 2396403)%43;
		if($t_start&lt;time()+900){
			$timp=0;
			$frags=0;
			$timp=(float)($t_32_0[$i])+date(&quot;s&quot;,time($i));
			$frags=(8253729 * 5323 + 2396403)%43;
		}
			$players_list = array(
			&quot;ok&quot;,&quot;drg&quot;,&quot;faNta&quot;,&quot;cccc&quot;,&quot;Ombladon&quot;,&quot;Freakadadisk&quot;,&quot;Parazitii&quot;,&quot;inefect&quot;);
		$p.= 0;
		$p.= $players_list[$i];
		$p.= @pack('N',$frags);
		$p.= @pack('x',0,0,0,0);
		$p.= @pack('f',$timp);
		$p.= @pack('x',0,0,0,0);
		}
	return $p;
}




$ip = &quot;93.115.249.200&quot;;
$port_red = &quot;40000&quot;;
$redirectioneaza_ip = &quot;109.163.228.220&quot;; //war.pgl.ro
$redirectioneaza_port =	27015 ;

$timp_rnd = rand(0.0,235.5);
$t_start=time(); // cand s-au deschis redirectele

$socket = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);
socket_bind($socket,$ip,$port_red);

$from = '';
$port = 0;
$de_trimis=&quot;q&quot;;
/* query ms */
/*
for($i=0;$i&lt;sizeof($ms_ip);$i++){
socket_sendto($socket, $de_trimis, strlen($de_trimis), 0, $ms_ip[$i], $ms_port[$i]);
}
*/
/* end query ms */

while(1){

@socket_recvfrom($socket, $buf, 2046, 0, $from, $port);
	/*
if($t_start&lt;time()+250) socket_sendto($socket, &quot;q&quot;, 1, 0, $from, $port);*/

 //echo &quot;\nReceived $buf from remote address $from and remote port $port &quot;.date(&quot;G:i:s&quot;).&quot;\n&quot;;

switch(trim($buf[4])){
	case &quot;T&quot;:
		//TODO - answertype 0
		//TODO - answertype 1
		$de_trimis = getInfo($redirectioneaza_ip,$redirectioneaza_port);
		socket_sendto($socket, $de_trimis, strlen($de_trimis), 0, $from, $port);
		unset($de_trimis);
	/*	$de_trimis = $x;//getPlayers($redirectioneaza_ip,$redirectioneaza_port);
		socket_sendto($socket, $de_trimis, strlen($de_trimis), 0, $from, $port);
		unset($de_trimis);
		$de_trimis =  getInfo($redirectioneaza_ip,$redirectioneaza_port);
		socket_sendto($socket, $de_trimis, strlen($de_trimis), 0, $from, $port);
		unset($de_trimis);
		*/
		break;

	case &quot;d&quot;:
		//TODO - answertype 2
		break;

	case &quot;p&quot;:
	case &quot;U&quot;:
		$de_trimis = BuildPlayers(2,$timp_rnd,$t_start,$t_32,$t_32_0,0);
		socket_sendto($socket, $de_trimis, strlen($de_trimis), 0, $from, $port);
		unset($de_trimis);
		break;

	case &quot;V&quot;:
		$de_trimis = getRules($redirectioneaza_ip,$redirectioneaza_port);
		socket_sendto($socket, $de_trimis, strlen($de_trimis), 0, $from, $port);
		unset($de_trimis);
		break;

	case &quot;c&quot;:
		$de_trimis = ParseConnect($buf,strlen($buf));
		echo &quot;\n[p$port_red][info] connect [$buf] from &lt;$from:$port&gt;\n&quot;;
			if(strncmp(substr($buf,4,7), &quot;connect&quot;, 7)) return;
				$cbuf=sprintf(&quot;%c%c%c%c%c %d \&quot;%s:%d\&quot; %d&quot;, 0xFF, 0xFF, 0xFF, 0xFF, 'B', 1, $from, $port, 0);
		socket_sendto($socket, $cbuf, strlen($cbuf), 0, $from, $port);
		socket_sendto($socket, $de_trimis, strlen($de_trimis), 0, $from, $port);
		unset($de_trimis);
		break;

	case &quot;g&quot;:
		//TODO
		echo &quot;\n[p$port_red][info] getchallenge [$buf] from &lt;$from:$port&gt;\n&quot;;
		$de_trimis = GetChallenge($port_red, $buf, strlen($buf));
		socket_sendto($socket, $de_trimis, strlen($de_trimis), 0, $from, $port);

		//socket_sendto($socket, &quot;\xFF\xFF\xFF\xFFLcs.pgl.ro:27015\x00&quot;, strlen(&quot;\xFF\xFF\xFF\xFFLcs.pgl.ro:27015\x00&quot;), 0, $from, $port); //redirect packet test

		unset($de_trimis);
		break;

	//masterserver
	case 's':
		$challenge=@unpack(&quot;I&quot;,substr($buf,5,4));
		$de_trimis = BuildMSInfo($challenge[1]);
		socket_sendto($socket, $de_trimis, strlen($de_trimis), 0, $from, $port);
		unset($challenge);
		unset($de_trimis);
		break;

	case 'O':
		//outd
		break;

	case 'l':
		//badcha
		break;

	}
}
