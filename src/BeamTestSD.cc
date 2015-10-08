//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//

#include "BeamTestSD.hh"
#include "BeamTestHit.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"

BeamTestSD::BeamTestSD(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="hitsCollection");

  HCID = -1;
}

BeamTestSD::~BeamTestSD(){;}

void BeamTestSD::Initialize(G4HCofThisEvent* HCE)
{

  hitsCollection = new BeamTestHitsCollection
                      (SensitiveDetectorName,collectionName[0]); 
  if(HCID<0)
  { HCID = GetCollectionID(0); }
  HCE->AddHitsCollection(HCID,hitsCollection);
}

G4bool BeamTestSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{

  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4TouchableHistory* touchable = (G4TouchableHistory*)(preStep->GetTouchable());

  // Ensure counting incoming tracks only.
  if ( preStep->GetStepStatus() == fGeomBoundary ){
    BeamTestHit* newHit = new BeamTestHit();
    newHit->SetStripNo(  touchable->GetReplicaNumber(0) );
    newHit->SetPosition( aStep->GetPreStepPoint()->GetPosition() );
    newHit->SetMomentum( aStep->GetPreStepPoint()->GetMomentum() );
    newHit->SetEnergy( aStep->GetPreStepPoint()->GetTotalEnergy() );
    newHit->SetParticle( aStep->GetTrack()->GetDefinition() );
    hitsCollection->insert( newHit );
  }
  return true;
}

void BeamTestSD::EndOfEvent(G4HCofThisEvent*)
{;}

void BeamTestSD::clear()
{;} 

void BeamTestSD::DrawAll()
{;} 

void BeamTestSD::PrintAll()
{;} 
