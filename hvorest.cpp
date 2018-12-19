#include "hvorest.h"

HVORest::HVORest() : routesIndex_(0), bufferIndex_(0) {}

void HVORest::addRoute(char* route, void (*f)(uint8_t pinNum)) {
  routes_[routesIndex_].name = route;
  routes_[routesIndex_].callback = f;
  routesIndex_++;
}

void HVORest::addData(char* name, char* val) {
  addData(name, val, true);
}

void HVORest::addData(char* name, int val) {
  char str[10];
  itoa(val, str, 10);
  addData(name, str, false);
}

void HVORest::addData(char* name, float val, uint8_t precision) {
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
    process(&client);

    // Finish output
    jsonClose();

    // Send result back
    sendBuffer(&client);

    // Disconnect and reset buffer
    client.stop();
    reset();
  }
}

void HVORest::setName(String n) {
  name_ = n;
}

void HVORest::process(EthernetClient* client) {
  bool gettingRoute = false;
  bool gettingParams = false;

  char c;
  while (client->connected() && client->available()) {
    c = client->read();
    
    if (c == '/') {
      gettingRoute = !gettingRoute;
      if (strcmp(route_.c_str(), "") != 0) {
        // Function name has ended, need to get param now
        gettingParams = true;
      }
      continue;
    }

    if (c == ' ') {
      if (gettingRoute || gettingParams) {
        // Route has ended, call function
        for (uint8_t i = 0; i < routesIndex_; i++) {
          if (strncmp(route_.c_str(), routes_[i].name, sizeof(routes_[i].name)) != 0) {
            continue;
          }

          if (strcmp(sPin_.c_str(), "") == 0)
            sPin_ = "1";
          routes_[i].callback(atoi(sPin_.c_str()));
        }
        break;
      }
      continue;
    }

    if (gettingRoute) {
      route_ += c;
    }

    if (gettingParams) {
      sPin_ += c;
    }
  }
}

void HVORest::reset() {
  memset(&buffer_[0], 0, sizeof(buffer_));
  bufferIndex_ = 0;
  route_ = "";
  sPin_ = "";
}

void HVORest::jsonOpen() {
  addToBuffer(F("{ "));
}

void HVORest::jsonClose() {
  addToBuffer(F("\"name\": \""));
  addToBuffer(name_.c_str());
  addToBuffer(F("\" }"));
}

void HVORest::addToBuffer(char* val) {
  for (int i = 0; i < strlen(val); i++) {
    buffer_[bufferIndex_++] = val[i];
  }
}

void HVORest::addToBuffer(const __FlashStringHelper* val) {
  uint8_t idx = 0;
  PGM_P p = reinterpret_cast<PGM_P>(val);

  for (unsigned char c = pgm_read_byte(p++);
       c != 0 && bufferIndex_ < OUTPUT_BUFFER_SIZE;
       c = pgm_read_byte(p++), bufferIndex_++) {
    buffer_[bufferIndex_] = c;
  }
}

void HVORest::sendBuffer(EthernetClient* client) {
  // Send HTTP Headers
  client->println(HTTP_COMMON_HEADERS);

  // Send everything else
  client->print(buffer_);
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
