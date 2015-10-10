#ifndef FakeSD_h
#define FakeSD_h 1

#include "G4VSensitiveDetector.hh"
#include "FakeSDHit.hh"
#include "B1Analysis.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class FakeSD : public G4VSensitiveDetector
{
  public:
     G4AnalysisManager * fAnalysisManager;

     G4int  fhForward_0 ;
     G4int  fhBackward_0;

     G4int  fhXvsE_all ;
     G4int  fhXvsE_gamma ;
     G4int  fhXvsE_not_gamma ;
     G4int  fhXvsE_n ;

     G4int  fhXY0_all;
     G4int  fhXY1_all;
     G4int  fhXY2_all;

     G4int  fhXY0_gamma;
     G4int  fhXY1_gamma;
     G4int  fhXY2_gamma;

     G4int  fhBackScatEnergy;
     G4int  fhForwardScatEnergy;
     G4int  fhBackScatXYEnergyWt;
     G4int  fhForwardScatXYEnergyWt;
     G4int  fhBackScatXYEnergyWt_1;
     G4int  fhForwardScatXYEnergyWt_1;

     G4int  fhBackScat_XY;
     G4int  fhForwardScat_XY;
     G4int  fhBackScat_XY_1;
     G4int  fhForwardScat_XY_1;

     G4int fhEDD_x_y0;
     G4int fhEDD_y_x0;
     G4int fhEDD_x_y0_1;
     G4int fhEDD_y_x0_1;

     G4int fhEDD_x_y0_2;
     G4int fhEDD_y_x0_2;

     G4int fhEDD_x_y0_3;
     G4int fhEDD_y_x0_3;

     G4int fhEDD_x_y0_4;
     G4int fhEDD_y_x0_4;

     G4int fhEDDNonIonizing_x_y0;
     G4int fhEDDNonIonizing_y_x0;

  public:
      FakeSD(G4String name);
      ~FakeSD();

      void Initialize(G4HCofThisEvent*HCE);
      G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      void EndOfEvent(G4HCofThisEvent*HCE);
      void clear();
      void DrawAll();
      void PrintAll();

  private:
      G4int HCID;
      FakeSDHitsCollection *hitsCollection;

};




#endif

