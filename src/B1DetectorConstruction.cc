#include "B1DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4UserLimits.hh"
#include "FakeSD.hh"

//___________________________________________________________________


B1DetectorConstruction::B1DetectorConstruction() : 
   G4VUserDetectorConstruction(), fScoringVolume(0)
{ }
//___________________________________________________________________

B1DetectorConstruction::~B1DetectorConstruction()
{ }
//___________________________________________________________________

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
   // Get nist material manager
   G4NistManager* nist = G4NistManager::Instance();
   //G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");

   // ------------------------------------------------------------------------
   // basic parameters
   double world_x = 1.0*m;
   double world_y = 1.0*m;
   double world_z = 1.0*m;
   double radiator_thickness      = 6.0*mm;
   double radiator_collimator_gap = 1.0*mm;
   double collimater_target_center_gap = 4.0*cm;
   double collimator_length       = 10.0*cm - radiator_collimator_gap-collimater_target_center_gap;
   double collimator_ID           = 1.0*cm;
   double collimator_OD           = 4.0*2.54*cm;

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

   // ------------------------------------------------------------------------
   // Elements
   // ------------------------------------------------------------------------
   //G4Element* elH  = new G4Element("Hydrogen","H",  Z=1.,  A=1.00794*g/mole);
   //G4Element* elSi = new G4Element("Silicon", "Si", Z=14., A= 28.0855*g/mole);
   //G4Element* elO  = new G4Element("Oxygen",  "O",  Z=8.,  A= 15.9994*g/mole);
   //G4Element* elC  = new G4Element("Carbon",  "C",  Z=6.,  A= 12.011 *g/mole);
   //G4Element* elN  = new G4Element("Nitrogen","N",  Z=7.,  A= 14.00674*g/mole);
   //G4Element* elNa = new G4Element("Sodium",  "Na", Z=11., A= 22.989768*g/mole);
   //G4Element* elAr = new G4Element("Argon",   "Ar", Z=18., A= 39.948*g/mole);
   //G4Element* elI  = new G4Element("Iodine",  "I",  Z=53., A= 126.90447*g/mole);
   //G4Element* elCs = new G4Element("Cesium",  "Cs", Z=55., A= 132.90543*g/mole);

   // ------------------------------------------------------------------------
   // World
   // ------------------------------------------------------------------------
   density     = universe_mean_density;
   pressure    = 1.e-7*bar;
   temperature = 0.1*kelvin;
   red         = 0.0/256.0;
   green       = 200.0/256.0;
   blue        = 0.0/256.0;
   alpha       = 0.4;

   G4Material        * world_mat   = nist->FindOrBuildMaterial("G4_AIR");
   G4Box             * world_solid = new G4Box( "World", 0.5*world_x, 0.5 * world_y, 0.5 * world_z );
   G4LogicalVolume   * world_log   = new G4LogicalVolume( world_solid, world_mat, "world_log" );
   G4VPhysicalVolume * world_phys  = new G4PVPlacement( 0, G4ThreeVector(), world_log, "world_phys", 0, false, 0, checkOverlaps );

   G4Colour            world_color {red, green, blue, alpha }; 
   G4VisAttributes   * world_vis   = new G4VisAttributes(world_color);
   //(*world_vis) = G4VisAttributes::GetInvisible();
   world_vis->SetForceWireframe(true);
   world_log->SetVisAttributes(world_vis);

   // ------------------------------------------------------------------------
   // beam vacuum  
   // ------------------------------------------------------------------------
   double beampipe_length   = 20.0*cm;
   double beampipe_diameter = 8.0*cm;
   density     = universe_mean_density;
   pressure    = 1.e-7*bar;
   temperature = 0.1*kelvin;
   red       = 0.0/256.0;
   green     = 0.0/256.0;
   blue      = 192.0/256.0;
   alpha     = 0.4;

   G4ThreeVector beampipe_pos { 0, 0, -beampipe_length/2.0 - radiator_thickness/2.0 };

   G4Material        * beampipe_mat   = new G4Material("beampipe_mat", /*z=*/1.0, /*a=*/1.01*g/mole, density, kStateGas,temperature,pressure);
   G4VSolid          * beampipe_solid  = new G4Tubs("beampipe_solid", 0.0, beampipe_diameter/2.0, beampipe_length/2.0, 0.0, 360.*deg );
   G4LogicalVolume   * beampipe_log   = new G4LogicalVolume(beampipe_solid, beampipe_mat,"beampipe_log");
   G4VPhysicalVolume * beampipe_phys  = new G4PVPlacement(0,beampipe_pos, beampipe_log, "beampipe_phys",world_log,false,0,checkOverlaps);                                  

   G4Colour            beampipe_color {red, green, blue, alpha };   // Gray 
   G4VisAttributes   * beampipe_vis   = new G4VisAttributes(beampipe_color);
   beampipe_log->SetVisAttributes(beampipe_vis);

   // ------------------------------------------------------------------------
   // radiator target centered at origin
   // ------------------------------------------------------------------------
   double radiator_diameter = 8.0*cm;
   red       = 215.0/256.0;
   green     = 97.0/256.0;
   blue      = 1.0/256.0;
   alpha     = 0.4;

   G4ThreeVector radiator_pos { 0, 0, 0.0 };

   G4Material        * radiator_mat   = nist->FindOrBuildMaterial("G4_Cu");
   G4VSolid          * radiator_solid  = new G4Tubs("radiator_solid", 0.0, radiator_diameter/2.0, radiator_thickness/2.0, 0.0, 360.*deg );
   G4LogicalVolume   * radiator_log   = new G4LogicalVolume(radiator_solid, radiator_mat,"radiator_log");
   G4VPhysicalVolume * radiator_phys  = new G4PVPlacement(0,radiator_pos, radiator_log, "radiator_phys",world_log,false,0,checkOverlaps);                                  

   G4Colour            radiator_color {red, green, blue, alpha };   // Gray 
   G4VisAttributes   * radiator_vis   = new G4VisAttributes(radiator_color);
   radiator_log->SetVisAttributes(radiator_vis);

   //G4UserLimits * scoring_limits = new G4UserLimits(0.004*um);
   //scoring_log->SetUserLimits(scoring_limits);

   // ------------------------------------------------------------------------
   // Cu Collimator 
   // ------------------------------------------------------------------------
   double collimator_diameter = 8.0*cm;
   red       = 215.0/256.0;
   green     = 97.0/256.0;
   blue      = 1.0/256.0;
   alpha     = 0.4;

   G4ThreeVector collimator_pos { 0, 0, collimator_length/2.0 + radiator_thickness/2.0 + radiator_collimator_gap };

   double collimator_z_end = collimator_length + radiator_thickness/2.0 + radiator_collimator_gap;

   G4Material        * collimator_mat   = nist->FindOrBuildMaterial("G4_Ag");
   G4VSolid          * collimator_solid = new G4Tubs("collimator_solid", collimator_ID/2.0, collimator_OD/2.0, collimator_length/2.0, 0.0, 360.*deg );
   G4LogicalVolume   * collimator_log   = new G4LogicalVolume(collimator_solid, collimator_mat,"collimator_log");
   G4VPhysicalVolume * collimator_phys  = new G4PVPlacement(0,collimator_pos, collimator_log, "collimator_phys",world_log,false,0,checkOverlaps);                                  

   G4Colour            collimator_color {red, green, blue, alpha };   // Gray 
   G4VisAttributes   * collimator_vis   = new G4VisAttributes(collimator_color);
   collimator_log->SetVisAttributes(collimator_vis);

   std::cout << "------------------------------------------------------------------------\n";
   std::cout << " Mass is " << collimator_log->GetMass()/kg << " kg\n";

   // ------------------------------------------------------------------------
   // Scoring volume 
   // ------------------------------------------------------------------------
   double scoring_diameter = 20.0*cm;
   double scoring_length = 1*mm;
   red       = 177.0/256.0;
   green     = 104.0/256.0;
   blue      = 177.0/256.0;
   alpha     = 0.4;

   G4ThreeVector scoring_pos { 0, 0, radiator_thickness/2.0 + radiator_collimator_gap/2.0 };

   G4Material        * scoring_mat   = nist->FindOrBuildMaterial("G4_AIR");
   G4VSolid          * scoring_solid  = new G4Tubs("scoring_solid", 0.0, scoring_diameter/2.0, scoring_length/2.0, 0.0, 360.*deg );
   G4LogicalVolume   * scoring_log   = new G4LogicalVolume(scoring_solid, scoring_mat,"scoring_log");
   G4VPhysicalVolume * scoring_phys  = new G4PVPlacement(0,scoring_pos, scoring_log, "scoring_phys",world_log,false,0,checkOverlaps);                                  

   G4Colour            scoring_color {red, green, blue, alpha };   // Gray 
   G4VisAttributes   * scoring_vis   = new G4VisAttributes(scoring_color);
   scoring_log->SetVisAttributes(scoring_vis);

   //G4UserLimits * scoring_limits = new G4UserLimits(0.004*um);
   //scoring_log->SetUserLimits(scoring_limits);

   FakeSD * scoring_det = new FakeSD("/FakeSD1");
   SetSensitiveDetector("scoring_log",scoring_det);

   // ------------------------------------------------------------------------
   // window 
   // ------------------------------------------------------------------------
   double window_diameter  = 1.0*cm;
   double window_thickness = 3.0*mm;
   red       = 5.0/256.0;
   green     = 255.0/256.0;
   blue      = 1.0/256.0;
   alpha     = 0.4;

   G4ThreeVector window_pos { 0, 0, collimator_z_end - window_thickness/2.0 };

   G4Material        * window_mat   = nist->FindOrBuildMaterial("G4_Cu");
   G4VSolid          * window_solid  = new G4Tubs("window_solid", 0.0, window_diameter/2.0, window_thickness/2.0, 0.0, 360.*deg );
   G4LogicalVolume   * window_log   = new G4LogicalVolume(window_solid, window_mat,"window_log");
   G4VPhysicalVolume * window_phys  = new G4PVPlacement(0,window_pos, window_log, "window_phys",world_log,false,0,checkOverlaps);                                  

   G4Colour            window_color {red, green, blue, alpha };   // Gray 
   G4VisAttributes   * window_vis   = new G4VisAttributes(window_color);
   window_log->SetVisAttributes(window_vis);

   // ------------------------------------------------------------------------
   // Scoring volume 
   // ------------------------------------------------------------------------
   double scoring2_diameter = 20.0*cm;
   double scoring2_length = 1*mm;
   red       = 66.0/256.0;
   green     = 174.0/256.0;
   blue      = 208.0/256.0;
   alpha     = 0.4;

   G4ThreeVector scoring2_pos { 0, 0, collimator_z_end + collimater_target_center_gap };

   G4Material        * scoring2_mat   = nist->FindOrBuildMaterial("G4_AIR");
   G4VSolid          * scoring2_solid  = new G4Tubs("scoring2_solid", 0.0, scoring2_diameter/2.0, scoring2_length/2.0, 0.0, 360.*deg );
   G4LogicalVolume   * scoring2_log   = new G4LogicalVolume(scoring2_solid, scoring2_mat,"scoring2_log");
   G4VPhysicalVolume * scoring2_phys  = new G4PVPlacement(0,scoring2_pos, scoring2_log, "scoring2_phys",world_log,false,0,checkOverlaps);                                  

   G4Colour            scoring2_color {red, green, blue, alpha };   // Gray 
   G4VisAttributes   * scoring2_vis   = new G4VisAttributes(scoring2_color);
   scoring2_log->SetVisAttributes(scoring2_vis);

   //G4UserLimits * scoring_limits = new G4UserLimits(0.004*um);
   //scoring_log->SetUserLimits(scoring_limits);

   FakeSD * scoring2_det = new FakeSD("/FakeSD2");
   SetSensitiveDetector("scoring2_log",scoring2_det);


   // --------------------------------------------------------

   fScoringVolume = scoring2_log;

   return world_phys;
}
//___________________________________________________________________


