#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1DetectorConstruction.hh"
#include "B1Run.hh"
#include "B1Analysis.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include <string>
#include <sstream>

using ss = std::stringstream;

B1RunAction::B1RunAction(G4int rn) : G4UserRunAction(),
   fRunNumber(rn)
{ 
   //fOutputFile               = 0;
   // add new units for dose
   const G4double milligray = 1.e-3*gray;
   const G4double microgray = 1.e-6*gray;
   const G4double nanogray  = 1.e-9*gray;  
   const G4double picogray  = 1.e-12*gray;

   new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
   new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
   new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
   new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);        

   // Create analysis manager
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   analysisManager->SetVerboseLevel(1);
   analysisManager->SetFirstHistoId(1);


}
//______________________________________________________________________________

B1RunAction::~B1RunAction()
{ }
//______________________________________________________________________________

G4Run* B1RunAction::GenerateRun()
{
   //std::cout << " start B1RunAction::GenerateRun\n";
   G4SDManager::GetSDMpointer()->ListTree();


   //fOutputFile = new TFile(Form("EBL_sim_output_%d.root",fRunNumber), "UPDATE");
   //fOutputFile->cd();


   B1Run * run = new B1Run(fRunNumber);

   return run; 
}
//______________________________________________________________________________

void B1RunAction::BeginOfRunAction(const G4Run*)
{ 
   //inform the runManager to save random number seed
   G4RunManager::GetRunManager()->SetRandomNumberStore(false);

   // Get analysis manager
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

   // Open an output file
   ss file_name;
   file_name << "EBL_sim_output_" << fRunNumber;
   analysisManager->OpenFile(file_name.str().c_str());
}
//______________________________________________________________________________

void B1RunAction::EndOfRunAction(const G4Run* run)
{
   G4int nofEvents = run->GetNumberOfEvent();
   if (nofEvents == 0) return;

   const B1Run* b1Run = static_cast<const B1Run*>(run);

   // Compute dose
   //
   G4double edep  = b1Run->GetEdep();
   G4double edep2 = b1Run->GetEdep2();
   G4double rms = edep2 - edep*edep/nofEvents;
   if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

   const B1DetectorConstruction* detectorConstruction
      = static_cast<const B1DetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
   G4double mass = detectorConstruction->GetScoringVolume()->GetMass();
   G4double dose = edep/mass;
   G4double rmsDose = rms/mass;

   // Run conditions
   //  note: There is no primary generator action object for "master"
   //        run manager for multi-threaded mode.
   const B1PrimaryGeneratorAction* generatorAction
      = static_cast<const B1PrimaryGeneratorAction*>
      (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
   G4String runCondition;
   if (generatorAction)
   {
      const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
      runCondition += particleGun->GetParticleDefinition()->GetParticleName();
      runCondition += " of ";
      G4double particleEnergy = particleGun->GetParticleEnergy();
      runCondition += G4BestUnit(particleEnergy,"Energy");
   }

   // Print
   //  
   if (IsMaster()) {
      G4cout
         << G4endl
         << "--------------------End of Global Run-----------------------";
      //fOutputFile->Write();
      //fOutputFile->Close();
      // Save histograms
      G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
      analysisManager->Write();
      analysisManager->CloseFile();
   }
   else {
      G4cout
         << G4endl
         << "--------------------End of Local Run------------------------";
   }

   G4cout
      << G4endl
      << " The run consists of " << nofEvents << " "<< runCondition
      << G4endl
      << " Dose in scoring volume : " 
      << G4BestUnit(dose,"Dose") << " +- " << G4BestUnit(rmsDose,"Dose")
      << G4endl
      << "------------------------------------------------------------"
      << G4endl
      << G4endl;


}
//______________________________________________________________________________

