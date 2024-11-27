#include "Arduino.h"
#include "Arduino_BuiltIn.h"
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <ESP8266HTTPClient.h>
#include <dummy.h>
#include <Arduino_JSON.h>
#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>


HTTPClient http;
WiFiClient client;
char operate = 0, GetOperate = 0;
String password, money, user(1,'a'), username, buffer(30,'0');


String url2 = "http://182.92.11.87:5000/user";


void print(String a)
{
  Serial.write('@');
  Serial.println(a);
}
void GUET_WIFI()
{
  String url = "http://10.0.1.5:801/eportal/portal/login?user_account=%2C0%2C2400220230&user_password=&wlan_user_mac=5002917bdefe&wlan_ac_name=HJ-BRAS-ME60-01";
  WiFi.mode(WIFI_STA);
  WiFi.begin("GUET-WiFi", "");
  while(!WiFi.isConnected())
  {
    delay(100);
  }
  delay(1500);
  http.begin(client, url);
  http.addHeader("Host", "10.0.1.5");
  http.addHeader("User-Agent", "Mozilla/5.0 (Linux; Android 10; K) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Mobile Safari/537.36");
  http.addHeader("Referer", "http://10.0.1.5/");
  http.addHeader("Accept-Encoding", "gzip, deflate");
  int httpCode = http.GET();
  http.end();
}  //致敬我自己的项目 Fuck-GUET-WiFi

void Mi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin("Xiaomi 14 Pro", "");
  while(!WiFi.isConnected())
  {
    delay(100);
  }
  delay(1000);
}

void POST(String user, String username, String password, String money)
{
  String body = "{\"name\":\"" +username + "\",\"password\":\"" + password + "\",\"money\":" + money + "}" ;
  HTTPClient httpost;
  httpost.begin(client, url2 + user + "/post");
  httpost.addHeader("Host", "182.92.11.87");
  httpost.addHeader("User-Agent", "Mozilla/5.0 (Linux; Android 10; K) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Mobile Safari/537.36");
  httpost.addHeader("Content-Type", "application/json");
  int HTTPCode = httpost.POST(body);
  httpost.end();
  print("~");
}

void GetInfo(String user)
{
  HTTPClient httpget;
  httpget.begin(client, url2 + user + "/info");
  httpget.addHeader("Host", "182.92.11.87");
  httpget.addHeader("User-Agent", "Mozilla/5.0 (Linux; Android 10; K) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Mobile Safari/537.36");
  httpget.addHeader("Referer", "http://182.92.11.87:5000/");
  httpget.addHeader("Accept-Encoding", "gzip, deflate");
  int HTTPCode = httpget.GET();
  print(httpget.getString());
  httpget.end();
}

void GetList(void)
{
  HTTPClient httpget;
  httpget.begin(client, "http://182.92.11.87:5000/list");
  httpget.addHeader("Host", "182.92.11.87");
  httpget.addHeader("User-Agent", "Mozilla/5.0 (Linux; Android 10; K) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Mobile Safari/537.36");
  httpget.addHeader("Referer", "http://182.92.11.87:5000/");
  httpget.addHeader("Accept-Encoding", "gzip, deflate");
  int HTTPCode = httpget.GET();
  print(httpget.getString());
  httpget.end();
}

void GetTime(void)
{
  HTTPClient httpget;
  httpget.begin(client, "http://182.92.11.87:5000/time");
  httpget.addHeader("Host", "182.92.11.87");
  httpget.addHeader("User-Agent", "Mozilla/5.0 (Linux; Android 10; K) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Mobile Safari/537.36");
  httpget.addHeader("Referer", "http://182.92.11.87:5000/");
  httpget.addHeader("Accept-Encoding", "gzip, deflate");
  int HTTPCode = httpget.GET();
  print(httpget.getString());
  httpget.end();
}

void DeleteUser(String user)
{
  HTTPClient httpdel;
  httpdel.begin(client, url2 + user + "/delete");
  int HTTPCode = httpdel.DELETE();
  httpdel.end();
  print("~");
}

void setup() 
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  //GUET_WIFI();
  Mi();
  
  for(int i=0; i<3; i++)
  {
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    delay(100);
  }
  print("?");
}
  
void loop() 
{
  buffer = Serial.readStringUntil('!');
  if(buffer)
  {
    operate = buffer[0];
    user = buffer[1];
    if(operate == 'P')//发送POST请求
    {
      for(int i = 2; i < 8; i++)
      {
        username += buffer[i];
      }
      for(int i = 8; i < 14; i++)
      {
        password += buffer[i];
      }
      for(int i = 14; buffer[i] != '?';i++)
      {
        money += buffer[i];
      }
      POST(user, username, password, money);
      digitalWrite(2, HIGH);
      delay(50);
      digitalWrite(2, LOW);
      username = ""; password = ""; money = "";
    }
    else if(operate == 'G')//发送GET请求
    {
      GetOperate = buffer[2];
      GetInfo(user);
      GetOperate = 0;
      digitalWrite(2, HIGH);
      delay(50);
      digitalWrite(2, LOW);
    }
    else if(operate == 'D')//发送DELETE请求
    {
      DeleteUser(user);
      digitalWrite(2, HIGH);
      delay(50);
      digitalWrite(2, LOW);
    }
    else if(operate == 'L')
    {
      GetList();
      digitalWrite(2, HIGH);
      delay(50);
      digitalWrite(2, LOW);
    }
    else if(operate == 'T')
    {
      GetTime();
      digitalWrite(2, HIGH);
      delay(50);
      digitalWrite(2, LOW);
    }
    else if(operate == 'S')
    {
      if(user == "0")
      {
        digitalWrite(2, LOW);
      }
      else if(user == "1")
      {
        digitalWrite(2, HIGH);
      }
    }
    operate = 0;user = "a";
  }
  buffer = "";
}