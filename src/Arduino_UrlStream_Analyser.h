/*
 Name:		Arduino_UrlStream_Analyser.h
 Created:	5/15/2022 10:31:57 AM
 Author:	podaen
 Editor:	http://www.visualmicro.com
*/

#ifndef _Arduino_UrlStream_Analyser_h
#define _Arduino_UrlStream_Analyser_h

#include <Arduino.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "Client.h"

class StreamAnalyserHTTPClient {
public:
	StreamAnalyserHTTPClient();
	~StreamAnalyserHTTPClient();

	bool begin(const char* Url);
	bool copy();
	bool setPath(const char* path);

protected:
	unsigned long last1 = 0;
	unsigned long time1 = 0;
	unsigned long last2 = 0;
	unsigned long time2 = 0;
	unsigned long last3 = 0;
	unsigned long time3 = 0;
	bool runonce;

private:
	uint8_t buffer[512] = { 0 };
	const int buffer_size = 512;
	HTTPClient http;
	WiFiClient* stream;
	int len;
	int cnt;
};

#endif

