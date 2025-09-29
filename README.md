## 💡 MODULE: Remote LED using ESP32 with Firebase

### I. PRACTICUM OBJECTIVE
To create a simple remote LED control application.

---

### II. TOOLS AND MATERIALS
1. Laptop/PC with internet connection
2. Smartphone (Android)

---

### III. THEORETICAL BASIS
The **ESP32** is a microcontroller introduced by Espressif Systems and is the successor to the ESP8266. One of the advantages of the ESP32 is the inclusion of **Wi-Fi and Bluetooth**, which greatly simplifies the creation of **IoT systems** that require a wireless connection. These features were not present in the ESP8266, making the ESP32 an upgrade from the ESP8266.

*(A pinout diagram of the ESP32 Dev Kit V1 is referenced here.)* 

---

### IV. PRACTICUM STEPS

#### A. Creating the Firebase Database

1.  Open **[https://firebase.google.com/](https://firebase.google.com/)**, then create/open an account and a new project.
2.  Click **Add project** > "give your project a name" > **Continue** > **Create project**. The following page will appear.
3.  Select **Realtime Database**, then choose **Create database** > **Next** > select **Start in locked Mode** > **Enable**.
4.  **Change the existing rules** to allow anyone to read, modify, or add data to the database, then **Publish**.
    *(Note: The specific changed rules are not provided but are generally: `"read": true, "write": true`)*
5.  In **Authentication**, select and **enable the anonymous option**.

---

#### B. ESP32 Coding

1.  **Wire the ESP32 and the traffic light module** as shown in the diagram.
    **Wiring Details:**
    a. The **R** (Red) pin of the traffic light connects to ESP32 pin **D13**.
    b. The **Y** (Yellow) pin of the traffic light connects to ESP32 pin **D12**.
    c. The **G** (Green) pin of the traffic light connects to ESP32 pin **D14**.
    d. The **GND** pin of the traffic light connects to **GND**.
2.  Open the Arduino IDE and **download the Firebase client library**.
3.  Select the **DOIT ESP32 DEVKIT V1** board.
4.  **Write the following source code** and **make adjustments** for the following points:
    a. `WIFI_SSID`: the name of your Wi-Fi network
    b. `WIFI_PASSWORD`: the password for your Wi-Fi network
    c. `API_KEY`: your Firebase API key
    d. `DATABASE_URL`: your Firebase RTDB address

**(Source Code Provided)**

5.  In this practicum, the controlled LEDs simulate a **traffic light**. The data sent from the ESP32 to Firebase and vice versa must be in **string format**. This is because the MIT App Inventor application can easily read data of the **string type**.
6.  The **`API_KEY`** is obtained from **Project Overview** > **Project Settings**.
7.  The **`DATABASE_URL`** is obtained from your Firebase Realtime Database dashboard.

---

#### C. Creating the Application in MIT App Inventor

1.  Open **[http://ai2.appinventor.mit.edu/](http://ai2.appinventor.mit.edu/)** and create a new project.
2.  Design the application with the following layout: *(Layout details are implied to include switches for Red, Yellow, Green, and a Mode switch.)* 
3.  **Change the properties** in `FirebaseDB1` according to your **Firebase token**, **Firebase URL**, and **ProjectBucket** corresponding to your database path.
4.  Switch to the blocks editor side and create the program blocks as follows: *(The blocks are detailed in section V.3)*

---

### V. Assignment

1.  Show the result of the application design and prove that the application works well (screenshot of the serial monitor, Firebase, HP screen, and demo)!
2.  Explain the function of the ESP32 source code!
3.  Explain the function of each block in the application!

*(The first question is left blank for practical results.)*

#### 2. Explanation of the ESP32 Source Code Function

***

##### 1. Include Library and Constant Definition Blocks

| Code | Function |
| :--- | :--- |
| `#include <WiFi.h>` | Library for managing **Wi-Fi connection** on ESP32. |
| `#include <Firebase_ESP_Client.h>` | The main library for **Firebase communication**. |
| `#include "addons/TokenHelper.h"` | Helper for managing Firebase **authentication tokens**. |
| `#include "addons/RTDBHelper.h"` | Specific helper for **Realtime Database** operations. |
| `#define...` (WIFI, API, DB URL) | Defines Wi-Fi credentials and Firebase configuration (SSID, Password, API Key, Database URL). |

---

##### 2. Global Variable Declaration Block

| Code | Function |
| :--- | :--- |
| `FirebaseData fbdo;` | Object to handle **Firebase data operations**. |
| `FirebaseAuth auth;` | Object to manage **Firebase authentication**. |
| `FirebaseConfig config;` | Object for **Firebase configuration**. |
| `int LED_RED = 13;` etc. | **Pin assignment** for the LEDs (Red=13, Yellow=12, Green=14). |
| `unsigned long sendDataPrevMillis = 0;` | **Timer** to set the data reading interval. |
| `bool signupOK = false;` | **Status flag** indicating successful Firebase connection/sign-up. |

---

##### 3. `setup()` Function - System Initialization

| Code | Function |
| :--- | :--- |
| `Serial.begin(9600);` `pinMode(..., OUTPUT);` | Initializes **serial communication** and sets LED pins as **OUTPUT**. |
| `WiFi.begin(...)` `while (WiFi.status() != WL_CONNECTED)` | Starts the **Wi-Fi connection** process and loops until connected. |
| `config.api_key = API_KEY;` `config.database_url = DATABASE_URL;` | Configures the **Firebase API key and database URL**. |
| `if (Firebase.signUp(&config, &auth, "", "")){...}` | Performs the **sign-up process (anonymous authentication)**, sets the `signupOK` flag, and displays errors if authentication fails. |
| `Firebase.begin(&config, &auth);` `Firebase.reconnectWiFi(true);` | **Starts the Firebase service** and enables **automatic Wi-Fi reconnection**. |

---

##### 4. `otomatis()` Function - Automatic Operation Mode

| Code | Function |
| :--- | :--- |
| `digitalWrite(LED_GREEN, HIGH);` `Firebase.RTDB.setString(..., "LED/GREEN", "1");` | **Activates the green light** for 10 seconds and **synchronizes the status to Firebase** (simulating the "Go" phase). |
| `digitalWrite(LED_YELLOW, HIGH);` `delay(10000);` | **Activates the yellow light** for 10 seconds (simulating the "Caution" phase). |
| `digitalWrite(LED_RED, HIGH);` `delay(10000);` | **Activates the red light** for 10 seconds (simulating the "Stop" phase). |

---

##### 5. `manual()` Function - Manual Operation Mode

| Code | Function |
| :--- | :--- |
| `Firebase.RTDB.getString(&fbdo, "LED/RED");` `String red = fbdo.stringData();` | **Reads the status of the Red LED** from the Firebase database. |
| `if (red == "1") { digitalWrite(LED_RED, HIGH); }` | **Controls the Red LED** based on the database value ("1"=ON, "0"=OFF). |
| *(The same pattern is repeated for the Yellow and Green LEDs)* | **Allows individual control** of each LED from the MIT App Inventor application. |
| `delay(1000);` | A short delay for **reading stability**. |

---

##### 6. `loop()` Function - Main Program Execution

| Code | Function |
| :--- | :--- |
| `if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000...` | Ensures Firebase is ready and authenticated. **Sets a data reading interval of 15 seconds** using non-blocking timing. |
| `Firebase.RTDB.getString(&fbdo, "LED/Mode");` | **Reads the operation mode** from Firebase (path "LED/Mode"). |
| `if (mode == "1") { otomatis(); } else if (mode == "0") { manual(); }` | **Calls the appropriate function** (`otomatis()` for mode "1", `manual()` for mode "0") based on the value read from Firebase. |

---

#### 3. Explanation of Each Block in the MIT App Inventor Application

The blocks in the MIT App Inventor application act as a **bidirectional communication bridge** with the ESP32 via the Firebase Realtime Database (RTDB).

##### Timer Clock Block for Database Synchronization

| Block | Function |
| :--- | :--- |
| `when Clock1.Timer` | Functions as an **automatic polling mechanism** that periodically reads the values from Firebase RTDB for the tags **"RED," "YELLOW," "GREEN," and "MODE"**. This ensures the mobile application always displays the current status of the ESP32 hardware, maintaining **real-time synchronization**. |

##### Firebase Database Communication Integration

| Block | Function |
| :--- | :--- |
| `FirebaseDB1.GetValue` | Used within the timer to **read the current status** (tags "RED," "YELLOW," "GREEN," "MODE") from the database and display it on the application's user interface. |
| `FirebaseDB1.StoreValue` | Used by the switch event handlers to **send control commands** ("1" or "0") from the user to the database with a consistent hierarchical structure (e.g., `LED/RED`). |

##### Individual LED Switches (SW\_RED, SW\_YELLOW, SW\_GREEN)

| Block | Function |
| :--- | :--- |
| `when SW_RED.Changed` | The event handler **sends the value "1" (on) or "0" (off)** to the corresponding tag (e.g., "RED") in Firebase when the user toggles the switch. This value is then read by the ESP32 in the `manual()` function to control the physical LED. |

##### Operation Mode Switch

| Block | Function |
| :--- | :--- |
| `when Mode.Changed` | Controls the system's operation by **sending "1" (Automatic) or "0" (Manual)** to the **"MODE"** tag in Firebase. It also **hides/shows the individual LED switches** and triggers the ESP32 to execute the corresponding `otomatis()` or `manual()` function. |
