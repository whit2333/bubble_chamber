#ifndef B1PrimaryGeneratorAction_h
#define B1PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

#include "CLHEP/Random/RandGauss.h"

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


  void   SetBeamEnergy(double E) { fBeamEnergy = E; }
  double GetBeamEnergy() const { return fBeamEnergy; }

  G4ThreeVector  fVertex = G4ThreeVector(0,0,0);

  double  fVertex_x_sig  = 1.0*CLHEP::mm;
  double  fVertex_y_sig  = 1.0*CLHEP::mm;

  double  fVertex_x_mean = 0.0;
  double  fVertex_y_mean = 0.0;

  double  fEnergy_sig  = 3.0*CLHEP::keV;

  void InitRand();


private:
  double          fBeamEnergy;
  G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
  G4Box*          fEnvelopeBox;

  CLHEP::RandGauss* fRandVertex_x = nullptr;
  CLHEP::RandGauss* fRandVertex_y = nullptr;

  CLHEP::RandGauss* fRandEnergy   = nullptr;

};


#endif


