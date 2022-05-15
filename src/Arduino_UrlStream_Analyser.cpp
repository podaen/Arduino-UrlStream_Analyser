/*
 Name:		Arduino_UrlStream_Analyser.cpp
 Created:	5/15/2022 10:31:57 AM
 Author:	podaen
 Editor:	http://www.visualmicro.com
*/

#include "Arduino_UrlStream_Analyser.h"

StreamAnalyserHTTPClient::StreamAnalyserHTTPClient()
{
}
StreamAnalyserHTTPClient::~StreamAnalyserHTTPClient()
{
}
bool StreamAnalyserHTTPClient::begin(const char* Url) {

    if (!http.begin(Url)) {
        return false;
    }
    Serial.println("[HTTP] begin...\n");

    //http.addHeader("Host", "sunshineradio.ice.infomaniak.ch");
    //http.addHeader("Host", "22183.live.streamtheworld.com");
    http.addHeader("Connection", "close");
    http.addHeader("Accept-Encoding", "");
    http.addHeader("Accept", "*/*");
    http.setUserAgent("Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)");

    http.setTimeout(30000);

    int httpCode = http.GET();
    if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK) {
            Serial.print("HTTP_CODE_OK: ");
            Serial.println(HTTP_CODE_OK);
            Serial.println("here");
            stream = http.getStreamPtr();
            Serial.println("here");
           
            len = this->stream->available();
            Serial.println("here");
            last1 = millis();
            return true;
        }
        else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            return false;
        }
    }
}
bool StreamAnalyserHTTPClient::copy() {
   // Serial.println("read bytes");
    len = this->stream->available();
    if (len > 0) {
        if (http.connected()) {
            len = this->stream->available();
            if (len > buffer_size) {
                size_t size = min((size_t)len, static_cast<size_t>(buffer_size));
                //Serial.print("size: ");
                //Serial.println(size);
                int read = this->stream->read((uint8_t*)buffer, size);
                if (read > 0) {
                    if (read < 512) {
                        Serial.print("read ");
                        Serial.print(cnt);
                        Serial.print(": ");
                        Serial.print(read);
                        Serial.println(" bytes");
                        Serial.println("This should never happen");
                    }
                    time1 = millis() - last1;
                    if (cnt > 4) {
                        if (time1 > time3) {
                            time3 = time1;
                        }
                    }
                    if (time1 >10) {
                        Serial.print("ms between read no ");
                        Serial.print(cnt);
                        Serial.print(": ");
                        Serial.println(time1);
                        Serial.print("max ms between read: ");
                        Serial.println(time3);
                        Serial.println("");
                    }
                    last1 = millis();
                    last2 = millis();
                    runonce = false;
                    cnt++;
                }
            }
            else
            {
                time2 = millis() - last2;
                if (time2 > 30000) {
                    if (!runonce) {
                        runonce = true;
                        Serial.print("Waiting for data 30000ms: ");
                        Serial.println(time2);
                        Serial.println("");
                    }
                }
            }
            return true;
        }
        else
        {
            Serial.println();
            Serial.print("[HTTP] connection closed or file end.\n");
            return false;
        }
    }

}
bool StreamAnalyserHTTPClient::setPath(const char* path) {
    Serial.println("set path");
    //http.end();
    //this->begin(path);
    Serial.println("end set path");
}

