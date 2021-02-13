#include <adri_timeNtp.h>
adri_timeNtp 		* myRealTime;

#define SECRET_WIFICONNECT_1
#include <adri_wifiConnect.h> // https://github.com/AdriLighting/adri_wifiConnect/
#include "secret.h"

#include <adri_timer.h> // https://github.com/AdriLighting/adri_timer/
adri_timer 			* myRealTimeTimer;

#include <LittleFS.h>

void setup()
{
		Serial.begin(115200);
		LittleFS.begin();

		boolean myWifiIsConnect = false;
		_wifiConnect(SECRET_SSID,SECRET_PASS,myWifiIsConnect);
		myRealTime = new adri_timeNtp();
		myRealTime->setup(myWifiIsConnect);

		myRealTimeTimer = new adri_timer(1000,"",true);
}

void loop()
{
	myRealTime->loop();
	if(myRealTimeTimer->loop()) {
		time_t time = myRealTime->timeget();
		String timeStr = myRealTime->timetoString(time);
		Serial.println(timeStr);
	}
}
