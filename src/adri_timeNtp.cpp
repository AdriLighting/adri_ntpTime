#include "adri_timeNtp.h"


// #ifdef MRLNTPTIME

#include <Arduino.h>

namespace {
	void seconds2time(unsigned long s, char * time) {
	   // int milli      = (s                    ) % 1000;
	   int seconds    = (s /   (1000)         ) % 60   ;
	   int minutes    = (s /   (1000*60)      ) % 60   ;
	   int hours      = (s /   (1000*3600)    ) % 24   ;
	   int days    = (s /  (1000*3600*24)  )     ;
	   sprintf(time,"%d-%02d:%02d:%02d", days, hours , minutes, seconds);
	}
	String on_time() {
	   char t[100];
	   unsigned long offset = 0; //1000 * 60 * 60 * 24 * 3 ;
	   unsigned long ms=millis()+offset;
	   seconds2time(ms, t);
	   return String(t);
	}	
	String literal_value(String name, String xml){
	   String open,close;
	   String value = "";
	   int start, end;
	   open="<"+name+">";
	   close="</"+name+">";
	   start=xml.indexOf(open);
	   if (start!=-1) {
	      start+=name.length()+2;
	      end=xml.indexOf(close);
	      value=xml.substring(start,end);
	   }
	   return value;
	}	
	int* splitTime(String Val, char sep) {
	    String      aVal            = Val;
	    byte        firstIndex      = aVal.indexOf(sep);
	    byte        secondIndex     = aVal.indexOf(sep, firstIndex + 1);
	    String      hr              = aVal.substring(0, firstIndex);
	    String      minute          = aVal.substring(firstIndex + 1, secondIndex);
	    String      sec             = aVal.substring(secondIndex + 1);
	    int         _hr             = hr.toInt();
	    int         _minute         = minute.toInt();
	    int         _sec            = sec.toInt();

	    int     *array          = new int[3];
	            array[0]        = _hr;
	            array[1]        = _minute;
	            array[2]        = _sec;
	    return array;    
	}

	/*-------- NTP code ----------*/
	const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
	byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

	time_t adri_timeNtp_espApTime;

	time_t adri_timeNtp_timeFromEsp() {
		return adri_timeNtp_espApTime;
	}
	time_t adri_timeNtp_getNtpTime() {

		adri_timeNtp * ptr = adri_timeNtp_instance();

		IPAddress ntpServerIP; // NTP server's ip address

		while (ptr->udpServer.parsePacket() > 0) ; // discard any previously received packets
		// Serial.println("Transmit NTP Request");
		// get a random server from the pool
		WiFi.hostByName(ptr->serverName, ntpServerIP);

		// String ip_str = ip2string(ntpServerIP);
		// Serial.printf("[getNtpTime] server: %10s ip: %10s\n", serverName, ip_str.c_str());

		ptr->sendNTPpacket(ntpServerIP);

		uint32_t beginWait = millis();
		while (millis() - beginWait < 1500) {
		int size = ptr->udpServer.parsePacket();
		if (size >= NTP_PACKET_SIZE) {
			// Serial.println("Receive NTP Response");
			ptr->udpServer.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
			unsigned long secsSince1900;
			// convert four bytes starting at location 40 to a long integer
			secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
			secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
			secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
			secsSince1900 |= (unsigned long)packetBuffer[43];
			// return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;

			secsSince1900 = secsSince1900 - 2208988800UL + (ptr->timeZone * SECS_PER_HOUR);
			if (ptr->isDst(hour(secsSince1900), day(secsSince1900), month(secsSince1900), dayOfWeek(secsSince1900) - 1)) secsSince1900 += 3600;  //add hour if DST           
			return secsSince1900;
		}
		}
		// Serial.println("No NTP Response :-(");
		return 0; // return 0 if unable to get the time
	}
}


