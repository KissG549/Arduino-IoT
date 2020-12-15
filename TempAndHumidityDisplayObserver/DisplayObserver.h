#ifndef _DISPLAY_OBSERVER_H_
#define _DISPLAY_OBSERVER_H_
#include "ISubject.h"
#include "IObserver.h"

class DisplayObserver : public IObserver
{
  public:
    DisplayObserver(ISubject* pSubject)
      : mSubject( pSubject )
    {
        mSubject->attach(this);
        static uint8_t id = 0;
        mId = id++;
        Serial.print("Observer id: " );
        Serial.println( mId );
    }
    
    virtual ~DisplayObserver()
    {
        Serial.print("Good bye, I was the Observer\n");
    }

    void update(const String& pMessage) override
    {
        mMessageFromSubject = pMessage;
        Serial.print("Message arrived:  " ) ;
        Serial.print(mId);
        Serial.print(" : ");
        Serial.println(pMessage);
    }

    void printInfo()
    {
      Serial.print("Observer, a new message is available: ");
      Serial.println( mMessageFromSubject ); 
    }
    
  private:
    ISubject* mSubject;
    String mMessageFromSubject;
    uint8_t mId;
};

#endif
