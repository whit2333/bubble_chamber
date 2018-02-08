#ifndef B1PrimaryGeneratorAction_h
#define B1PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

/// The primary generator action class with particle gun.

class B1PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    B1PrimaryGeneratorAction();    
    virtual ~B1PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event*);         
  
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

    G4ThreeVector  fVertex = G4ThreeVector(0,0,0);

    void   SetBeamEnergy(double E) { fBeamEnergy = E; }
    double GetBeamEnergy() const { return fBeamEnergy; }
  
  
  private:
    double          fBeamEnergy;
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    G4Box*          fEnvelopeBox;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


