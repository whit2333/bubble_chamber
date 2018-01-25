#ifndef SimplePrimaryGeneratorAction_h
#define SimplePrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"
#include <fstream>
//#include "ThrownEvent.h"

#include "Math/Vector3D.h"
#include "Math/Vector4D.h"

struct PrimaryParticle {
  int                        pdg = 11;
  ROOT::Math::XYZVector      vertex;
  ROOT::Math::PxPyPzMVector  momentum;
};

class G4GeneralParticleSource;
class G4Event;
class G4Box;
class G4ParticleTable;

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued 
/// in front of the phantom across 80% of the (X,Y) phantom size.

class SimplePrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  private:
    G4GeneralParticleSource*  fParticleGun; // pointer a to G4 gun class

    //double        fP_min     = 0.001*CLHEP::MeV;
    //double        fP_max     = 1.000*CLHEP::GeV;
    //double        fTheta_min = 0.0;
    //double        fTheta_max = 100.0*CLHEP::degree;
    //double        fPhi_min   =-180.0*CLHEP::degree;
    //double        fPhi_max   = 180.0*CLHEP::degree;

    //double        fdP        = 1.0;
    //double        fdTheta    = 1.0;
    //double        fdPhi      = 1.0;

    //double        fDelta_z   = 5.0*CLHEP::cm;

    G4ParticleTable* particleTable;

  public:

    std::vector<PrimaryParticle>  fThrownEvent;

    SimplePrimaryGeneratorAction(int pdgcode = 11);    
    virtual ~SimplePrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4GeneralParticleSource* GetParticleGun() const { return fParticleGun; }
};

#endif


