#include "SimplePrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
//#include "SimulationManager.hh"
//#include "TParticle.h"


SimplePrimaryGeneratorAction::SimplePrimaryGeneratorAction(int pdgcode) : 
   G4VUserPrimaryGeneratorAction()
{
   G4int n_particle = 1;
   fParticleGun  = new G4GeneralParticleSource();

   // default particle kinematic
   particleTable = G4ParticleTable::GetParticleTable();
   G4String  particleName;

   //G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e-");
   G4ParticleDefinition* particle = particleTable->FindParticle(pdgcode);
   fParticleGun->SetParticleDefinition(particle);
   //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
   //fParticleGun->SetParticleEnergy(11.0*GeV); // kinetic energy (not total)

   std::cout << " SIMPLE PRIMARY EVENT GENERATOR ACTION!!!!!!!!!! \n";

   G4UImanager* UImanager = G4UImanager::GetUIpointer();
   UImanager->ApplyCommand("/gps/pos/type Volume");
   UImanager->ApplyCommand("/gps/pos/shape Cylinder");
   UImanager->ApplyCommand("/gps/pos/centre 0 0 0 cm");
   UImanager->ApplyCommand("/gps/pos/halfz 2.5 cm ");
   UImanager->ApplyCommand("/gps/pos/radius 0.1 mm ");

   UImanager->ApplyCommand("/gps/ang/type iso");
   UImanager->ApplyCommand("/gps/ang/mintheta 0   deg");
   UImanager->ApplyCommand("/gps/ang/maxtheta 100 deg");
   UImanager->ApplyCommand("/gps/ang/minphi  -180 deg");
   UImanager->ApplyCommand("/gps/ang/maxphi   180 deg");

   //UImanager->ApplyCommand("/gps/ene/type   Exp");
   //UImanager->ApplyCommand("/gps/ene/ezero 100 GeV");
   UImanager->ApplyCommand("/gps/ene/type   Lin");
   UImanager->ApplyCommand("/gps/ene/intercept 1.0");
   UImanager->ApplyCommand("/gps/ene/gradient  0.0");

   UImanager->ApplyCommand("/gps/ene/min  1 MeV");
   UImanager->ApplyCommand("/gps/ene/max  1 GeV");

   fThrownEvent = { { 11, {0.0,0.0,0.0}, {0,0,0,0}} };

}
//______________________________________________________________________________

SimplePrimaryGeneratorAction::~SimplePrimaryGeneratorAction()
{
   delete fParticleGun;
}
//______________________________________________________________________________

void SimplePrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
   //fdP     = fP_max-fP_min;
   //fdTheta = fTheta_max-fTheta_min;
   //fdPhi   = fPhi_max-fPhi_min;

  int npart = fThrownEvent.size();
  for(auto& part : fThrownEvent) {

    //auto root_PDGparticle = part[
    auto g4_particle      = fParticleGun->GetParticleDefinition();

    fParticleGun->GeneratePrimaryVertex(anEvent);

    //int pdgcode = g4_particle->GetPDGEncoding();
    //double mass          = g4_particle->GetPDGMass()/GeV;
    ////std::cout << " Mass = " << mass << std::endl;
    ////double mass          = part->GetMass();
    //double E_kin         = fParticleGun->GetParticleEnergy()/GeV;
    //double E_tot         = (E_kin+mass);
    //double p_rand        = TMath::Sqrt( E_tot*E_tot - mass*mass );
    //G4ThreeVector pvec   = fParticleGun->GetParticleMomentumDirection();
    //pvec.setMag(p_rand); 
    //G4ThreeVector pos   = fParticleGun->GetParticlePosition();

    //TLorentzVector P4vec = {pvec.x(),pvec.y(),pvec.z(),E_tot}; 
    //part->SetPdgCode(pdgcode);
    //part->SetCalcMass(mass);
    //part->SetMomentum(P4vec);
    //part->SetProductionVertex(TLorentzVector(pos.x()/cm,pos.y()/cm,pos.z()/cm, 0.0));

  } 

}
//______________________________________________________________________________

