#include "hvorest.h"

HVORest::HVORest() : routesIndex_(0), bufferIndex_(0) {}

void HVORest::addRoute(char* method, char* route, void (*f)(char* params, char* body, uint8_t bodySize)) {
  routes_[routesIndex_].method = method;
  routes_[routesIndex_].name = route;
  routes_[routesIndex_].callback = f;
  routesIndex_++;
}

void HVORest::addData(char* name, char* val) {

}

void HVORest::addData(char* name, int val) {

}

void HVORest::addData(char* name, float val) {

}

void HVORest::handle(EthernetClient& client) {

}

void HVORest::check() {

}

void HVORest::reset() {

}

void HVORest::jsonOpen() {
  
}

void HVORest::jsonClose() {

}

void HVORest::addToBuffer(char* val) {
  for (int i = 0; i < strlen(val); i++) {
    buffer_[bufferIndex_++] = value[i];
  }
}

void HVORest::send(uint8_t chunkSize, uint8_t delay) {

}
