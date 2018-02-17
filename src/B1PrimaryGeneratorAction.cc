#include "B1PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"



B1PrimaryGeneratorAction::B1PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0), fEnvelopeBox(0),fBeamEnergy(8.0*MeV)
{

  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e-");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(fBeamEnergy); // kinetic energy (not total)

  if (!fRandVertex_x) fRandVertex_x = new CLHEP::RandGauss(CLHEP::HepRandom::getTheEngine(),fVertex_x_mean, fVertex_x_sig);
  if (!fRandVertex_y) fRandVertex_y = new CLHEP::RandGauss(CLHEP::HepRandom::getTheEngine(),fVertex_y_mean, fVertex_y_sig);
  if (!fRandEnergy)   fRandEnergy   = new CLHEP::RandGauss(CLHEP::HepRandom::getTheEngine(),fBeamEnergy,fEnergy_sig);
}
//______________________________________________________________________________

B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
  delete fParticleGun;
}
//______________________________________________________________________________

void B1PrimaryGeneratorAction::InitRand()
{
  if (!fRandVertex_x) delete fRandVertex_x;
  fRandVertex_x= new CLHEP::RandGauss(CLHEP::HepRandom::getTheEngine(),0.0, fVertex_x_sig);
  if (!fRandVertex_y) delete fRandVertex_y;
  fRandVertex_y = new CLHEP::RandGauss(CLHEP::HepRandom::getTheEngine(),0.0, fVertex_y_sig);
  if (!fRandEnergy)   delete fRandEnergy;
  fRandEnergy= new CLHEP::RandGauss(CLHEP::HepRandom::getTheEngine(),fBeamEnergy,fEnergy_sig);
}
//______________________________________________________________________________

void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  G4double envSizeXY = 0.001*mm;
  G4double envSizeZ  = 0.0*mm;

  fParticleGun->SetParticleEnergy(fBeamEnergy); // kinetic energy (not total)

  G4double size = 1.0; 
  G4double x0 = fRandVertex_x->fire();
  G4double y0 = fRandVertex_y->fire();
  G4double z0 = -10.0*cm;
  auto primary_postion  = G4ThreeVector(x0,y0,z0) + fVertex;

  G4double E0 = fRandEnergy->fire();

  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(E0); // kinetic energy (not total)
  fParticleGun->SetParticlePosition(primary_postion);
  fParticleGun->GeneratePrimaryVertex(anEvent);

  //anEvent->Print();
  //anEvent->GetPrimaryVertex()->Print();
}
//______________________________________________________________________________

