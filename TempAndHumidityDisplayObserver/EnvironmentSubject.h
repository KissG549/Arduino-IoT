#ifndef _ENVIRONMENT_SUBJECT_H_
#define _ENVIRONMENT_SUBJECT_H_
#include "ISubject.h"
#include "IObserver.h"

class EnvironmentSubject : public ISubject {
   public:
    virtual ~EnvironmentSubject()
    {
        Serial.print("Good bye, I was the Subject\n");
    }

    /**
     *  Subscription management methods
     */
    void Attach(IObserver *observer) override 
    {
     
    }
    
    void Detach(IObserver *observer) override 
    {
     
    }
    
    void Notify() override
    {
    }

    /* @return Number of attached observers */
    void Size()
    {
    }

    void BusinessLogic() 
    {
      Notify();  
    }

  private:
    String mMessage;
    
 };

#endif
