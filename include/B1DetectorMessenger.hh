#ifndef B1DetectorMessenger_h
#define B1DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class B1DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


/// Messenger class that defines commands for B1DetectorConstruction.
///
/// It implements commands:
/// - /B1/det/setTargetMaterial name
/// - /B1/det/setChamberMaterial name
/// - /B1/det/stepMax value unit

class B1DetectorMessenger: public G4UImessenger
{
  public:
    B1DetectorMessenger(B1DetectorConstruction* );
    virtual ~B1DetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    B1DetectorConstruction*  fDetectorConstruction;

    G4UIdirectory*           fB1Directory;
    G4UIdirectory*           fDetDirectory;

    G4UIcmdWithoutParameter    * fPrintConfigInfoCmd;
    G4UIcmdWithAString        * fRadiatorMatCmd;
    G4UIcmdWithAString        * fCollimatorMatCmd;
    G4UIcmdWithADoubleAndUnit * fCollimatorLengthCmd;
    G4UIcmdWithADoubleAndUnit * fInnerCollimatorODCmd;
    G4UIcmdWithADoubleAndUnit * fRadiatorCollimatorGapCmd;
    G4UIcmdWithAString        * fTargMatCmd;
    G4UIcmdWithAString        * fChamMatCmd;

    G4UIcmdWithADoubleAndUnit* fStepMaxCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
