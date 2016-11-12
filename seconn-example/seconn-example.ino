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

void printHex(void *vdata, size_t len) {
    char alphabet[] = "0123456789ABCDEF";
    uint8_t *data = (uint8_t*)vdata;
    Serial.print("0x");
    for(int i = 0; i < len; i++) {
        Serial.print(alphabet[(data[i] & 0xF0) >> 4]);
        Serial.print(alphabet[(data[i] & 0x0F)]);
    }
}

void c_seconn_state_changed(seconn_state prev, seconn_state cur) {
    Serial.print("State:");
    Serial.println(cur);
    if(cur == 4) {
        Serial.print("Pubkey of other side is: ");
        printHex(sconn.public_key, 64);
        Serial.println("");
    }
}

void setup() {
    Serial.begin(9600);
    while(!Serial);
    btinit();
    Serial.println("S!");
    seconn_init(&sconn, c_seconn_write_data, c_seconn_data_received,
    c_seconn_state_changed, &RNG, 0);

    Serial.print("Our pubkey is: ");
    uint8_t pubkey[64];
    seconn_get_public_key(&sconn, pubkey);
    printHex(pubkey, 64);
    Serial.println("");
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
