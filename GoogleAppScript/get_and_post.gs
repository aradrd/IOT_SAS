//   Based on code published by Martin Hawksey
//   https://hawksey.info/blog/2014/07/google-sheets-as-a-database-insert-with-apps-script-using-postget-methods-with-ajax-example/


// Usage
//  1. Enter sheet name where data is to be written below
//  2. Run > setup
//
//  3. Publish > Deploy as web app
//    - enter Project Version name and click 'Save New Version'
//    - set security level and enable service (most likely execute as 'me' and access 'anyone, even anonymously)
//
//  4. Copy the 'Current web app URL' and post this in your form/script action
//
//  5. Insert column names on your destination sheet matching the parameter names of the data you are passing in (exactly matching case)

var SCRIPT_PROP = PropertiesService.getScriptProperties(); // new property service
var LOG_ENTRY_SIZE = 2;
var USER_INFO_SIZE = 2;

function doGet(e) {
    return ContentService.createTextOutput(JSON.stringify({ "result": "error", "error": "no get operations"}));
}

function doPost(e) {
  var res = ContentService.createTextOutput(JSON.stringify({ "result": "empty"}));
  if(e != undefined && e.parameter.get_approved_users !== undefined){
    res = get_all_approved_users();
  }
  else if(e.postData.contents !== undefined){
    if(e.parameter.add_user !== undefined){
      res = add_pending_user(e.postData.contents);
    }
    else if(e.parameter.add_log !== undefined){
      res = add_log_entry(e.postData.contents);
    }
    else if (e.parameter.add_multi_users !== undefined) {
      res = add_multi_pending_users(e.postData.contents);
    }
    else if (e.parameter.add_log_entries !== undefined) {
      res = add_multi_log_entries(e.postData.contents);
    }
    else {
      res = ContentService.createTextOutput(JSON.stringify({ "result": "error", "error": "action undefined", "postData": e.postData.contents}));
    }
  }
  return res;
}

function isApproved(arr) {
  return arr[2] == "V";
};

function isPending(arr) {
  return !isApproved(arr);
}

function get_all_approved_users(){
  var lock = get_lock();
  var sheet = get_sheet(0);
  var lastRow = sheet.getLastRow();
  var user_list = sheet.getRange("A2:C" + lastRow);
  var values;
  if (user_list !== undefined)
  {
      try {values = user_list.getValues();}
      catch (error) {return ContentService.createTextOutput(JSON.stringify({ "result": "error", "NoMatchingHeaderError": error })); }
  }


  var approved = values.filter(isApproved);
  var pending = values.filter(isPending);
  approved.forEach(remove_third_col);
  pending.forEach(remove_third_col);
  Logger.log(approved);
  Logger.log(pending);
  lock.releaseLock();
  return ContentService.createTextOutput(JSON.stringify({ "result": "success", "approved": approved, "pending": pending }));
}

function remove_third_col(value){
  value.splice(2,1);
}

function add_single_pending_user(user_info_arr) {
  if (user_info_arr.length !== USER_INFO_SIZE) {
    return false;
  }
  var id = user_info_arr[0].toString();
  var uid = user_info_arr[1];
  var sheet = get_sheet(0);
  var row = sheet.getLastRow() + 1;
  sheet.getRange(row, 1, 1, 2).setValues([[id, uid]]);
  return true;
}

function add_single_log_entry(log_arr) {
  if (log_arr.length != LOG_ENTRY_SIZE) {
    return false;
  }
  var attendance_log_sheet = get_sheet(1);
  var user_list_sheet = get_sheet(0);
  var uid = log_arr[0];
  // checks
  var row = attendance_log_sheet.getLastRow() + 1;
  attendance_log_sheet.getRange(row, 2, 1, 2).setValues([log_arr]);
  var user_id_index = user_list_sheet.getRange(1,2,user_list_sheet.getLastRow()).createTextFinder(uid).matchCase(false).findNext();
  var user_id = user_list_sheet.getRange(user_id_index.getRow(), 1).getValue().toString();
  attendance_log_sheet.getRange(row, 1).setValue(user_id);
  return true;
}

