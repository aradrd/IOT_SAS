#ifndef GOOGLE_SHEET_H
#define GOOGLE_SHEET_H

#include <HTTPClient.h>
#include <WiFi.h>

String GOOGLE_SCRIPT_ID = "AKfycbwE-wRd4-k9RimSp_svSTjKdhQbHha_pTppacQrqA0_s8QRCHWLDjZIl7IhHYOTQRB3";

class GoogleSheet{
public:
    GoogleSheet(const char* ssid, const char *password) : ssid(ssid), password(password) {};
    GoogleSheet(const GoogleSheet&) = default;
    GoogleSheet& operator=(const GoogleSheet&) = delete;

    void init(){
        init_wifi();
    }

    void readDataFromGoogleSheet(){
        if (WiFi.status() == WL_CONNECTED) {
            HTTPClient http;
            String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec";
            Serial.println("Making a request");
            Serial.println(url);
            http.begin(url.c_str()); //Specify the URL and certificate
            http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
            int httpCode = http.GET();
            String payload;
            if (httpCode > 0) { //Check for the returning code
                payload = http.getString();
                Serial.println(httpCode);
                Serial.println(payload);
                //display1.println(payload);
            }
            else {
                Serial.println("Error on HTTP request");
            }
            http.end();
        }
    }

    void post_data(){
        String urlFinal = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec";

        Serial.println("POST data to spreadsheet:");
        Serial.println(urlFinal);
        HTTPClient http;
        http.begin(urlFinal.c_str());
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
        // Specify content-type header
        http.addHeader("Content-Type", "text/csv");
        // Data to send with HTTP POST
        String httpRequestData = "bla1,bla2,bla3";
        // Send HTTP POST request
        int httpResponseCode = http.POST(httpRequestData);
        Serial.print("HTTP Status Code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
    }


private:
    const char* ssid;
    const char* password;

    void init_wifi() {
        WiFi.mode(WIFI_STA);
        //WiFi.begin(ssid, password);
        WiFi.begin(ssid);
        Serial.print("Connecting to WiFi ..");
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print('.');
            delay(1000);
        }
        Serial.println(WiFi.localIP());
    }

};

#endif