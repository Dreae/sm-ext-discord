#pragma once

#include "include/smsdk_ext.h"
#include "include/rust.h"

#define LOG_MESSAGE(format, ...) \
  smutils->LogMessage(myself, format, ##__VA_ARGS__);

class Extension : public SDKExtension {
public:
  virtual bool SDK_OnLoad(char *error, size_t err_max, bool late);
  virtual void SDK_OnUnload();
};

class BaseClass {
  friend class Extension;
public:
  BaseClass() {
    next = BaseClass::head;
    BaseClass::head = this;
  }
  virtual void OnExtLoad() { };
  virtual void OnExtUnload() { };
private:
  BaseClass *next;
  static BaseClass *head;
};

extern Extension extension;
