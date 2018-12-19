#ifndef hvorest_h
#define hvorest_h

#include "Arduino.h"
#include <Ethernet.h>

#ifndef ROUTES_TOTAL
#define ROUTES_TOTAL 10
#endif

#ifndef HTTP_COMMON_HEADERS
#define HTTP_COMMON_HEADERS "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET\r\nContent-Type: application/json\r\nConnection: close\r\n"
#endif

#ifndef OUTPUT_BUFFER_SIZE
#define OUTPUT_BUFFER_SIZE 500
#endif

struct Routes {
  char* name;
  void (*callback)(char* params);
};

class HVORest {
  public:
    HVORest();

    void addRoute(char* route, void (*f)(uint8_t));

    void addData(char* name, char* val);
    void addData(char* name, int val);
    void addData(char* name, float val, uint8_t precision);
    void addData(char* name, char* val, bool quote);

    void handle(EthernetClient& client);

    void setName(String n);

  private:
    Routes routes_[ROUTES_TOTAL];
    uint8_t routesIndex_;
    
    char buffer_[OUTPUT_BUFFER_SIZE];
    uint16_t bufferIndex_;

    String name_;
    String route_;
    String sPin_;

    void process(EthernetClient* client);
    void reset();
    void jsonOpen();
    void jsonClose();
    void addToBuffer(char* value);
    void addToBuffer(const __FlashStringHelper* value);
    void sendBuffer(EthernetClient* client);

    void ftoa(double f, char* a, uint8_t precision);
};

#endif
