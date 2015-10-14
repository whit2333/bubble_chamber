#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class B1DetectorMessenger;
class G4Material;
class G4VSolid;
class FakeSD;
#include "G4ThreeVector.hh"
#include "G4String.hh"

/// Detector construction class to define materials and geometry.

class B1DetectorConstruction : public G4VUserDetectorConstruction
{
   private:

      G4double world_x                       ;
      G4double world_y                       ;
      G4double world_z                       ;
      G4double radiator_thickness            ;
      G4double collimator_target_center_gap  ;
      G4double outer_collimator_ID           ;
      G4double outer_collimator_OD           ;
      G4double collimator_ID                 ;
      G4double collimator_OD                 ;
      G4double collimator_diameter           ;
      G4double collimator_z_end              ;
      G4double radiator_collimator_gap ;
      G4double collimator_length   ;
      G4double beampipe_length   ;
      G4double beampipe_diameter ;
      G4double radiator_diameter ;
      G4double scoring_diameter  ;
      G4double scoring_length    ;
      G4double window_diameter   ;
      G4double window_thickness  ;
      G4double scoring2_diameter ;
      G4double scoring2_length   ;

   protected:
      G4LogicalVolume     * fScoringVolume;
      B1DetectorMessenger * fMessenger;
      G4String    fCollimatorMatName;

      bool fHasBeenBuilt;


   private:

      FakeSD * scoring_det;
      FakeSD * scoring2_det;

      G4Material        * world_mat   ;
      G4VSolid          * world_solid ;
      G4LogicalVolume   * world_log   ;
      G4VPhysicalVolume * world_phys  ;

      G4ThreeVector       beampipe_pos;
      G4Material        * beampipe_mat   ;
      G4VSolid          * beampipe_solid ;
      G4LogicalVolume   * beampipe_log   ;
      G4VPhysicalVolume * beampipe_phys  ;
      G4ThreeVector       radiator_pos;
      G4Material        * radiator_mat  ;
      G4VSolid          * radiator_solid;
      G4LogicalVolume   * radiator_log  ;
      G4VPhysicalVolume * radiator_phys ;
      G4ThreeVector       collimator_pos;
      G4Material        * collimator_mat ;  
      G4VSolid          * collimator_solid ;
      G4LogicalVolume   * collimator_log ;  
      G4VPhysicalVolume * collimator_phys;  
      G4ThreeVector       collimator2_pos;
      G4Material        * collimator2_mat ;  
      G4VSolid          * collimator2_solid ;
      G4LogicalVolume   * collimator2_log ;  
      G4VPhysicalVolume * collimator2_phys;  
      G4ThreeVector       outer_collimator_pos;
      G4Material        * outer_collimator_mat ;  
      G4VSolid          * outer_collimator_solid ;
      G4LogicalVolume   * outer_collimator_log ;  
      G4VPhysicalVolume * outer_collimator_phys;  
      G4ThreeVector       scoring_pos;
      G4Material        * scoring_mat   ;
      G4VSolid          * scoring_solid ;
      G4LogicalVolume   * scoring_log   ;
      G4VPhysicalVolume * scoring_phys  ;
      G4ThreeVector       window_pos;
      G4Material        * window_mat  ;
      G4VSolid          * window_solid;
      G4LogicalVolume   * window_log  ;
      G4VPhysicalVolume * window_phys ;
      G4ThreeVector       scoring2_pos;
      G4Material        * scoring2_mat  ;
      G4VSolid          * scoring2_solid;
      G4LogicalVolume   * scoring2_log  ;
      G4VPhysicalVolume * scoring2_phys ;

   public:
      B1DetectorConstruction();
      virtual ~B1DetectorConstruction();

      virtual G4VPhysicalVolume* Construct();

      void SetRadiatorMaterial(G4String);
      void SetCollimatorMaterial(G4String);

      G4double GetRadiatorCollimatorGap() const {return radiator_collimator_gap;}
      G4double GetCollimatorLength() const {return collimator_length;}

      void     SetRadiatorCollimatorGap(G4double l) ;
      void     SetCollimatorLength(G4double l) ;
      void     SetInnerCollimatorOD(G4double l) ;

      void     PrintConfigInfo() const;

      void CalculatePositions();

      void Rebuild();

      G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

   protected:
};
//______________________________________________________________________________

#endif

