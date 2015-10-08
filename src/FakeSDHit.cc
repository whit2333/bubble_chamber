#include "FakeSDHit.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"


G4Allocator<FakeSDHit> FakeSDHitAllocator;

FakeSDHit::FakeSDHit()
{;}

FakeSDHit::~FakeSDHit()
{;}

G4int FakeSDHit::operator==(const FakeSDHit &right) const
{
  return (this==&right) ? 1 : 0;
}

void FakeSDHit::Draw()
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

void FakeSDHit::Print()
{
   std::cout << "Derp\n";
}


