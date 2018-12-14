#ifndef hvorest_h
#define hvorest_h

#include "Arduino.h"
#include <Ethernet.h>
#include "hvorestsettings.h"

struct Routes {
  char* method;
  char* name;
  void (*callback)(char* params, char* body, uint8_t* bodySize);
};

class HVORest {
  public:
    HVORest(EthernetServer& client);

    void addRoute(char* method, char* route, void (*f)(char*, char* params, uint8_t));

    void addData(char* name, char* val);
    void addData(char* name, int val);
    void addData(char* name, float val);

    void handle(EthernetClient& client);

  private:
    Routes routes_[ROUTES_TOTAL];
    uint8_t routesIndex_;
    
    char buffer_[OUTPUT_BUFFER_SIZE];
    uint16_t bufferIndex_;
    String name

    void check();
    void reset();
    void jsonOpen();
    void jsonClose();
    void addToBuffer(char* value);
    void send(uint8_t chunkSize, uint8_t delay);
};

#endif
