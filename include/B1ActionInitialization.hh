#ifndef B1ActionInitialization_h
#define B1ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "B1RunAction.hh"

//class B1PrimaryGeneratorAction;
//class B1EventAction;
//class B1SteppingAction;

/// Action initialization class.

class B1ActionInitialization : public G4VUserActionInitialization
{
   public:
      G4int              fRunNumber;
      //B1RunAction      * fRunAction     ;
      //B1EventAction     fEventAction   ;
      //B1SteppingAction  fSteppingAction;

   public:
      B1ActionInitialization(G4int rn = 0);
      virtual ~B1ActionInitialization();

      virtual void BuildForMaster() const;
      virtual void Build() const;
};
//______________________________________________________________________________

#endif

    
