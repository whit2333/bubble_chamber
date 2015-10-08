#include "EBLsim.h"
#include "B1DetectorConstruction.hh"
#include "B1ActionInitialization.hh"
#include "G4SystemOfUnits.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "QBBC.hh"
#include "G4UIQt.hh"
#include <qmainwindow.h>

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4PhysListFactory.hh"
#include "G4StepLimiterPhysics.hh"
#include "Randomize.hh"
//______________________________________________________________________________

int main(int argc,char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4long seed = time(NULL);
  CLHEP::HepRandom::setTheSeed(seed);
  
  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new B1DetectorConstruction());

  // Physics list
  G4PhysListFactory     factory;
  G4VModularPhysicsList * physicsList =  factory.GetReferencePhysList("QGSP_BERT_HP_PEN");
  physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  //physicsList->ReplacePhysics(new G4IonQMDPhysics());
  physicsList->SetDefaultCutValue(0.005*um);
  //QGSP_BERT_HP_PEN

  //G4VModularPhysicsList* physicsList = new QBBC;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);
    
  // User action initialization
  runManager->SetUserInitialization(new B1ActionInitialization());
  
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  G4UIQt* qui = static_cast<G4UIQt*> (UImanager->GetG4UIWindow());
  if (qui) {
       qui->GetMainWindow()->setVisible(true);
  }


  // Process macro or start UI session
  //
  if ( ! ui ) { 
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else { 
     // interactive mode
     G4String command   = "/control/macroPath ";
     G4String mac_dir   = EBLSIM_MACRO_DIR;
     G4String fileName = "init_default.mac";

     std::cout << " executing " << command+mac_dir << std::endl;
     UImanager->ApplyCommand(command+mac_dir);

     command = "/control/execute ";

     std::cout << " executing " << command+fileName << std::endl;
     UImanager->ApplyCommand(command+fileName);

     ui->SessionStart();
     delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
