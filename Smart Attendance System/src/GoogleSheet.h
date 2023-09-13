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

    String getUserList();
    bool postEntry(FileName file_name, const String& entry);
    uint16_t postChanges(FileName file_name, const std::vector<String>& pending_changes);

private:
    HTTPClient http;
    const String url;

    void establishConnection();
    void endConnection();
    const String get_url(const String& google_script_id);
    bool postData(const String& data, const String& post_url);
};

#endif
