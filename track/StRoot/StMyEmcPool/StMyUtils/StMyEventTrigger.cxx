#include "StMyEventTrigger.h"
#include "StTriggerUtilities/StTriggerSimuMaker.h"
#include "StTriggerUtilities/Emc/StEmcTriggerSimu.h"

int StMyEventTrigger::operator()(const StTriggerId &data, StEmcTriggerSimu *simu){
      if(simu->JP2() && data.isTrigger(JP2ID))
      return 2;
    else if(simu->JP1() && data.isTrigger(JP1ID))
      return 1;
    else if(simu->JP0() && data.isTrigger(JP0ID))
      return 0;
    else 
      return -1;
}
int StMyEventTrigger::operator()(StEmcTriggerSimu *simu){
    if(simu->JP2())
      return 2;
    else if(simu->JP1())
      return 1;
    else if(simu->JP0())
      return 0;
    else 
      return -1;
}
