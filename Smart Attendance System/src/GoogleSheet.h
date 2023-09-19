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
    uint16_t postChanges(FileName file_name, const std::vector<String>& pending_changes);
    void validateWiFi();
    void init();

private:
    HTTPClient http;
    const String url;

    bool establishConnection(const String& override_url = "");
    void endConnection();
    const String get_url(const String& google_script_id);
    bool postData(const String& data, const String& post_url);
    String getDataWithPost(const String& data, const String& post_url);
    String getPostUrl(FileName file_name);
    bool postMultiEntries(FileName file_name, const std::vector<String>& entries);
    bool postEntry(FileName file_name, const String& entry); // Slow.
};

#endif
