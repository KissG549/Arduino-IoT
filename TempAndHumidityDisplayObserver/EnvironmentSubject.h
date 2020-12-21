#ifndef _ENVIRONMENT_SUBJECT_H_
#define _ENVIRONMENT_SUBJECT_H_
#include "ISubject.h"
#include "IObserver.h"

const static uint8_t MAX_HANDLED_OBSERVERS = 4;

class EnvironmentSubject : public ISubject {
  public:
    EnvironmentSubject()
    {
      mNOfRegisteredObservers = 0;
    }
      
    virtual ~EnvironmentSubject()
    {
    }

    /**
        Subscription management methods.
    */
    // TODO convert to stl compatible
    void attach(IObserver *pObserver) override
    {
      if( mNOfRegisteredObservers >= MAX_HANDLED_OBSERVERS )
      {
        Serial.println("Can't attach new observer to the subject, cache is full! Please increase the static cache size!");
        return;  
      }
      
      mObserverArray[ mNOfRegisteredObservers ] = pObserver;
      Serial.println("Observer attached.");
      ++mNOfRegisteredObservers;
    }

    /**
        Notify every observer and pass the mMessage parameter.
    */
    void notify() override
    {
      Serial.println("Notify observers.");

      for( uint8_t idx = 0; 
            idx < mNOfRegisteredObservers;
            ++idx )
        {
            mObserverArray[ idx ]->update( mMessage );
        }
      
     /* for ( uint8_t idx = 0; idx < mObservers.size(); ++idx )
      {
        IObserver *obs = mObservers.at(idx);
        obs->update(mMessage);
      }
      */
    }

    /* @return Number of attached observers */
    uint8_t size()
    {
      return mNOfRegisteredObservers;
    }

    /* @pMessage will be passed to the observers with their Update function. */
    void createMessage(String pMessage)
    {
      mMessage = pMessage;
      notify();
    }

  private:
    String mMessage;
    //std::list<IObserver *> mObservers;
    IObserver *mObserverArray[MAX_HANDLED_OBSERVERS];
    char mNOfRegisteredObservers;
};

#endif
