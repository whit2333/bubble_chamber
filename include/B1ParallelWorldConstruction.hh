#ifndef B1ParallelWorldConstruction_h
#define B1ParallelWorldConstruction_h 1

#include "globals.hh"
#include <array>

class G4VPhysicalVolume;
class G4LogicalVolume;
class B1DetectorMessenger;
class G4Material;
class G4VSolid;
class FakeSD;
class G4VisAttributes;
#include "G4ThreeVector.hh"
#include "G4String.hh"
#include "G4VUserParallelWorld.hh"


/// A parallel world construction class
///
/// - void Construct()
///     creates a parallel world in the mass world and parameterized volumes
//
class B1ParallelWorldConstruction : public G4VUserParallelWorld
{
   private:
      G4bool        fConstructed;
      G4bool        fNeedsRebuilt;
      double        fSpanDistance;
      G4ThreeVector fDirection;
      G4ThreeVector fStartingPoint;
      double        fDet_size;

      static const int fNplanes = 10;
      std::array< G4ThreeVector      , fNplanes > fDet_pos   ;
      std::array< G4VSolid          *, fNplanes > fDet_solid ;
      std::array< G4LogicalVolume   *, fNplanes > fDet_log   ;
      std::array< G4VPhysicalVolume *, fNplanes > fDet_phys  ;
      std::array< FakeSD            *, fNplanes > fDet_det  ;
      std::array< G4VisAttributes   *, fNplanes > fDet_vis  ;

   public:
      B1ParallelWorldConstruction(G4String& parallelWorldName);
      virtual ~B1ParallelWorldConstruction();

      void SetSpanDistance(double L) {
         fNeedsRebuilt = true;
         fSpanDistance = L;
      }

   public:
      virtual void Construct();


};

#endif

