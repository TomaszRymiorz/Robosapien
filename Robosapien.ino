#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti wifiMulti;
WiFiServer server(80);
HTTPClient http;
WiFiClient wifi;

// Body
#define StepForward       0xA6
#define StepBackward      0xA7
#define StepTurnRight     0xA0
#define StepTurnLeft      0xA8
#define WalkForward       0x86
#define WalkBackward      0x87
#define TurnRight         0x80
#define TurnLeft          0x88
#define LeanForward       0xAD
#define LeanBackward      0xA5
#define TiltRight         0x83
#define TiltLeft          0x8B
#define Stop              0x8E

// Left Arm
#define LeftArmUp         0x89
#define LeftArmDown       0x8C
#define LeftArmOut        0x8A
#define LeftArmIn         0x8D
#define LeftHandThump     0xA9
#define LeftHandThrow     0xAA
#define LeftHandPickup    0xAC
#define LeftHandSweep     0xC9
#define LeftHandStrike1   0xCD
#define LeftHandStrike2   0xCB
#define LeftHandStrike3   0xC8

// Right Arm
#define RightArmUp        0x81
#define RightArmDown      0x84
#define RightArmOut       0x82
#define RightArmIn        0x85
#define RightHandThump    0xA1
#define RightHandThrow    0xA2
#define RightHandPickup   0xA4
#define RightHandSweep    0xC1
#define RightHandStrike1  0xC5
#define RightHandStrike2  0xC3
#define RightHandStrike3  0xC0

// Others
#define PowerOff          0xD1
#define Sleep             0xA3
#define WakeUp            0xB1
#define Burp              0xC2
#define High5             0xC4
#define Bulldozer         0xC6
#define Oops              0xC7
#define Whistle           0xCA
#define TalkBack          0xCC
#define Roar              0xCE
#define AllDemo           0xD0
#define KarateSkits       0xD2
#define RudeSkits         0xD3
#define Dance             0xD4

#define NoOp              0xEF

int irIn = 5;
int irOut = 4;

volatile int robotBit = 9;
volatile int robotCommand;
boolean robotUsed = false;


void setup() {
  Serial.begin(9600);
  pinMode(irIn, INPUT);
  pinMode(irOut, OUTPUT);
  digitalWrite(irOut, HIGH);
  attachInterrupt(0, readCommand, RISING);
  robotCommand = NoOp;

  Serial.println("");
  Serial.println("Initiation Wi-Fi");
  if (initiatingWifi()) {
    Serial.println(" Wi-Fi initiation completed");
    sendAdressToServer();
  }
}

void loop() {
  robotLoop();
  restLoop();
}


boolean initiatingWifi() {
  wifiMulti.addAP("LOGIN", "PASSWORD");
  wifiMulti.addAP("LOGIN", "PASSWORD");
  wifiMulti.addAP("LOGIN", "PASSWORD");

  Serial.print(" Connecting to the AP ");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("  Connected to Wi-Fi " + WiFi.SSID());

  Serial.println(" Server startup");
  server.begin();
  Serial.println("  Arduino REST server was started");

  return (WiFi.status() == WL_CONNECTED);
}

void sendAdressToServer() {
  Serial.println("Sending an address to the server");
  String url = "http://";
  String values = "username=LOGIN&password=PASSWORD&dbname=DATABASE&table=robosapien&wifissid=" + WiFi.SSID() + "&address=" + WiFi.localIP().toString();
  String request = url + "?" + values;// + " HTTP/1.1";

  Serial.println(" " + request);
  http.begin(request);
  http.addHeader("Accept", "application/json, text/plain, */*");
  http.addHeader("Content-Type", "application/json;charset=utf-8");
  http.POST(request);
  Serial.println(" Data sent");
  Serial.print("  ");
  http.writeToStream(&Serial);
  Serial.println(" locations in the database");
  http.end();
}


void robotLoop() {
  if (!robotUsed && robotCommand != NoOp) {
    sendCommand(robotCommand);
    robotCommand = NoOp;
  }
}

void readCommand() {
  delayMicroseconds(833 + 208);
  int bit = digitalRead(irIn);

  if (robotBit == 9) {
    robotCommand = 0;
    robotBit = 0;
    robotUsed = true;
  }
  if (robotBit < 8) {
    robotCommand <<= 1;
    robotCommand |= bit;
  }
  robotBit++;
  if (robotBit == 9) robotUsed = false;
}

void sendCommand(int command) {
  robotUsed = true;
  digitalWrite(irOut, LOW);

  delayMicroseconds(8 * 833);
  for (int i = 0; i < 8; i++) {
    digitalWrite(irOut, HIGH);
    delayMicroseconds(833);
    if ((command & 128) != 0) delayMicroseconds(3 * 833);
    digitalWrite(irOut, LOW);
    delayMicroseconds(833);
    command <<= 1;
  }

  digitalWrite(irOut, HIGH);
  delay(250);
  robotUsed = false;
}

