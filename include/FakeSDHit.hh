#ifndef FakeSDHit_h
#define FakeSDHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"

class FakeSDHit : public G4VHit
{
  public:

      FakeSDHit();
      ~FakeSDHit();
      G4int operator==(const FakeSDHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

      void Draw();
      void Print();

  private:
      G4int         stripNo;
      G4ThreeVector position;
      G4ThreeVector momentum;
      G4double      energy;
      G4ParticleDefinition* particle;

  public:
      inline void SetStripNo(G4int strip)
      { stripNo=strip; }
      inline G4int GetStripNo()
      { return stripNo; }
      inline void SetPosition(G4ThreeVector pos)
      { position=pos; }
      inline G4ThreeVector GetPosition()
      { return position; }
      inline void SetMomentum(G4ThreeVector mom)
      { momentum = mom; }
      inline G4ThreeVector GetMomentum()
      { return momentum; }
      inline void SetEnergy(G4double ene)
      { energy = ene; }
      inline G4double GetEnergy()
      { return energy; }
      inline void SetParticle(G4ParticleDefinition* pdef)
      { particle = pdef; }
      inline G4ParticleDefinition* GetParticle()
      { return particle; }

};

typedef G4THitsCollection<FakeSDHit> FakeSDHitsCollection;

extern G4Allocator<FakeSDHit> FakeSDHitAllocator;

inline void* FakeSDHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) FakeSDHitAllocator.MallocSingle();
  return aHit;
}

inline void FakeSDHit::operator delete(void *aHit)
{
  FakeSDHitAllocator.FreeSingle((FakeSDHit*) aHit);
}

#endif

