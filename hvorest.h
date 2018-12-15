#ifndef hvorest_h
#define hvorest_h

#include "Arduino.h"
#include <Ethernet.h>
#include "hvorestsettings.h"

struct Routes {
  char* name;
  void (*callback)(char* params, char* body, uint8_t* bodySize);
};

class HVORest {
  public:
    HVORest();

    void addRoute(char* route, void (*f)(char*, char* params, uint8_t));

    void addData(char* name, char* val);
    void addData(char* name, int val);
    void addData(char* name, float val, uint8_t precision);
    void addData(char* name, char* val, bool quote);

    void handle(EthernetClient& client);

  private:
    Routes routes_[ROUTES_TOTAL];
    uint8_t routesIndex_;
    
    char buffer_[OUTPUT_BUFFER_SIZE];
    uint16_t bufferIndex_;

    String name;

    void analog(uint8_t pin);
    void digital(uint8_t pin);

    void process();
    void reset();
    void jsonOpen();
    void jsonClose();
    void addToBuffer(char* value);
    void sendBuffer(EthernetClient& client);

    void ftoa(double f, char* a, uint8_t precision);
};

#endif