int bodyCommand(String request) {
  if (request.indexOf("/stepforward") != -1) {
    wifi.print(getFullResponse("I execute the command StepForward"));
    return StepForward;
  }
  else if (request.indexOf("/stepbackward") != -1) {
    wifi.print(getFullResponse("I execute the command StepBackward"));
    return StepBackward;
  }
  else if (request.indexOf("/stepturnright") != -1) {
    wifi.print(getFullResponse("I execute the command StepTurnRight"));
    return StepTurnRight;
  }
  else if (request.indexOf("/stepturnleft") != -1) {
    wifi.print(getFullResponse("I execute the command StepTurnLeft"));
    return StepTurnLeft;
  }
  else if (request.indexOf("/walkforward") != -1) {
    wifi.print(getFullResponse("I execute the command WalkForward"));
    return WalkForward;
  }
  else if (request.indexOf("/walkbackward") != -1) {
    wifi.print(getFullResponse("I execute the command WalkBackward"));
    return WalkBackward;
  }
  else if (request.indexOf("/turnright") != -1) {
    wifi.print(getFullResponse("I execute the command TurnRight"));
    return TurnRight;
  }
  else if (request.indexOf("/turnleft") != -1) {
    wifi.print(getFullResponse("I execute the command TurnLeft"));
    return TurnLeft;
  }
  else if (request.indexOf("/leanforward") != -1) {
    wifi.print(getFullResponse("I execute the command LeanForward"));
    return LeanForward;
  }
  else if (request.indexOf("/leanbackward") != -1) {
    wifi.print(getFullResponse("I execute the command LeanBackward"));
    return LeanBackward;
  }
  else if (request.indexOf("/tiltright") != -1) {
    wifi.print(getFullResponse("I execute the command TiltRight"));
    return TiltRight;
  }
  else if (request.indexOf("/tiltleft") != -1) {
    wifi.print(getFullResponse("I execute the command TiltLeft"));
    return TiltLeft;
  }
  else if (request.indexOf("/stop") != -1) {
    wifi.print(getFullResponse("I execute the command Stop"));
    return Stop;
  }
  else {
    wifi.print(getFullResponse("invalid request"));
    return NoOp;
  }
}

int leftHandCommand(String request) {
  if (request.indexOf("/armup") != -1) {
    wifi.print(getFullResponse("I execute the command LeftArmUp"));
    return LeftArmUp;
  }
  else if (request.indexOf("/armdown") != -1) {
    wifi.print(getFullResponse("I execute the command LeftArmDown"));
    return LeftArmDown;
  }
  else if (request.indexOf("/armout") != -1) {
    wifi.print(getFullResponse("I execute the command LeftArmOut"));
    return LeftArmOut;
  }
  else if (request.indexOf("/armin") != -1) {
    wifi.print(getFullResponse("I execute the command LeftArmIn"));
    return LeftArmIn;
  }
  else if (request.indexOf("/handthump") != -1) {
    wifi.print(getFullResponse("I execute the command LeftHandThump"));
    return LeftHandThump;
  }
  else if (request.indexOf("/handthrow") != -1) {
    wifi.print(getFullResponse("I execute the command LeftHandThrow"));
    return LeftHandThrow;
  }
  else if (request.indexOf("/handpickup") != -1) {
    wifi.print(getFullResponse("I execute the command LeftHandPickup"));
    return LeftHandPickup;
  }
  else if (request.indexOf("/handsweep") != -1) {
    wifi.print(getFullResponse("I execute the command LeftHandSweep"));
    return LeftHandSweep;
  }
  else if (request.indexOf("/handstrike1") != -1) {
    wifi.print(getFullResponse("I execute the command LeftHandStrike1"));
    return LeftHandStrike1;
  }
  else if (request.indexOf("/handstrike2") != -1) {
    wifi.print(getFullResponse("I execute the command LeftHandStrike2"));
    return LeftHandStrike2;
  }
  else if (request.indexOf("/handstrike3") != -1) {
    wifi.print(getFullResponse("I execute the command LeftHandStrike3"));
    return LeftHandStrike3;
  }
  else {
    wifi.print(getFullResponse("invalid request"));
    return NoOp;
  }
}

