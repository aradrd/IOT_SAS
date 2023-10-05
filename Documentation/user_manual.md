# User Manual For Smart Attendance System
  
## Starting the system
Connect the system to a voltage source.
The system will connect to the WIFI, syncronize the time while presenting "Connecting to NTP server".
After its finised, the system will present "Please swipe card".
> **_NOTE:_**   without WIFI and NTP connection, the system cannot work. If the system is stuck in "Connecting to NTP server" try reseting the system (reconnect voltage) and check the WIFI.

## Main screen
The user can swipe a card. If the card is approved, there will be a flash on the screen.
If the card is pending aprroval, the user should wait for manager approval.
If the card isn't registered, the user can register. more instructions in 'Registration screen'.

## Registration screen
1. Press **A** to register or **C** to Cancel.
*If the user pressed another key, an error message will be presented.*

2. Enter a 9 digit ID and press *A* in the end. To delete a digit, press *B*.
*If the ID is already registered to another card, an error message will be presented and return to the main screen.*
*If the ID is too short (less than 9-digit), an error message will be presented and return to the main screen.*
* To cancel registeration in any phase, press *C*.
3. The card info will be sent for approval and now considered pending.

## Connectivity indication
If the system is connected to WIFI, there will be a small WIFI symbol ᯤ in the top right corner.
If the system isn't connected to WIFI, there will be a small X symbol in the top right corner.
> **_NOTE:_**   The user can still register (but will not be approved) and enter (if approved) with no WIFI. 

## Manager manual
* To approve a user, go to the 'user_list' sheet and add V in the aprroved column for the user line.
The user can be identified with its ID.
* To delete a user, go to the 'user_list' sheet and simply delete the user row.
* To see the attendance log,  go to the 'attendance_log' sheet.