adri_timeNtp * adri_timeNtp_ptr = nullptr;
adri_timeNtp * adri_timeNtp_instance() 	{return adri_timeNtp_ptr;}
adri_timeNtp::adri_timeNtp() 			{adri_timeNtp_ptr = this;}

bool adri_timeNtp::isDst(int hour, int day, int month, int dow)  //north american dst  dow 0=SUN
{
	// if (WebServer.daylight == false) return false; //option to disable DST

													 //January, february, and december are out.
	if (month < 3 || month > 11) { return false; }
	//April to October are in
	if (month > 3 && month < 11) { return true; }
	int previousSunday = day - dow;
	//In march, we are DST if our previous sunday was on or after the 8th.
	if (month == 3) { return previousSunday >= 8; }
	//In november we must be before the first sunday to be dst.
	//That means the previous sunday must be before the 1st.
	return previousSunday <= 0;
}





// send an NTP request to the time server at the given address
void adri_timeNtp::sendNTPpacket(IPAddress &address)
{

	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;
	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	udpServer.beginPacket(address, 123); //NTP requests are to port 123
	udpServer.write(packetBuffer, NTP_PACKET_SIZE);
	udpServer.endPacket();
}				

time_t adri_timeNtp::timeFromEsp_set(String str) {

	String s_d 	= literal_value("date", str);	
	String s_t 	= literal_value("time", str);	
	String s_dw = literal_value("time", str);	
	int 	*split_date = splitTime(s_d, '-');
	int 	s_year 	= split_date[0];
	int 	s_mon 	= split_date[1];
	int 	s_day 	= split_date[2];
	int 	*split_time = splitTime(s_t, ':');
	int 	s_hr 	= split_time[0];
	int 	s_min 	= split_time[1];
	int 	s_sec 	= split_time[2];

	char buff[120];
	sprintf(buff, "\n(timeFromEsp_set)\n%02d%02d%02d %02d%02d%02d", s_year, s_mon, s_day, s_hr, s_min, s_sec);
	Serial.println(buff);

	myTimeElements.Year = ( s_year-1970);
	myTimeElements.Month = s_mon;
	myTimeElements.Day = s_day;
	myTimeElements.Hour = s_hr;
	myTimeElements.Minute = s_min;
	myTimeElements.Second = s_sec;
	time_t t = makeTime(myTimeElements);

	adri_timeNtp_espApTime = t;
	setTime(adri_timeNtp_espApTime);  

	setSyncProvider(adri_timeNtp_timeFromEsp);
	setSyncInterval(10000);	

	time_isSet = true;	

}
void adri_timeNtp::timeZone_set(int value){timeZone=value;}
void adri_timeNtp::timeIsSet(boolean & value){value=time_isSet;}


void adri_timeNtp::setup(boolean WifiConnected){

	time_isSet = WifiConnected;

	if (!WifiConnected) return;

	udpServer.begin(localPort);
	setSyncProvider(adri_timeNtp_getNtpTime);
	setSyncInterval(10000);	
}
void adri_timeNtp::loop(){
	if (now() != prevDisplay) {prevDisplay = now();}   
}
time_t adri_timeNtp::timeget() {
	if(adri_timeNtp_ptr == nullptr) return 0;
	return prevDisplay;
}
void adri_timeNtp::dateGet(int & wDay, int & sMon, int & sYear) {
	if(adri_timeNtp_ptr == nullptr) return;
	wDay 	= day(now());
	sMon 	= month(now());
	sYear 	= year(now());
}
String adri_timeNtp::timetoString(time_t t) {
    char tmpStr[100];
    sprintf(tmpStr, "%02d:%02d:%02d", hour(t), minute(t), second(t));
    return String(tmpStr);
}
void adri_timeNtp::timeget(String & ret) {
    time_t  time;
    if (adri_timeNtp_instance()!=nullptr){
        time    = timeget();
        ret     = timetoString(time);           
    } else {
        ret = on_time();                     
    }
}
