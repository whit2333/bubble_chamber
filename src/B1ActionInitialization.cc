#include "B1ActionInitialization.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "SimplePrimaryGeneratorAction.hh"
#include "B1RunAction.hh"
#include "B1EventAction.hh"
#include "B1SteppingAction.hh"
#include "StackingAction.hh"


B1ActionInitialization::B1ActionInitialization(G4int rn) : G4VUserActionInitialization(),
   fRunNumber(rn)
{ }
//______________________________________________________________________________
B1ActionInitialization::B1ActionInitialization(const Settings& sim_set) : G4VUserActionInitialization(),
   fRunNumber(sim_set.run_number)
{
  sim_settings = sim_set;
  PrintSettings(sim_settings);
}
//______________________________________________________________________________


B1ActionInitialization::~B1ActionInitialization()
{ }
//______________________________________________________________________________

void B1ActionInitialization::BuildForMaster() const
{
   
   SetUserAction(new B1RunAction(fRunNumber));
}
//______________________________________________________________________________

void B1ActionInitialization::Build() const
{
  auto primary_action = new B1PrimaryGeneratorAction();
  //auto primary_action = new SimplePrimaryGeneratorAction();
  
  using namespace CLHEP;
  primary_action->SetBeamEnergy(fBeamEnergy);
  primary_action->fEnergy_sig   = sim_settings.delta_E *MeV;
  primary_action->fVertex = G4ThreeVector(sim_settings.vertex.at(0)*mm,
                                          sim_settings.vertex.at(1)*mm,
                                          sim_settings.vertex.at(2)*mm);
  primary_action->fVertex_x_sig = sim_settings.vertex_rms.at(0)*mm;
  primary_action->fVertex_y_sig = sim_settings.vertex_rms.at(1)*mm;
  primary_action->InitRand();

  SetUserAction(primary_action);
  SetUserAction(new B1RunAction(fRunNumber));

  B1EventAction* eventAction = new B1EventAction;
  SetUserAction(eventAction);

  SetUserAction(new B1SteppingAction(eventAction));

  SetUserAction(new StackingAction());

}  
//______________________________________________________________________________

