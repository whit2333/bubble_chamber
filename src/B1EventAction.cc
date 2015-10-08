#include "B1EventAction.hh"
#include "B1Run.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"


B1EventAction::B1EventAction() : G4UserEventAction(), 
   fEdep(0.)
{ } 
//______________________________________________________________________________

B1EventAction::~B1EventAction()
{}
//______________________________________________________________________________

void B1EventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
}
//______________________________________________________________________________

void B1EventAction::EndOfEventAction(const G4Event*)
{   
  // Called after G4Run::RecordEvent
  // accumulate statistics in B1Run
  B1Run* run = static_cast<B1Run*>( G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  run->AddEdep(fEdep);
  //if( run->GetNumberOfEvent()%1000 == 0 ) std::cout << "EndOfEventAction\n";
}
//______________________________________________________________________________

