#define BLYNK_PRINT Serial  // Define the Blynk print output to Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Blynk authentication token and Wi-Fi credentials
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// Pin assignments for flow sensors
const int input1 = 13;
const int input2 = 12;
const int input3 = 14;

// Variables to store sensor readings and calculations
int X1, X2, X3;
double TIME1 = 0, TIME2 = 0, TIME3 = 0;
double FREQUENCY1 = 0, FREQUENCY2 = 0, FREQUENCY3 = 0;
double WATER1 = 0, WATER2 = 0, WATER3 = 0;
double TOTAL1 = 0, TOTAL2 = 0, TOTAL3 = 0;
double LS1 = 0, LS2 = 0, LS3 = 0;
double totalSum = 0;

bool alertTriggered = false;  // Flag variable to track if alert has been triggered

unsigned long startTime;
unsigned long elapsedTime;
bool isActivated = false;

// Function to forward data between Serial and Software Serial (Serial2)
void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    Serial2.write(Serial.read());
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read());
  }
}

void setup()
{
  Serial2.begin(9600);  // Initialize Software Serial (Serial2)
  delay(3000);

  startTime = millis();  // Store the current time

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);  // Connect to Blynk server

  Serial.begin(9600);      // Initialize Serial communication
  pinMode(input1, INPUT);  // Set flow sensor pins as INPUT
  pinMode(input2, INPUT);
  pinMode(input3, INPUT);

  Blynk.virtualWrite(V4, 0);     // Set the initial state of the button to OFF
  Blynk.virtualWrite(V4, HIGH);  // Enable the button to send messages to the board
}

// Function to print the liters value to Serial and trigger alert if necessary
void printLiters(const char* label, double value)
{
  Serial.print(label);
  Serial.print(": ");
  Serial.print(value);
  Serial.println(" liters");

  if (value > 0.5 && !alertTriggered)
  {
    alertTriggered = true;
    Serial2.begin(9600);
    send_SMS();
  }
}

// Function to send an SMS alert
void send_SMS()
{
  Serial2.println("AT+CMGF=1");                  // Configure GSM module in TEXT mode
  updateSerial();
  Serial2.println("AT+CMGS=\"+YOUR_PHONE_NUMBER\""); // Specify the recipient's phone number
  updateSerial();
  Serial2.print("ALERT: Exceeded 50% of the bill cut"); // Text content of the SMS
  updateSerial();
  Serial2.println();
  Serial2.write(26);  // Send SMS message
}

void loop()
{
  updateSerial();

  // Read flow sensor 1 data
  X1 = pulseIn(input1, HIGH);
  Y1 = pulseIn(input1, LOW);
  TIME1 = X1 + Y1;
  FREQUENCY1 = 1000000 / TIME1;
  WATER1 = FREQUENCY1 / 7.5;
  LS1 = WATER1 / 60;

  if (FREQUENCY1 >= 0)
  {
    if (isinf(FREQUENCY1))
    {
      Blynk.virtualWrite(V1, TOTAL1);
    }
    else
    {
      TOTAL1 += LS1;
      Blynk.virtualWrite(V1, TOTAL1);
    }
  }

  // Read flow sensor 2 data
  X2 = pulseIn(input2, HIGH);
  Y2 = pulseIn(input2, LOW);
  TIME2 = X2 + Y2;
  FREQUENCY2 = 1000000 / TIME2;
  WATER2 = FREQUENCY2 / 7.5;
  LS2 = WATER2 / 60;

  if (FREQUENCY2 >= 0)
  {
    if (isinf(FREQUENCY2))
    {
      Blynk.virtualWrite(V2, TOTAL2);
    }
    else
    {
      TOTAL2 += LS2;
      Blynk.virtualWrite(V2, TOTAL2);
    }
  }

  // Read flow sensor 3 data
  X3 = pulseIn(input3, HIGH);
  Y3 = pulseIn(input3, LOW);
  TIME3 = X3 + Y3;
  FREQUENCY3 = 1000000 / TIME3;
  WATER3 = FREQUENCY3 / 7.5;
  LS3 = WATER3 / 60;

  if (FREQUENCY3 >= 0)
  {
    if (isinf(FREQUENCY3))
    {
      Blynk.virtualWrite(V3, TOTAL3);
    }
    else
    {
      TOTAL3 += LS3;
      Blynk.virtualWrite(V3, TOTAL3);
    }
  }

  elapsedTime = millis() - startTime;  // Calculate the elapsed time
  // Check if activation time has passed and call the activateFunction
  if (!isActivated && elapsedTime >= 30L * 60L * 1000L)  // 30 minutes
  {
    activateFunction();
    isActivated = true;
  }
}

// Function to send an SMS with the total flow values for each apartment
void activateFunction()
{
  Serial2.println("AT+CMGF=1");
  updateSerial();
  Serial2.println("AT+CMGS=\"+YOUR_PHONE_NUMBER\"");  // Specify the recipient's phone number
  updateSerial();
  Serial2.print("for Owner: Apartment 1 = " + String(TOTAL1) + ", Apartment 2 = " + String(TOTAL2) + ", Apartment 3 = " + String(TOTAL3));
  updateSerial();
}

// Blynk virtual pin callback function triggered by button-state change
BLYNK_WRITE(V4)
{
  if (param.asInt())
  {
    // Reset flow sensor data to zero
    TIME1 = 0, TIME2 = 0, TIME3 = 0;
    FREQUENCY1 = 0, FREQUENCY2 = 0, FREQUENCY3 = 0;
    WATER1 = 0, WATER2 = 0, WATER3 = 0;
    TOTAL1 = 0, TOTAL2 = 0, TOTAL3 = 0;
    LS1 = 0, LS2 = 0, LS3 = 0;
  }
}
