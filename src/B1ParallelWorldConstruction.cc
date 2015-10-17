#include "B1ParallelWorldConstruction.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"    
#include "G4SDManager.hh"    
#include "G4Colour.hh"    
#include "G4VisAttributes.hh"    
#include <string>
#include "FakeSD.hh"    

B1ParallelWorldConstruction ::B1ParallelWorldConstruction(G4String& parallelWorldName) :
   G4VUserParallelWorld(parallelWorldName),
   fConstructed(false),
   fNeedsRebuilt(true),
   fSpanDistance(40.0*cm),
   fDirection(0,0,1.0),
   fStartingPoint(0,0,0),
   fDet_size(50.0*cm)
{
   for(int i = 0; i < fNplanes; i++) {
      fDet_solid[i]  = nullptr;
      fDet_log[i]    = nullptr;
      fDet_phys[i]   = nullptr;
      fDet_det[i]    = nullptr;
      fDet_vis[i]    = nullptr;
   }
   std::cout << "Parallel world ctor" <<std::endl;
}
//______________________________________________________________________________

B1ParallelWorldConstruction::~B1ParallelWorldConstruction()
{ }
//______________________________________________________________________________

void B1ParallelWorldConstruction::Construct()
{
   if( !fNeedsRebuilt && fConstructed) return;
   fConstructed = true;

   std::cout << "constriing Parallel world " <<std::endl;

   bool    checkOverlaps    = false;
   int     natoms           = 0;
   int     ncomponents      = 0;
   double  A                = 0.0;
   double  Z                = 0.0;
   double  thickness        = 0.0;
   double  surface_z        = 0.0;
   double  red              = 0.0;
   double  green            = 0.0;
   double  blue             = 0.0;
   double  alpha            = 0.0;
   double  density          = 0.0;
   double  pressure         = 0.0;
   double  temperature      = 0.0;

   G4VSolid          * scoring_solid  = nullptr;
   G4LogicalVolume   * scoring_log    = nullptr;
   G4VPhysicalVolume * scoring_phys   = nullptr;
   FakeSD            * scoring_det    = nullptr;
   G4VisAttributes   * scoring_vis    = nullptr;


   // --------------------------------------------------------------
   // World
   //
   G4VPhysicalVolume * ghostWorld   = GetWorld();
   G4LogicalVolume   * worldLogical = ghostWorld->GetLogicalVolume();

   // --------------------------------------------------------------
   // 

   double        scoring_length = 1.0*um;
   double        step_size   = fSpanDistance/double(fNplanes-1);
   G4ThreeVector scoring_pos = fStartingPoint;
   G4ThreeVector step        = fDirection;
   step.setMag(step_size);

   for(int i = 0; i < fNplanes; i++) {

      red       = 177.0/256.0;
      green     = 104.0/256.0;
      blue      = 177.0/256.0;
      alpha     = 0.4;

      scoring_solid  = fDet_solid[i];
      scoring_log    = fDet_log[i];
      scoring_phys   = fDet_phys[i];
      scoring_det    = fDet_det[i];
      scoring_vis    = fDet_vis[i];

      if(scoring_phys)  delete scoring_phys;
      if(scoring_log)   delete scoring_log;
      if(scoring_solid) delete scoring_solid;
      //if(scoring_det)   delete scoring_det;

      std::string detname_solid = "scoring_solid_" + std::to_string(i);
      std::string detname_log   = "scoring_log_"   + std::to_string(i);
      std::string detname_phys  = "scoring_phys_"  + std::to_string(i);

      scoring_solid = new G4Tubs(detname_solid, 0.0, fDet_size/2.0, scoring_length/2.0, 0.0, 360.*deg );
      scoring_log   = new G4LogicalVolume(scoring_solid, 0, detname_log);
      scoring_phys  = new G4PVPlacement(0,scoring_pos, scoring_log, detname_phys, worldLogical,false,0,checkOverlaps);                                  

      G4Colour            scoring_color {red, green, blue, alpha };   // Gray 
      if(!scoring_vis) {
         scoring_vis   = new G4VisAttributes(scoring_color);
      } else {
         scoring_vis->SetColour(scoring_color);
      }
      scoring_log->SetVisAttributes(scoring_vis);

      //G4UserLimits * scoring_limits = new G4UserLimits(0.004*um);
      //scoring_log->SetUserLimits(scoring_limits);

      if(!scoring_det){
         std::string sdname = "/p" + std::to_string(i);
         scoring_det = new FakeSD(sdname);
      }

      //SetSensitiveDetector("scoring_log",scoring_det);
      G4SDManager::GetSDMpointer()->AddNewDetector(scoring_det);
      scoring_log->SetSensitiveDetector(scoring_det);

      scoring_pos += step;
   }

   //
   // material defined in the mass world
   //
   //G4Material* water = G4Material::GetMaterial("G4_WATER");

   ////
   //// parallel world placement box
   ////
   //G4VSolid* paraBox = new G4Box("paraBox",5.0*cm,30.0*cm,5.0*cm);
   //G4LogicalVolume* paraBoxLogical = new G4LogicalVolume(paraBox,water,
   //                                                      "paraBox");
   //new G4PVPlacement(0,G4ThreeVector(-25.0*cm,0.,0.),paraBoxLogical,
   //                  "paraBox",worldLogical,false,0);

   ////
   //// mother of parallel world parameterized volumes
   ////
   //G4VSolid* paraMom = new G4Box("paraMom",20.0*cm,40.0*cm,20.0*cm);
   //G4LogicalVolume* paraMomLogical = new G4LogicalVolume(paraMom,0,"paraMom");
   //new G4PVPlacement(0,G4ThreeVector(10.0*cm,0.,0.),paraMomLogical,"paraMom",
   //                  worldLogical,false,0);

   ////
   //// parallel world parameterized volumes
   ////
   //G4VSolid* paraPara = new G4Box("paraPara",5.0*cm,15.0*cm,10.0*cm);
   //G4LogicalVolume* paraParaLogical = new G4LogicalVolume(paraPara,water,
   //                                                       "paraPara");
   //B1ParallelWorldParam* param = new B1ParallelWorldParam();
   //new G4PVParameterised("paraPara",paraParaLogical,paraMomLogical,
   //                         kXAxis, 2, param);
   fNeedsRebuilt = false;

}
//______________________________________________________________________________

