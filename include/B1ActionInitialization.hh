#ifndef B1ActionInitialization_h
#define B1ActionInitialization_h 1
#include "BubbleChamberSim.h"

#include "G4VUserActionInitialization.hh"
#include "B1RunAction.hh"
#include "GPSConfig.hh"

//class B1PrimaryGeneratorAction;
//class B1EventAction;
//class B1SteppingAction;

/// Action initialization class.

class B1ActionInitialization : public G4VUserActionInitialization
{
private:
  GPSConfig*  gps_config = nullptr;

public:
  G4int              fRunNumber;
  double             fBeamEnergy;
  //B1RunAction      * fRunAction     ;
  //B1EventAction     fEventAction   ;
  //B1SteppingAction  fSteppingAction;
  void   SetBeamEnergy(double E) { fBeamEnergy = E; }
  double GetBeamEnergy() const { return fBeamEnergy; }

  Settings  sim_settings;


public:
  B1ActionInitialization(G4int rn = 0);
  B1ActionInitialization(const Settings& sim_set);
  virtual ~B1ActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;
};
//______________________________________________________________________________

#endif

    
