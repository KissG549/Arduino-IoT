#ifndef _DISPLAY_OBSERVER_H_
#define _DISPLAY_OBSERVER_H_
#include "ISubject.h"
#include "IObserver.h"
#include "EnvironmentSubject.h"

class DisplayObserver : public IObserver
{
  public:
    DisplayObserver(EnvironmentSubject& pSubject)
      : mSubject(pSubject)
    {
      mSubject.Attach(this);
    }
    
    virtual ~DisplayObserver()
    {
        Serial.print("Good bye, I was the Observer\n");
    }

    void Update(const String& pMessage) override
    {
        mMessageFromSubject = pMessage;
    }

    void RemoveMeFromTheList()
    {
      mSubject.Detach(this);
      Serial.print("Observer removed from the list.\n");
    }

    void PrintInfo()
    {
      Serial.print("Observer, a new message is available: ");
      Serial.println( mMessageFromSubject ); 
    }
    
  private:
    EnvironmentSubject& mSubject;
    String mMessageFromSubject;
};

#endif
