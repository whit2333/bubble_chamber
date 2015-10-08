#include "BeamTestHit.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"


G4Allocator<BeamTestHit> BeamTestHitAllocator;

BeamTestHit::BeamTestHit()
{;}

BeamTestHit::~BeamTestHit()
{;}

G4int BeamTestHit::operator==(const BeamTestHit &right) const
{
  return (this==&right) ? 1 : 0;
}

void BeamTestHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(position);
    circle.SetScreenSize(10.04);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(0.,0.,1.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

void BeamTestHit::Print()
{;}


