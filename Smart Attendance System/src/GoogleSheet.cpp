#include <GoogleSheet.h>

const std::map<FileName, String> FILE_TO_POST_EXTENSION {
    {ATTENDANCE_LOG, ADD_LOG},
    {PENDING_USER_LIST, ADD_USER},
};

String GoogleSheet::getUserList(){
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

bool GoogleSheet::postEntry(FileName file_name, const String& entry) {
    return postMultiEntries(file_name, std::vector<String>({entry}));
}

String GoogleSheet::getPostUrl(FileName file_name) {
    return url + "?" + FILE_TO_POST_EXTENSION.at(file_name);
}

bool GoogleSheet::postMultiEntries(FileName file_name, const std::vector<String>& entries) {
    String data = "";
    for (const auto& entry : entries) {
        data += entry + "\n";
    }
    return postData(data, getPostUrl(file_name));
}

void GoogleSheet::establishConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected.");
        return;
    }

    http.begin(url.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
}

void GoogleSheet::endConnection() {
    http.end();
}

const String GoogleSheet::get_url(const String& google_script_id) {
    return "https://script.google.com/macros/s/" + google_script_id + "/exec";
}

bool GoogleSheet::postData(const String& data, const String& post_url){
    establishConnection();
    http.addHeader("Content-Type", "text/csv");
    int http_response_code = http.POST(data);
    Serial.print("HTTP Status Code: ");
    Serial.println(http_response_code);
    String payload = http.getString();
    Serial.println("Payload: " + payload);
    endConnection();
    return http_response_code == 200;
}

uint16_t GoogleSheet::postChanges(FileName file_name, const std::vector<String>& pending_changes) {
    bool res = postMultiEntries(file_name, pending_changes);
    return res * pending_changes.size();
}