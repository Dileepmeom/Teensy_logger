// this I have manually edited
// second edit at 1:26
// third edit on the local machine
#include <FlexCAN_T4.h>
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

int count = 0;
void setup(void) {
  //Serial.begin(115200);delay(10);
  //pinMode(6, OUTPUT); digitalWrite(6, LOW); /* optional tranceiver enable pin */
  Can0.begin();
  Can0.setBaudRate(500000);
  //Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(canSniff);
  Can0.mailboxStatus();
}

void canSniff(const CAN_message_t &msg) {
  if (msg.id == 0x7F1)
  {
    Serial.print("MB "); Serial.print(msg.mb);
    Serial.print("  OVERRUN: "); Serial.print(msg.flags.overrun);
    Serial.print("  LEN: "); Serial.print(msg.len);
    Serial.print(" EXT: "); Serial.print(msg.flags.extended);
    Serial.print(" TS: "); Serial.print(msg.timestamp);
    Serial.print(" ID: "); Serial.print(msg.id, HEX);
    Serial.print(" Buffer: ");
    for ( uint8_t i = 0; i < msg.len; i++ ) {
      Serial.print(msg.buf[i]); Serial.print(" ");
    } Serial.println();
  }
}

void loop() {
  Can0.events();
  

  static uint32_t timeout = millis();
  if ( millis() - timeout > 200 ) {
    CAN_message_t msg;
    msg.id = 0x7FE;
    for ( uint8_t i = 0; i < 7; i++ ) msg.buf[i] = i + 1;
    count += 1;
    msg.buf[7] = count;
    Can0.write(msg);
    timeout = millis();
  }

}
