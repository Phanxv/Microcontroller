#include <Arduino.h>
#include "WiFi.h"
#include "wifi_conf.h"
#include "HTTPClient.h"

HTTPClient http_client;
// put function declarations here:
void wifi_scan_connect(void);

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  Serial.println("T-SIMCAM Wifi test");
  wifi_scan_connect();
}

void loop() {
  // put your main code here, to run repeatedly:
}

void wifi_scan_connect(void)
{
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    delay(100);
    Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
            delay(10);
        }
    }
    Serial.println("");
    WiFi.disconnect();

    uint32_t last_m = millis();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        vTaskDelay(100);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.printf("\r\n-- wifi connect success! --\r\n");
    Serial.printf("It takes %d milliseconds\r\n", millis() - last_m);
    delay(100);
    String rsp;
    bool is_get_http = false;
    do {
        http_client.begin("https://webhook.site/a14c2c98-9ccc-42f8-837f-af372ae4ed94");
        int http_code = http_client.GET();
        Serial.println(http_code);
        if (http_code > 0) {
            Serial.printf("HTTP get code: %d\n", http_code);
            if (http_code == HTTP_CODE_OK) {
                rsp = http_client.getString();
                Serial.println(rsp);
                is_get_http = true;
            } else {
                Serial.printf("fail to get http client,code:%d\n", http_code);
            }
        } else {
            Serial.println("HTTP GET failed. Try again");
        }
        delay(3000);
    } while (!is_get_http);
    // WiFi.disconnect();
    http_client.end();
}