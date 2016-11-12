#include <string.h>
#include <stdint.h>
#include <SoftwareSerial.h>
#include <seconn.h>
#include <rng.h>

#define btRxPin 9
#define btTxPin 8

SoftwareSerial Bluetooth = SoftwareSerial(btRxPin, btTxPin);

struct seconn sconn;

void btinit() {
    Serial.begin(9600);
    while(!Serial);
    pinMode(btRxPin, INPUT);
    pinMode(btTxPin, OUTPUT);
    Bluetooth.begin(9600);
    delay(1000);
    Bluetooth.println("AT+RESET");
    delay(1000);
    Bluetooth.println("AT+UART=9600,0,0");
    Bluetooth.println("AT+NAME=seconn");
    Bluetooth.println("AT+PSWD=0000");
    Bluetooth.println("AT+ROLE=0");
    delay(1000);
    while(Bluetooth.available()) {
        Bluetooth.read();
    }
}

int c_seconn_write_data(void *src, size_t bytes) {
    Bluetooth.write((const char*)src, bytes);
    return bytes;
}

void c_seconn_data_received(void *src, size_t bytes) {
    Serial.print("D:>");
    Serial.write((const char*)src, bytes);
    Serial.println("<");
}

void c_seconn_state_changed(seconn_state prev, seconn_state cur) {
    Serial.print("State:");
    Serial.println(cur);
}

void setup() {
    Serial.begin(9600);
    while(!Serial);
    btinit();
    Serial.println("S!");
    seconn_init(&sconn, c_seconn_write_data, c_seconn_data_received,
    c_seconn_state_changed, &RNG, 0);
}

char c;
char buff[10];
void loop() {
    int i = 0;

    while(Bluetooth.available() && i < 10) {
        buff[i++] = Bluetooth.read();
    }

    if(i > 0) {
        seconn_new_data(&sconn, buff, i);
    }
}
