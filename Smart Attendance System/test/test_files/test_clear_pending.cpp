#include <Arduino.h>
#include <unity.h>
#include "IOTFiles.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_read_pendings_user_list(void){
    IOTFiles files;
    String file_data = files.readPendingUserList();
    printf(file_data.c_str());
    printf("\n");
}

void test_clear_pending_user_list(void){
    IOTFiles files;
    files.clearPendingUserList();
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_read_pendings_user_list);
    RUN_TEST(test_clear_pending_user_list);
    RUN_TEST(test_read_pendings_user_list);
    UNITY_END();
}

void loop() {}