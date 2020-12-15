#ifndef _TM1637_OBSERVER_H_
#define _TM1637_OBSERVER_H_

#include "ISubject.h"
#include "IObserver.h"

#include <SevenSegmentTM1637.h>
#include <SevenSegmentExtended.h>

class TM1637Observer : public IObserver
{
  public:
    TM1637Observer(ISubject* pSubject, uint8_t pClkPin, uint8_t pDataPin)
    : mSubject(pSubject)
    {
        pSubject->attach(this);
        mDisplay = new SevenSegmentExtended(pClkPin, pDataPin);

        mDisplay->begin();            // initializes the display
        mDisplay->setBacklight(30);
        mDisplay->print("INIT");
    }
    
    virtual ~TM1637Observer(){ }

    void update(const String& pMessage) override
    {
        Serial.print("Message arrived:  " ) ;
        Serial.println(pMessage);
        mDisplay->clear();
        mDisplay->print( pMessage );
    }
    
  private:
    ISubject* mSubject;
    SevenSegmentExtended *mDisplay;
    String mMessageFromSubject;

};

#endif
