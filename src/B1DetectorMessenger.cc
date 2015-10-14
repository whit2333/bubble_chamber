#include "B1DetectorMessenger.hh"
#include "B1DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//______________________________________________________________________________

B1DetectorMessenger::B1DetectorMessenger(B1DetectorConstruction* Det) :
   G4UImessenger(), fDetectorConstruction(Det)
{
  fB1Directory = new G4UIdirectory("/B1/");
  fB1Directory->SetGuidance("UI commands specific to this example.");

  fDetDirectory = new G4UIdirectory("/B1/det/");
  fDetDirectory->SetGuidance("Detector construction control");

  fPrintConfigInfoCmd = new G4UIcmdWithoutParameter("/B1/det/printConfigInfo",this);
  fPrintConfigInfoCmd->SetGuidance("prints details of interest.");
  fPrintConfigInfoCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fRadiatorMatCmd = new G4UIcmdWithAString("/B1/det/setRadiatorMaterial",this);
  fRadiatorMatCmd->SetGuidance("Select Material of the Radiator.");
  fRadiatorMatCmd->SetParameterName("choice",false);
  fRadiatorMatCmd->SetCandidates("G4_Cu G4_Au G4_Ag G4_Fe");
  fRadiatorMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fCollimatorMatCmd = new G4UIcmdWithAString("/B1/det/setCollimatorMaterial",this);
  fCollimatorMatCmd->SetGuidance("Select Material of the Collimator.");
  fCollimatorMatCmd->SetParameterName("choice",false);
  fCollimatorMatCmd->SetCandidates("G4_Cu G4_Au G4_Ag G4_Fe");
  fCollimatorMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fCollimatorLengthCmd = new G4UIcmdWithADoubleAndUnit("/B1/det/setCollimatorLength",this);
  fCollimatorLengthCmd->SetGuidance("Define a the length of the collimator");
  fCollimatorLengthCmd->SetParameterName("length",false);
  fCollimatorLengthCmd->SetUnitCategory("Length");
  fCollimatorLengthCmd->AvailableForStates(G4State_Idle);

  fInnerCollimatorODCmd = new G4UIcmdWithADoubleAndUnit("/B1/det/setInnerCollimatorOD",this);
  fInnerCollimatorODCmd->SetGuidance("Define a the OD of the inner collimator");
  fInnerCollimatorODCmd->SetParameterName("length",false);
  fInnerCollimatorODCmd->SetUnitCategory("Length");
  fInnerCollimatorODCmd->AvailableForStates(G4State_Idle);

  fRadiatorCollimatorGapCmd = new G4UIcmdWithADoubleAndUnit("/B1/det/setRadiatorCollimatorGap",this);
  fRadiatorCollimatorGapCmd->SetGuidance("Define a the space between the end of the radiator and the face of the collimator.");
  fRadiatorCollimatorGapCmd->SetParameterName("length",false);
  fRadiatorCollimatorGapCmd->SetUnitCategory("Length");
  fRadiatorCollimatorGapCmd->AvailableForStates(G4State_Idle);

  fTargMatCmd = new G4UIcmdWithAString("/B1/det/setTargetMaterial",this);
  fTargMatCmd->SetGuidance("Select Material of the Target.");
  fTargMatCmd->SetParameterName("choice",false);
  fTargMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fChamMatCmd = new G4UIcmdWithAString("/B1/det/setChamberMaterial",this);
  fChamMatCmd->SetGuidance("Select Material of the Chamber.");
  fChamMatCmd->SetParameterName("choice",false);
  fChamMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/B1/det/stepMax",this);
  fStepMaxCmd->SetGuidance("Define a step max");
  fStepMaxCmd->SetParameterName("stepMax",false);
  fStepMaxCmd->SetUnitCategory("Length");
  fStepMaxCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorMessenger::~B1DetectorMessenger()
{
  delete fRadiatorMatCmd;
  delete fCollimatorLengthCmd;
  delete fRadiatorCollimatorGapCmd;
  delete fTargMatCmd;
  delete fChamMatCmd;
  delete fStepMaxCmd;
  delete fB1Directory;
  delete fDetDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fCollimatorMatCmd )
   { fDetectorConstruction->SetCollimatorMaterial(newValue);}

  if( command == fCollimatorLengthCmd )
   { fDetectorConstruction->SetCollimatorLength( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue));}

  if( command == fInnerCollimatorODCmd )
   { fDetectorConstruction->SetInnerCollimatorOD( G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue));}

  if( command == fPrintConfigInfoCmd )
   { fDetectorConstruction->PrintConfigInfo();}

  //if( command == fChamMatCmd )
  // { fDetectorConstruction->SetChamberMaterial(newValue);}

  //if( command == fStepMaxCmd ) {
  //  fDetectorConstruction ->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
  //}   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
