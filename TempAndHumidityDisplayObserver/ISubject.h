#ifndef _ISUBJECT_H_
#define _ISUBJECT_H_
/**
* Observer Design Pattern 
* Based on https://refactoring.guru/design-patterns/observer/cpp/example
* 
*/
class IObserver;

class ISubject {
  public:
    virtual ~ISubject(){};
    virtual void attach(IObserver* observer) = 0;
    virtual void notify() = 0;
};
#endif
