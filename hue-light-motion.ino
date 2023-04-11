#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include "secrets.h"

/*
 * Required Secrets
 * 
 * String HUE_IP              : IP address of hue lights
 * String HUE_USER_NAME       : meethue API username
 * const char* WIFI_SSID      : Wifi name
 * const char* WIFI_PASSWORD  : Wifi password
 */

// #define DEBUG 1
#define SENSOR_PIN 13
#define LIGHT_ON_DELAY 300000

// Check for movement again with the tolerance
unsigned long tolerance_delay = 30000;

bool motion_detected = false;
bool is_light_on = false;
bool keep_light_on = false;

unsigned long start_millis;
unsigned long current_millis;

void connect_to_wifi() {

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("Connected to the WiFi network"); 

}

void setup() {
    pinMode(SENSOR_PIN, INPUT);
    Serial.begin(9600);
    connect_to_wifi();
}

void control_light(byte light_id, bool state){

    // Confirm network status
    if (WiFi.status() != WL_CONNECTED) {
        connect_to_wifi();
    }

    HTTPClient http; 
    String http_request_url;

    http_request_url = "http://";
    http_request_url += HUE_IP;
    http_request_url += "/api/";
    http_request_url += HUE_USER_NAME;
    http_request_url += "/lights/";
    http_request_url += light_id;
    http_request_url += "/state";

    #ifdef DEBUG
        Serial.println(http_request_url);
    #endif

    http.begin(http_request_url);
    http.addHeader("Content-Type", "text/plain");
    
    String put_body;
    put_body = "{\"on\":";
    put_body += (state) ? "true" : "false";
    put_body += ",\"bri\": 200";
    put_body += "}";
  
    int http_response_code = http.PUT(put_body);
    
    if (http_response_code > 0) {
        String response = http.getString();
        Serial.println(http_response_code);
        Serial.println(response);          
    } else {
        Serial.print("Error on sending PUT Request: ");
        Serial.println(http_response_code);
    }

    http.end();
}

void loop() {

    // Read motion sensor
    motion_detected = (digitalRead(SENSOR_PIN) == HIGH);

    // Turn lights on
    if (motion_detected && !is_light_on) {
        control_light(1, true);
        control_light(2, true);
        is_light_on = true;
    }

    // Keep lights on for LIGHT_ON_DELAY
    if (is_light_on) {
        delay(LIGHT_ON_DELAY);
    }

    /*
    * Wait for some tolerance and detect motion during this time.
    * Don't turn lights off if motion detected 
    */

    keep_light_on = false;
    current_millis = millis();
    start_millis = current_millis;
      
    // Check for motion within tolerance_delay; Account for current_millis rolling back to 0
    while ((current_millis - start_millis < tolerance_delay) && (current_millis - start_millis) >= 0) {
        #ifdef DEBUG
            Serial.println(current_millis - start_millis);
        #endif
        
        motion_detected = (digitalRead(SENSOR_PIN) == HIGH);
        current_millis = millis();
        if (motion_detected) {
            keep_light_on = true;
            break;
        }
    }

    // Turn lights off
    if (is_light_on && !keep_light_on) {
        control_light(1, false);
        control_light(2, false);
        is_light_on = false;
    }
}
