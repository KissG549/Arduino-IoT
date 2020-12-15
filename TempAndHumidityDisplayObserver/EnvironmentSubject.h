#ifndef _ENVIRONMENT_SUBJECT_H_
#define _ENVIRONMENT_SUBJECT_H_
#include "ISubject.h"
#include "IObserver.h"
#include "LinkedList.h"

class EnvironmentSubject : public ISubject {
   public:
    virtual ~EnvironmentSubject()
    {
        Serial.println("Good bye, I was the Subject.");
    }

    /**
     *  Subscription management methods.
     */
    void attach(IObserver *pObserver) override 
    {
      mObservers.insert(pObserver);
      Serial.println("Observer attached.");
    }

    /**
     *  Notify every observer and pass the mMessage parameter. 
     */
    void notify() override
    {
      Serial.println("Notify observers.");
      for( uint8_t idx = 0; idx < mObservers.size(); ++idx )
      {
          IObserver *obs = mObservers.at(idx);
          obs->update(mMessage);
      }
    }

    /* @return Number of attached observers */
    uint8_t size()
    {
      return mObservers.size();
    }

    /* @pMessage will be passed to the observers with their Update function. */
    void createMessage(String pMessage)
    {
      mMessage = pMessage;
      notify();
    }

  private:
    String mMessage;
    LinkedList<IObserver *> mObservers; 
 };

#endif