function add_multi_log_entries(contents) {
  var lock = get_lock();
  var res;
  var count = 0;
  var all_log_entries_arr = Utilities.parseCsv(contents);
  for (var i = 0; i < all_log_entries_arr.length; i++) {
    count += add_single_log_entry(all_log_entries_arr[i]);
  }
  if (count == all_log_entries_arr.length) {
    res = ContentService.createTextOutput(JSON.stringify({"result": "success", "count": count}));
  }
  else {
    res = ContentService.createTextOutput(JSON.stringify({"result": "partial", "count": count}));
  }
  lock.releaseLock();
  return res;
}

function add_multi_pending_users(contents) {
  var lock = get_lock();
  var res;
  var count = 0;
  var all_users = Utilities.parseCsv(contents);
  for (var i = 0; i < all_users.length; i++) {
    count += add_single_pending_user(all_users[i]);
  }
  if (count == all_users.length) {
    res = ContentService.createTextOutput(JSON.stringify({"result": "success", "count": count}));
  }
  else {
    res = ContentService.createTextOutput(JSON.stringify({"result": "partial", "count": count}));
  }
  lock.releaseLock();
  return res;
}

function add_pending_user(user_info){
  var lock = get_lock();
  var user_list_sheet = get_sheet(0);
  var res;
  if(user_info != undefined){
    var user_info_arr = Utilities.parseCsv(user_info)[0];
    if(user_info_arr.length == USER_INFO_SIZE){
      var user_id = user_info_arr[0];
      var user_uid = user_info_arr[1];
      var row = user_list_sheet.getLastRow() + 1;
      user_list_sheet.getRange(row, 1, 1, 2).setValues([[user_id, user_uid]]);
      res = ContentService.createTextOutput(JSON.stringify({"result": "success"}))
    }
    else{
      Logger.log("Post request not in correct csv format");
      res = ContentService.createTextOutput(JSON.stringify({ "result": "error", "error": "wrong csv format"}));
    }
  }
  else{
    Logger.log("Missing user info from post request");
    res = ContentService.createTextOutput(JSON.stringify({ "result": "error", "error": "missing user info"}));
  }
  lock.releaseLock();
  return res;
}


function add_log_entry(log){
  var lock = get_lock();
  var attendance_log_sheet = get_sheet(1);
  var user_list_sheet = get_sheet(0);
  var res;
  if(log != undefined){
    var log_arr = Utilities.parseCsv(log)[0];
    var user_uid = log_arr[0];
    // checks
    if(log_arr.length == LOG_ENTRY_SIZE){
      var row = attendance_log_sheet.getLastRow() + 1;
      attendance_log_sheet.getRange(row, 2, 1, 2).setValues([log_arr]);
      var user_id_index = user_list_sheet.getRange(1,2,user_list_sheet.getLastRow()).createTextFinder(user_uid).matchCase(false).findNext();
      var user_id = user_list_sheet.getRange(user_id_index.getRow(), 1).getValue();
      attendance_log_sheet.getRange(row, 1).setValue(user_id);
      res = ContentService.createTextOutput(JSON.stringify({"result": "success"}));
    }
    else{
      Logger.log("Post request not in correct csv format");
      res = ContentService.createTextOutput(JSON.stringify({ "result": "error", "error": "wrong csv format"}));
    }
  }
  else{
    Logger.log("Missing log from post request");
    res = ContentService.createTextOutput(JSON.stringify({ "result": "error", "error": "missing log"}));
  }

  lock.releaseLock();
  return res;
}

function get_lock(){
  var lock = LockService.getPublicLock();
  lock.waitLock(30000);  // wait 30 seconds before conceding defeat.
  return lock;
}

function get_sheet(index){
  var ss = SpreadsheetApp.getActiveSpreadsheet();
  var sheet = ss.getSheets()[index];
  return sheet;
}

function setup() {
    var doc = SpreadsheetApp.getActiveSpreadsheet();
    SCRIPT_PROP.setProperty("key", doc.getId());
}

