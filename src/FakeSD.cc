#include "FakeSD.hh"
#include "FakeSDHit.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "B1Run.hh"

FakeSD::FakeSD(G4String name) : G4VSensitiveDetector(name)
{
   G4String HCname;
   collectionName.insert(HCname="hitsCollection");

   HCID = -1;

   fAnalysisManager = G4AnalysisManager::Instance();


   double hist_Emax = 8;
   // Creating histograms
   fhBackward_0            = fAnalysisManager->CreateH1(name+"/back0","Backward scattered energies", 100,0,hist_Emax);
   fhForward_0             = fAnalysisManager->CreateH1(name+"/forw0","Forward scattered energies",  100,0,hist_Emax);

   fhXvsE_all    = fAnalysisManager->CreateH2(name+"/fhXvsE_all","E vs X all",     100,-10,10,100,0,hist_Emax);
   fhXvsE_gamma  = fAnalysisManager->CreateH2(name+"/fhXvsE_gamma","E vs X gamma", 100,-10,10,100,0,hist_Emax);
   fhXvsE_not_gamma  = fAnalysisManager->CreateH2(name+"/fhXvsE_not_gamma","E vs X not gamma", 100,-10,10,100,0,hist_Emax);
   fhXvsE_n      = fAnalysisManager->CreateH2(name+"/fhXvsE_n","E vs X n",         100,-10,10,100,0,hist_Emax);

   fhXY0_all    = fAnalysisManager->CreateH2(name+"/fhXY0_all","E vs X all",     100,-10,10,100,-10,10);
   fhXY1_all    = fAnalysisManager->CreateH2(name+"/fhXY1_all","E vs X all",     100,-5,5,100,-5,5);
   fhXY2_all    = fAnalysisManager->CreateH2(name+"/fhXY2_all","E vs X all",     100,-2,2,100,-2,2);

   fhXY0_gamma    = fAnalysisManager->CreateH2(name+"/fhXY0_gamma","E vs X gamma",     100,-10,10,100,-10,10);
   fhXY1_gamma    = fAnalysisManager->CreateH2(name+"/fhXY1_gamma","E vs X gamma",     100,-5,5,100,-5,5);
   fhXY2_gamma    = fAnalysisManager->CreateH2(name+"/fhXY2_gamma","E vs X gamma",     100,-2,2,100,-2,2);

   //fhBackScatXYEnergyWt    = fAnalysisManager->CreateH2(name+"/fhBackScatXYEnergyWt","Backward XY energy wts ",         100,-20,20,200,-20,20);
   //fhForwardScatXYEnergyWt = fAnalysisManager->CreateH2(name+"/fhForwardScatXYEnergyWt","Forwardward XY energy wts ",   100,-20,20,200,-20,20);

   //fhBackScatXYEnergyWt_1    = fAnalysisManager->CreateH2(name+"/fhBackScatXYEnergyWt_1","Backward XY energy wts ",       200,-3,3,200,-3,3);
   //fhForwardScatXYEnergyWt_1 = fAnalysisManager->CreateH2(name+"/fhForwardScatXYEnergyWt_1","Forward scattering zoomed ", 200,-0.01,0.01,200,-0.01,0.01);

   //fhBackScat_XY       = fAnalysisManager->CreateH2(name+"/fhBackScat_XY",    "Backward XY",     200,-20,20, 200,-20,20);
   //fhForwardScat_XY    = fAnalysisManager->CreateH2(name+"/fhForwardScat_XY", "Forwardward XY",  200, -1, 1, 200, -1, 1);
   //fhBackScat_XY_1     = fAnalysisManager->CreateH2(name+"/fhBackScat_XY_1",    "Backward XY",     200,-1,1, 200,-1,1);
   //fhForwardScat_XY_1  = fAnalysisManager->CreateH2(name+"/fhForwardScat_XY_1", "Forwardward XY",  200, -0.01, 0.01, 200, -0.01, 0.01);

   //fhEDD_x_y0    = fAnalysisManager->CreateH2(name+"/fhEDD_x_y0", "EDD along y0 ",  200, -60, 60, 200, 0.0,120.0);
   //fhEDD_y_x0    = fAnalysisManager->CreateH2(name+"/fhEDD_y_x0", "EDD along x0 ",  200, -60, 60, 200, 0.0,120.0);
   //fhEDD_x_y0_1  = fAnalysisManager->CreateH2(name+"/fhEDD_x_y0_1", "EDD along y0 ",  200, -5, 5, 200, 0.0,120.0);
   //fhEDD_y_x0_1  = fAnalysisManager->CreateH2(name+"/fhEDD_y_x0_1", "EDD along x0 ",  200, -5, 5, 200, 0.0,120.0);
   //fhEDD_x_y0_2  = fAnalysisManager->CreateH2(name+"/fhEDD_x_y0_2", "EDD along y0 ",  200, -5, 5, 100, 0.0,120.0);
   //fhEDD_y_x0_2  = fAnalysisManager->CreateH2(name+"/fhEDD_y_x0_2", "EDD along x0 ",  200, -5, 5, 100, 0.0,120.0);
   //fhEDD_x_y0_3  = fAnalysisManager->CreateH2(name+"/fhEDD_x_y0_3", "EDD along y0 ",  200, -2, 2, 200, 0.0,120.0);
   //fhEDD_y_x0_3  = fAnalysisManager->CreateH2(name+"/fhEDD_y_x0_3", "EDD along x0 ",  200, -2, 2, 200, 0.0,120.0);

   //fhEDD_x_y0_4  = fAnalysisManager->CreateH2(name+"/fhEDD_x_y0_4", "EDD along y0 ",  100, -2, 2, 20, 0.0,120.0);
   //fhEDD_y_x0_4  = fAnalysisManager->CreateH2(name+"/fhEDD_y_x0_4", "EDD along x0 ",  100, -2, 2, 20, 0.0,120.0);

   //fhEDDNonIonizing_x_y0  = fAnalysisManager->CreateH2(name+"/fhEDDNI_x_y0", "Non Ionizing EDD along y0 ",  200, -5, 5, 200,0.0, 120.0);
   //fhEDDNonIonizing_y_x0  = fAnalysisManager->CreateH2(name+"/fhEDDNI_y_x0", "Non Ionizing EDD along x0 ",  200, -5, 5, 200,0.0, 120.0);

   //if(fhEDD_x_y0 < 0 ) std::cout << "bad fhEDD_x_y0" << fhEDD_x_y0 << std::endl;
   //if(fhEDD_y_x0 < 0 ) std::cout << "bad fhEDD_y_x0" << fhEDD_y_x0 << std::endl;
   //if(fhEDDNonIonizing_x_y0 < 0 ) std::cout << "bad fhEDDNonIonizing_x_y0" << fhEDDNonIonizing_x_y0 << std::endl;
   //if(fhEDDNonIonizing_y_x0 < 0 ) {
   //   std::cout << "bad fhEDDNonIonizing_y_x0" << fhEDDNonIonizing_y_x0 << std::endl;
   //   exit(-1);
   //}
}
//______________________________________________________________________________

