#ifndef ADRI_TIMENTP_H
	#define ADRI_TIMENTP_H

	// #include "config.h"

	// #ifdef MRLNTPTIME

		#include <TimeLib.h>
		#include <ESP8266WiFi.h>
		#include <WiFiUdp.h>
		#include <arduino.h>

		// void 	ntp_setup(boolean WifiConnected);
		// extern time_t ntp_prevDisplay;
		// extern boolean ntp_time_isSet;


		class adri_timeNtp
		{




		public:

 				const char* 	serverName     		= "fr.pool.ntp.org";    // NTP Servers:
           		int             timeZone            = 1;                    // Central European Time
				unsigned int    localPort           = 8888;                 // local port to listen for UDP packets
				time_t          prevDisplay         = 0; 

				WiFiUDP 		udpServer;

				boolean 		time_isSet 			= false;

				TimeElements 	myTimeElements;

			adri_timeNtp();
			~adri_timeNtp(){};

			bool 	isDst(int hour, int day, int month, int dow);
			void 	sendNTPpacket(IPAddress &address);
			time_t 	getNtpTime();

			void 	timeZone_set(int value);

			void 	setup(boolean WifiConnected);
			void 	loop();

			void 	timeIsSet(boolean & value);

			time_t 	timeFromEsp_set(String str);

			time_t 	timeget();
			void 	dateGet(int & wDay, int & sMon, int & sYear) ;

		};

		adri_timeNtp * adri_timeNtp_instance() ;
	// #endif
#endif