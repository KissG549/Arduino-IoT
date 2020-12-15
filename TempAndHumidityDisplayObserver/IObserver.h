#ifndef _IOBSERVER_H_
#define _IOBSERVER_H_
/**
* Observer Design Pattern 
* Based on https://refactoring.guru/design-patterns/observer/cpp/example
* 
*/
class IObserver {
  
  public:
    virtual ~IObserver(){};
    virtual void update(const String& pMessage) = 0;
};

#endif