int rightHandCommand(String request) {
  if (request.indexOf("/armup") != -1) {
    wifi.print(getFullResponse("I execute the command RightArmUp"));
    return RightArmUp;
  }
  else if (request.indexOf("/armdown") != -1) {
    wifi.print(getFullResponse("I execute the command RightArmDown"));
    return RightArmDown;
  }
  else if (request.indexOf("/armout") != -1) {
    wifi.print(getFullResponse("I execute the command RightArmOut"));
    return RightArmOut;
  }
  else if (request.indexOf("/armin") != -1) {
    wifi.print(getFullResponse("I execute the command RightArmIn"));
    return RightArmIn;
  }
  else if (request.indexOf("/handthump") != -1) {
    wifi.print(getFullResponse("I execute the command RightHandThump"));
    return RightHandThump;
  }
  else if (request.indexOf("/handthrow") != -1) {
    wifi.print(getFullResponse("I execute the command RightHandThrow"));
    return RightHandThrow;
  }
  else if (request.indexOf("/handpickup") != -1) {
    wifi.print(getFullResponse("I execute the command RightHandPickup"));
    return RightHandPickup;
  }
  else if (request.indexOf("/handsweep") != -1) {
    wifi.print(getFullResponse("I execute the command RightHandSweep"));
    return RightHandSweep;
  }
  else if (request.indexOf("/handstrike1") != -1) {
    wifi.print(getFullResponse("I execute the command RightHandStrike1"));
    return RightHandStrike1;
  }
  else if (request.indexOf("/handstrike2") != -1) {
    wifi.print(getFullResponse("I execute the command RightHandStrike2"));
    return RightHandStrike2;
  }
  else if (request.indexOf("/handstrike3") != -1) {
    wifi.print(getFullResponse("I execute the command RightHandStrike3"));
    return RightHandStrike3;
  }
  else {
    wifi.print(getFullResponse("invalid request"));
    return NoOp;
  }
}

int programmedCommand(String request) {
  if (request.indexOf("/poweroff") != -1) {
    wifi.print(getFullResponse("I execute the command PowerOff"));
    return PowerOff;
  }
  else if (request.indexOf("/sleep") != -1) {
    wifi.print(getFullResponse("I execute the command Sleep"));
    return Sleep;
  }
  else if (request.indexOf("/wakeup") != -1) {
    wifi.print(getFullResponse("I execute the command WakeUp"));
    return WakeUp;
  }
  else if (request.indexOf("/burp") != -1) {
    wifi.print(getFullResponse("I execute the command Burp"));
    return Burp;
  }
  else if (request.indexOf("/high5") != -1) {
    wifi.print(getFullResponse("I execute the command High5"));
    return High5;
  }
  else if (request.indexOf("/bulldozer") != -1) {
    wifi.print(getFullResponse("I execute the command Bulldozer"));
    return Bulldozer;
  }
  else if (request.indexOf("/oops") != -1) {
    wifi.print(getFullResponse("I execute the command Oops"));
    return Oops;
  }
  else if (request.indexOf("/whistle") != -1) {
    wifi.print(getFullResponse("I execute the command Whistle"));
    return Whistle;
  }
  else if (request.indexOf("/talkback") != -1) {
    wifi.print(getFullResponse("I execute the command TalkBack"));
    return TalkBack;
  }
  else if (request.indexOf("/roar") != -1) {
    wifi.print(getFullResponse("I execute the command Roar"));
    return Roar;
  }
  else if (request.indexOf("/alldemo") != -1) {
    wifi.print(getFullResponse("I execute the command AllDemo"));
    return AllDemo;
  }
  else if (request.indexOf("/karateskits") != -1) {
    wifi.print(getFullResponse("I execute the command KarateSkits"));
    return KarateSkits;
  }
  else if (request.indexOf("/rudeskits") != -1) {
    wifi.print(getFullResponse("I execute the command RudeSkits"));
    return RudeSkits;
  }
  else if (request.indexOf("/dance") != -1) {
    wifi.print(getFullResponse("I execute the command Dance"));
    return Dance;
  }
  else {
    wifi.print(getFullResponse("invalid request"));
    return NoOp;
  }
}

void restLoop() {
  wifi = server.available();

  if (wifi) {
    while (!wifi.available()) {
      delay(1);
    }

    String request = wifi.readStringUntil('\r');
    wifi.flush();

    if (request.indexOf("/body") != -1) {
      robotCommand = bodyCommand(request);
    }
    else if (request.indexOf("/left") != -1) {
      robotCommand = leftHandCommand(request);
    }
    else if (request.indexOf("/right") != -1) {
      robotCommand = rightHandCommand(request);
    }
    else if (request.indexOf("/program") != -1) {
      robotCommand = programmedCommand(request);
    }
    else {
      wifi.print(getFullResponse("invalid request"));
      robotCommand = NoOp;
    }

    wifi.flush();
    delay(1);
  }
}

String getFullResponse(String request) {
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  s += request;
  s += "</html>\n";

  return s;
}
