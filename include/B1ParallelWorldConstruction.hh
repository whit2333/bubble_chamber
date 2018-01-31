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
#include "globals.hh"
#include "G4SystemOfUnits.hh"


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
      double        fSpanDistance  = 35*2.54*cm;//64.0*cm;
      G4ThreeVector fDirection    {0,0,1.0};
      G4ThreeVector fStartingPoint{0,0,3.25*mm} ;
      double        fDet_size      = 10.0*cm;

      static const int fNplanes = 6;
      std::array< G4ThreeVector      , fNplanes > fDet_pos   ;
      std::array< G4VSolid          *, fNplanes > fDet_solid ;
      std::array< G4LogicalVolume   *, fNplanes > fDet_log   ;
      std::array< G4VPhysicalVolume *, fNplanes > fDet_phys  ;
      std::array< FakeSD            *, fNplanes > fDet_det  ;
      std::array< G4VisAttributes   *, fNplanes > fDet_vis  ;
      //std::array< G4ThreeVector      , fNplanes > fDet_pos = {
      //  {0,0,3.25*mm},
      //  {0,0,3.25*mm + 1.0*mm + 6.0*2.54*cm },
      //  {0,0,3.25*mm + 1.0*mm + 6.0*2.54*cm + 15.24*cm + 5.17*mm}
      //};


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

