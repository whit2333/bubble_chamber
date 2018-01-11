#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

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

      G4double world_x                        = 1.0*m        ;
      G4double world_y                        = 1.0*m        ;
      G4double world_z                        = 2.0*m        ;
      G4double radiator_thickness             = 6.0*mm       ;
      G4double radiator_collimator_gap        = 1.0*mm       ; // ?
      G4double collimator_OD                  = 4.0*2.54*cm  ;
      G4double collimator_ID                  = 0.315*2.54*cm;
      G4double collimator_z_end               = 0.0*cm       ;
      G4double collimator_length              = 6.0*2.54*cm  ;
      G4double collimator_target_center_gap   = 4.0*cm       ;
      G4double collimator_chamber_port_gap    = 2.0*mm       ; // ?
      G4double chamber_port_length            = 15.24*cm ;
      G4double chamber_port_flange_length     =  7.62*cm ;
      G4double chamber_flange_port_offset     =  3.17*mm ;
      G4double collimator_insert_length       = 10.16*cm ;

      G4double beampipe_length              = 20.0*cm ;
      G4double beampipe_diameter            = 8.0*cm  ;
      G4double radiator_diameter            = 8.0*cm  ;
      G4double scoring_diameter             = 1.5*2.54*cm ;
      G4double scoring_length               = 0.01*mm ;
      G4double window_diameter              = 1.0*cm  ;
      G4double window_thickness             = 8.0*mm  ;
      G4double scoring2_diameter            = 1.5*2.54*cm ;
      G4double scoring2_length              = 0.01*mm ;

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

      G4ThreeVector       port_collimator_pos;
      G4ThreeVector       port_collimator_offset;
      G4Material        * port_collimator_mat ;  
      G4VSolid          * port_collimator_solid ;
      G4LogicalVolume   * port_collimator_log ;  
      G4VPhysicalVolume * port_collimator_phys;  

      G4ThreeVector       chamber_port_flange_pos;
      G4ThreeVector       chamber_port_flange_offset; // for mesh importing
      G4Material        * chamber_port_flange_mat ;  
      G4VSolid          * chamber_port_flange_solid ;
      G4LogicalVolume   * chamber_port_flange_log ;  
      G4VPhysicalVolume * chamber_port_flange_phys;  

      G4ThreeVector       chamber_port_pos;
      G4ThreeVector       chamber_port_offset; // for mesh importing
      G4Material        * chamber_port_mat ;  
      G4VSolid          * chamber_port_solid ;
      G4LogicalVolume   * chamber_port_log ;  
      G4VPhysicalVolume * chamber_port_phys;  

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
      void     SetCollimatorOD(G4double l) ;
      void     SetCollimatorID(G4double l) ;

      void     PrintConfigInfo() const;

      void CalculatePositions();

      void Rebuild();

      G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

   protected:
};
//______________________________________________________________________________

#endif