FakeSD::~FakeSD()
{ }
//______________________________________________________________________________

void FakeSD::Initialize(G4HCofThisEvent* HCE)
{
   hitsCollection = new FakeSDHitsCollection(SensitiveDetectorName,collectionName[0]); 

   if(HCID<0) { HCID = GetCollectionID(0); }
   HCE->AddHitsCollection(HCID,hitsCollection);
}
//______________________________________________________________________________

G4bool FakeSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // Fill histograms

   G4StepPoint* preStep = aStep->GetPreStepPoint();
   G4TouchableHistory* touchable = (G4TouchableHistory*)(preStep->GetTouchable());

   double pz           = aStep->GetPreStepPoint()->GetMomentum().z();
   double energy       = aStep->GetPreStepPoint()->GetKineticEnergy()/MeV;
   G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();
   double dE_step     = aStep->GetTotalEnergyDeposit()/MeV;
   double dE_non_ion  = aStep->GetNonIonizingEnergyDeposit()/MeV;

   int pdgcode = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();

   if (aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary)  {
      // First step in volume
      // For some reason IsLastStepInVolume doesn't work for parallel geometry

   //if( aStep->IsLastStepInVolume() ) {
      //std::cout << "Made it from " << SensitiveDetectorName  << std::endl;

      fAnalysisManager->FillH2( fhXvsE_all, pos.x()/cm,energy);
      fAnalysisManager->FillH2( fhXY0_all, pos.x()/cm, pos.y()/cm);
      fAnalysisManager->FillH2( fhXY1_all, pos.x()/cm, pos.y()/cm);
      fAnalysisManager->FillH2( fhXY2_all, pos.x()/cm, pos.y()/cm);

      if( pdgcode == 22 ) {
         //photon
         fAnalysisManager->FillH2( fhXvsE_gamma, pos.x()/cm,energy);
      fAnalysisManager->FillH2( fhXY0_gamma, pos.x()/cm, pos.y()/cm);
      fAnalysisManager->FillH2( fhXY1_gamma, pos.x()/cm, pos.y()/cm);
      fAnalysisManager->FillH2( fhXY2_gamma, pos.x()/cm, pos.y()/cm);
      } else {
         fAnalysisManager->FillH2( fhXvsE_not_gamma, pos.x()/cm,energy);
      }

      if( pdgcode == 2112 ) {
         //neutron
         fAnalysisManager->FillH2( fhXvsE_n, pos.x()/cm,energy);
      }

      if( pz < 0.0 ) {
         fAnalysisManager->FillH1( fhBackward_0, energy);
      //   fAnalysisManager->FillH2( fhBackScatXYEnergyWt, pos.x()/cm, pos.y()/cm, energy);
      //   fAnalysisManager->FillH2( fhBackScatXYEnergyWt_1, pos.x()/cm, pos.y()/cm, energy);
      //   fAnalysisManager->FillH2( fhBackScat_XY, pos.x()/cm, pos.y()/cm);
      //   fAnalysisManager->FillH2( fhBackScat_XY_1, pos.x()/cm, pos.y()/cm);

      //   ////std::cout << energy << "\n";
      } else {
         fAnalysisManager->FillH1(fhForward_0, energy);
      //   //fRun->fhForwardScatEnergy->Fill( energy  );
      //   //fAnalysisManager->FillH2( fhForwardScatXYEnergyWt, pos.x()/cm, pos.y()/cm, energy);
      //   //fAnalysisManager->FillH2( fhForwardScatXYEnergyWt_1, pos.x()/cm, pos.y()/cm, energy);
      //   fAnalysisManager->FillH2( fhForwardScat_XY, pos.x()/cm, pos.y()/cm);
      //   fAnalysisManager->FillH2( fhForwardScat_XY_1, pos.x()/cm, pos.y()/cm);
      }
   }
   //if(dE_step > 0.0) {
   //   //std::cout << " dE_step    = " << dE_step    << std::endl;;
   //   //std::cout << " dE_non_ion = " << dE_non_ion << std::endl;;
   //   //std::cout << pos.x()/cm << " " << pos.z()/cm << std::endl;
   //   fAnalysisManager->FillH2( fhEDD_x_y0 , pos.x()/cm, pos.z()/cm, dE_step);
   //   fAnalysisManager->FillH2( fhEDD_y_x0 , pos.y()/cm, pos.z()/cm, dE_step);

   //   fAnalysisManager->FillH2( fhEDD_x_y0_1 , pos.x()/cm, pos.z()/cm, dE_step);
   //   fAnalysisManager->FillH2( fhEDD_y_x0_1 , pos.y()/cm, pos.z()/cm, dE_step);

   //   fAnalysisManager->FillH2( fhEDD_x_y0_2 , pos.x()/cm, pos.z()/cm, dE_step);
   //   fAnalysisManager->FillH2( fhEDD_y_x0_2 , pos.y()/cm, pos.z()/cm, dE_step);

   //   fAnalysisManager->FillH2( fhEDD_x_y0_3 , pos.x()/cm, pos.z()/cm, dE_step);
   //   fAnalysisManager->FillH2( fhEDD_y_x0_3 , pos.y()/cm, pos.z()/cm, dE_step);

   //   fAnalysisManager->FillH2( fhEDD_x_y0_4 , pos.x()/cm, pos.z()/cm, dE_step);
   //   fAnalysisManager->FillH2( fhEDD_y_x0_4 , pos.y()/cm, pos.z()/cm, dE_step);

   //   //fAnalysisManager->FillH2( fhEDDNonIonizing_x_y0 , pos.x()/nm, pos.z()/nm, dE_non_ion);
   //   //fAnalysisManager->FillH2( fhEDDNonIonizing_y_x0 , pos.y()/nm, pos.z()/nm, dE_non_ion);
   //}
   // Ensure counting incoming tracks only.
   //if ( preStep->GetStepStatus() == fGeomBoundary ){
   //   FakeSDHit* newHit = new FakeSDHit();
   //   newHit->SetStripNo(  touchable->GetReplicaNumber(0) );
   //   newHit->SetPosition( aStep->GetPreStepPoint()->GetPosition() );
   //   newHit->SetMomentum( aStep->GetPreStepPoint()->GetMomentum() );
   //   newHit->SetEnergy( aStep->GetPreStepPoint()->GetTotalEnergy() );
   //   newHit->SetParticle( aStep->GetTrack()->GetDefinition() );
   //   hitsCollection->insert( newHit );
   //}
   return true;
}
//______________________________________________________________________________

void FakeSD::EndOfEvent(G4HCofThisEvent*)
{ }
//______________________________________________________________________________

void FakeSD::clear()
{ } 
//______________________________________________________________________________

void FakeSD::DrawAll()
{ } 
//______________________________________________________________________________

void FakeSD::PrintAll()
{ } 
//______________________________________________________________________________

