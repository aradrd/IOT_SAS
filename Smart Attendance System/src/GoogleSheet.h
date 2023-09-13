#ifndef GOOGLE_SHEET_H
#define GOOGLE_SHEET_H

#include <string>
#include <map>

#include <HTTPClient.h>
#include <WiFi.h>

#include <consts.h>

class GoogleSheet{
public:
    GoogleSheet() : http(), url(get_url(GOOGLE_SCRIPT_ID)) {};
    GoogleSheet(const GoogleSheet&) = default;
    GoogleSheet& operator=(const GoogleSheet&) = delete;

    String getUserList(){
        establishConnection();
        int httpCode = http.GET();
        String payload;
        if (httpCode > 0) {
            payload = http.getString();
        }
        else {
            Serial.println("Error on HTTP request");
        }

        endConnection();
        return payload;
    }

    void addAttendanceLogEntry(const String& entry) {
        postData(entry ,url + "?" + ADD_LOG);
    }

    void addPendingUserEntry(const String& user) {
        postData(user ,url + "?" + ADD_USER);
    }

private:
    HTTPClient http;
    const String url;

    void establishConnection() {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("WiFi not connected.");
            return;
        }

        http.begin(url.c_str());
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    }

    void endConnection() {
        http.end();
    }

    const String get_url(const String& google_script_id) {
        return "https://script.google.com/macros/s/" + google_script_id + "/exec";
    }

    void postData(const String& data, const String& post_url){
        establishConnection();
        http.addHeader("Content-Type", "text/csv");
        int httpResponseCode = http.POST(data);
        Serial.print("HTTP Status Code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        endConnection();
    }
};

#endif
