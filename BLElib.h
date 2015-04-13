/*
  sam.h - Library for testing libraries in Arduino.
  Created by BLElib Fallahi 10 March, 2015.
*/

#ifndef BLELIB_H
#define BLELIB_H

#include <Arduino.h>

class BLElib{
  public:
    BLElib(Stream* serial_stream);
    void sendCMD(String command);
    bool bufferIsEmpty();
    String getBuffer();
    String cmd(String input);
    bool cmdIsValid(String buffer);
    String cleanBuffer(String buffer);
    void resetDevice();
    void emptyBuffer();
    bool isDiscoverable();
    void makeDiscoverable();
    bool contains(String str, String sub_str);
    bool setShortName(String sName);
    void autoconnect();
  private:
    Stream *stream;
};

#endif