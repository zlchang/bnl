#ifndef STMYEVENTTRIGGER
#include "TObject.h"
#include "StEvent/StTriggerId.h"

class StEmcTriggerSimu;
class StMyEventTrigger : public TObject{
 public:
  StMyEventTrigger(){
    JP0ID = 380401;
    JP1ID = 380402;
    JP2ID = 380403;
  }

  int operator()(const StTriggerId &data, StEmcTriggerSimu *simu);

  int operator()(StEmcTriggerSimu *simu);
  
 private:
  int JP0ID;
  int JP1ID;
  int JP2ID;
  ClassDef(StMyEventTrigger, 0);
};

#endif
