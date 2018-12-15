#include "hvorest.h"

HVORest::HVORest() : routesIndex_(0), bufferIndex_(0) {
  addRoute("/analog", analog);
  addRoute("/digital", digital);
}

void HVORest::addRoute(char* route, void (*f)(char* params, char* body, uint8_t bodySize)) {
  routes_[routesIndex_].name = route;
  routes_[routesIndex_].callback = f;
  routesIndex_++;
}

void HVORest::addData(char* name, char* val) {
  addData(name, val, true);
}

void HVORest::addData(char* name, int val) {
  char str[10];
  itoa(val, str);
  addData(name, str, false);
}

void HVORest::addData(char* name, float val) {
  char str[20];
  ftoa(val, str, precision);
  addData(name, str, false);
}

void HVORest::addData(char* name, char* val, bool quote) {
  addToBuffer(F("\""));
  addToBuffer(name);
  addToBuffer(F("\": "));
  if (quote) {
    addToBuffer(F("\""));
  }
  addToBuffer(val);
  if (quote) {
    addToBuffer(F("\""));
  }
  addToBuffer(F(", "));
}

void HVORest::handle(EthernetClient& client) {
  if (client.available()) {
    // Start output
    jsonOpen();

    // Process the input
    process();

    // Finish output
    jsonClose();

    // Send result back
    send();

    // Disconnect and reset buffer
    client.stop();
    reset();
  }
}

void HVORest::analog(uint8_t pin) {
  addData(F("Value"), analogRead(pin));
  addData(F("PinNumber"), pin);
}

void HVORest::digital(uint8_t pin) {
  addData(F("Value"), digitalRead(pin));
  addData(F("PinNumber"), pin);
}

void HVORest::process() {

}

void HVORest::reset() {
  memset(&buffer_[0], 0, sizeof(buffer_));
  bufferIndex_ = 0;
}

void HVORest::jsonOpen() {
  addToBuffer(F("{ "));
}

void HVORest::jsonClose() {
  addToBuffer(F("\"name\": \""));
  addToBuffer(name);
  addToBuffer(F("\" }"));
}

void HVORest::addToBuffer(char* val) {
  for (int i = 0; i < strlen(val); i++) {
    buffer_[bufferIndex_++] = value[i];
  }
}

void HVORest::sendBuffer(EthernetClient& client) {
  // Send HTTP Headers
  client.println(HTTP_COMMON_HEADERS);

  // Send everything else
  client.print(buffer_);
}

void HVORest::ftoa(double f, char* a, uint8_t precision) {
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};

  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
}
