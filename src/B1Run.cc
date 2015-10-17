#include "B1Run.hh"

#include "G4HCofThisEvent.hh"
#include "G4Event.hh"

B1Run::B1Run(G4int rn) : G4Run(),
  fRunNumber(rn), fEdep(0.), fEdep2(0.)
{
} 
//______________________________________________________________________________

B1Run::~B1Run()
{} 
//______________________________________________________________________________
 
void B1Run::Merge(const G4Run* run)
{
  const B1Run* localRun = static_cast<const B1Run*>(run);
  fEdep  += localRun->fEdep;
  fEdep2 += localRun->fEdep2;

  G4Run::Merge(run); 
} 
//______________________________________________________________________________

void B1Run::RecordEvent(const G4Event * evt)
{
   G4HCofThisEvent* HCE = evt->GetHCofThisEvent();

   if( numberOfEvent%50000 == 0 ) std::cout << numberOfEvent << std::endl;

   G4Run::RecordEvent(evt); // increments run number
}
//______________________________________________________________________________

void B1Run::AddEdep (G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep;
}
//______________________________________________________________________________


