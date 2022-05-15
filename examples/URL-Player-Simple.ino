/*
 Name:		URL_Player_Simple.ino
 Created:	5/11/2022 11:50:58 AM
 Author:	Podaen
*/

#include <Arduino_UrlStream_Analyser.h>
#include <WiFi.h>
#include <esp_bt_main.h>//power saving bluetooth
#include <esp_bt.h>//power saving bluetooth

const char* urls[] = {
	"http://22183.live.streamtheworld.com/JOE.mp3",
	"http://25683.live.streamtheworld.com:80/MNM_128.mp3",
	"http://icecast-servers.vrtcdn.be/ketnetradio-high.mp3",
	"http://icecast-servers.vrtcdn.be/mnm_90s00s-high.mp3"
};

StreamAnalyserHTTPClient Player;

const char* ssid = "SSID";
const char* password = "PASS";

unsigned long currentMillis;
unsigned const long StationDelay = 30000;
unsigned long StationPrevious = 0;
//bool BlnAutoSelectNextStation = false;//does not work yet
//bool BlnAutoSelectPreviousStation = false;//does not work yet
int UrlSelector = 0;//set the stream

void setup() {
	Serial.begin(500000);
	while (!Serial);

	WiFi.mode(WIFI_MODE_STA);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		// Serial.println("Connecting to WiFi..");
	}
    Serial.println("Connected");
	WiFi.setSleep(false);

	esp_bluedroid_disable();
	esp_bluedroid_deinit();
	esp_bt_controller_disable();
	esp_bt_controller_deinit();

	Player.begin(urls[UrlSelector]);

	Serial.println("end setup");
}
void loop() {
	if (WiFi.status() == WL_CONNECTED) {
		Player.copy();
		//if (BlnAutoSelectPreviousStation == true) AutoSelectPreviousStation();
		//if (BlnAutoSelectNextStation == true) AutoSelectNextStation();
    }
	else
	{
		Serial.println("wifi disconnected");
	}
}

void AutoSelectNextStation() {
	currentMillis = millis();
	if (currentMillis - StationPrevious > StationDelay) {//VERTRAAGDE SIRENE INSCHAKELING
		StationPrevious = currentMillis;
		UrlSelector++;
		if (UrlSelector >= sizeof(urls) / sizeof(urls[0]))UrlSelector = 0;
		Serial.println("Auto Next");
		Serial.println(urls[UrlSelector]);
		//EepromWriteUrlSelector();
		Player.setPath(urls[UrlSelector]);
	}
}
void AutoSelectPreviousStation() {
	currentMillis = millis();
	if (currentMillis - StationPrevious > StationDelay) {//VERTRAAGDE SIRENE INSCHAKELING
		StationPrevious = currentMillis;
		UrlSelector--;
		if (UrlSelector < 0)UrlSelector = (sizeof(urls) / sizeof(urls[0]) - 1);
		Serial.println("Auto Previous");
		Serial.println(urls[UrlSelector]);
		//EepromWriteUrlSelector();
		Player.setPath(urls[UrlSelector]);
	}
}