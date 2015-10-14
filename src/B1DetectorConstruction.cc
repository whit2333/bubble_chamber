#include "B1DetectorConstruction.hh"
#include "B1DetectorMessenger.hh"

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
   G4VUserDetectorConstruction(), 
   world_x                      ( 1.0*m                  ),
   world_y                      ( 1.0*m                  ),
   world_z                      ( 1.0*m                  ),
   radiator_thickness           ( 6.0*mm                 ),
   collimator_target_center_gap ( 4.0*cm                 ),
   collimator_ID                ( 1.0*cm                 ),
   collimator_OD                ( 2.54*cm                ),
   outer_collimator_ID          ( 2.54*cm                ),
   outer_collimator_OD          ( 4.0*2.54*cm            ),
   collimator_diameter          ( 8.0*cm            ),
   collimator_z_end             ( 0.0*cm            ),
   radiator_collimator_gap      ( 1.0*mm                 ),
   collimator_length            ( 4.0*cm                 ),
   beampipe_length              ( 20.0*cm ),
   beampipe_diameter            ( 8.0*cm  ),
   radiator_diameter            ( 8.0*cm  ),
   scoring_diameter             ( 20.0*cm ),
   scoring_length               ( 0.01*mm    ),
   window_diameter              ( 1.0*cm  ),
   window_thickness             ( 8.0*mm  ),
   scoring2_diameter            ( 20.0*cm ),
   scoring2_length              ( 0.01*mm    ),
   fScoringVolume               ( 0),
   fHasBeenBuilt(false)
{
   fMessenger = new B1DetectorMessenger(this);
   fCollimatorMatName = "G4_Cu";
   scoring_det      = 0;
   scoring2_det     = 0;

   world_mat        = 0;
   world_solid      = 0;
   world_log        = 0;
   world_phys       = 0;

   beampipe_mat     = 0;
   beampipe_solid   = 0;
   beampipe_log     = 0;
   beampipe_phys    = 0;
   radiator_mat     = 0;
   radiator_solid   = 0;
   radiator_log     = 0;
   radiator_phys    = 0;
   collimator_mat   = 0;
   collimator_solid = 0;
   collimator_log   = 0;
   collimator_phys  = 0;
   collimator2_mat   = 0;
   collimator2_solid = 0;
   collimator2_log   = 0;
   collimator2_phys  = 0;
   outer_collimator_mat   = 0;
   outer_collimator_solid = 0;
   outer_collimator_log   = 0;
   outer_collimator_phys  = 0;
   scoring_mat      = 0;
   scoring_solid    = 0;
   scoring_log      = 0;
   scoring_phys     = 0;
   window_mat       = 0;
   window_solid     = 0;
   window_log       = 0;
   window_phys      = 0;
   scoring2_mat     = 0;
   scoring2_solid   = 0;
   scoring2_log     = 0;
   scoring2_phys    = 0;
}
//___________________________________________________________________

B1DetectorConstruction::~B1DetectorConstruction()
{
   delete fMessenger;
}
//______________________________________________________________________________

void B1DetectorConstruction::Rebuild()
{
   B1DetectorConstruction::Construct();
   G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
//______________________________________________________________________________

void B1DetectorConstruction::CalculatePositions()
{
   beampipe_pos     = { 0, 0, -beampipe_length/2.0 - radiator_thickness/2.0 };
   radiator_pos     = { 0, 0, 0.0 };
   collimator_pos   = { 0, 0, collimator_length/4.0 + radiator_thickness/2.0 + radiator_collimator_gap };
   collimator2_pos   = { 0, 0, 3.0*collimator_length/4.0 + radiator_thickness/2.0 + radiator_collimator_gap };
   outer_collimator_pos   = { 0, 0, collimator_length/2.0 + radiator_thickness/2.0 + radiator_collimator_gap };
   collimator_z_end = collimator_length + radiator_thickness/2.0 + radiator_collimator_gap;
   scoring_pos      = { 0, 0, radiator_thickness/2.0 + radiator_collimator_gap/2.0 };
   window_pos       = { 0, 0, collimator_z_end - window_thickness/2.0 };
   scoring2_pos     = { 0, 0, collimator_z_end + collimator_target_center_gap };
}
//______________________________________________________________________________
G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
   //std::cout << "============================================================" << std::endl;
   //std::cout << "B1DetectorConstruction::Construct()" << std::endl;

   // Get nist material manager

   G4NistManager* nist = G4NistManager::Instance();

   CalculatePositions();

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
   // World
   // ------------------------------------------------------------------------
   density     = universe_mean_density;
   pressure    = 1.e-7*bar;
   temperature = 0.1*kelvin;
   red         = 0.0/256.0;
   green       = 200.0/256.0;
   blue        = 0.0/256.0;
   alpha       = 0.4;

   world_mat   = nist->FindOrBuildMaterial("G4_AIR");
   if(!world_solid) world_solid = new G4Box( "World", 0.5*world_x, 0.5 * world_y, 0.5 * world_z );
   if(!world_log)   world_log = new G4LogicalVolume( world_solid, world_mat, "world_log" );
   if(!world_phys)  world_phys  = new G4PVPlacement( 0, G4ThreeVector(), world_log, "world_phys", 0, false, 0, checkOverlaps );

   G4Colour            world_color {red, green, blue, alpha }; 
   G4VisAttributes   * world_vis   = new G4VisAttributes(world_color);
   //(*world_vis) = G4VisAttributes::GetInvisible();
   world_vis->SetForceWireframe(true);
   world_log->SetVisAttributes(world_vis);

   // ------------------------------------------------------------------------
   // beam vacuum  
   // ------------------------------------------------------------------------
   density     = universe_mean_density;
   pressure    = 1.e-7*bar;
   temperature = 0.1*kelvin;
   red       = 0.0/256.0;
   green     = 0.0/256.0;
   blue      = 192.0/256.0;
   alpha     = 0.4;


   if(!beampipe_mat)   beampipe_mat   = new G4Material("beampipe_mat", /*z=*/1.0, /*a=*/1.01*g/mole, density, kStateGas,temperature,pressure);
   if(!beampipe_solid) beampipe_solid  = new G4Tubs("beampipe_solid", 0.0, beampipe_diameter/2.0, beampipe_length/2.0, 0.0, 360.*deg );
   if(!beampipe_log  ) beampipe_log   = new G4LogicalVolume(beampipe_solid, beampipe_mat,"beampipe_log");
   if(!beampipe_phys ) beampipe_phys  = new G4PVPlacement(0,beampipe_pos, beampipe_log, "beampipe_phys",world_log,false,0,checkOverlaps);                                  

   G4Colour            beampipe_color {red, green, blue, alpha };   // Gray 
   G4VisAttributes   * beampipe_vis   = new G4VisAttributes(beampipe_color);
   beampipe_log->SetVisAttributes(beampipe_vis);

   // ------------------------------------------------------------------------
   // radiator target centered at origin
   // ------------------------------------------------------------------------
   red       = 256.0/256.0;
   green     = 1.0/256.0;
   blue      = 1.0/256.0;
   alpha     = 0.4;

   if(radiator_phys) delete radiator_phys;
   if(radiator_log) delete radiator_log;
   if(radiator_solid) delete radiator_solid;

   radiator_mat   = nist->FindOrBuildMaterial("G4_Cu");
   radiator_solid = new G4Tubs("radiator_solid", 0.0, radiator_diameter/2.0, radiator_thickness/2.0, 0.0, 360.*deg );
   radiator_log   = new G4LogicalVolume(radiator_solid, radiator_mat,"radiator_log");
   radiator_phys  = new G4PVPlacement(0,radiator_pos, radiator_log, "radiator_phys",world_log,false,0,checkOverlaps);                                  

   G4Colour            radiator_color {red, green, blue, alpha };   // Gray 
   G4VisAttributes   * radiator_vis   = new G4VisAttributes(radiator_color);
   radiator_log->SetVisAttributes(radiator_vis);

   //G4UserLimits * scoring_limits = new G4UserLimits(0.004*um);
   //scoring_log->SetUserLimits(scoring_limits);

   // ------------------------------------------------------------------------
   // Inner Collimator 
   // ------------------------------------------------------------------------
   // Part I : Upstream inner cone 
   red       = 250.0/256.0;
   green     = 0.0/256.0;
   blue      = 1.0/256.0;
   alpha     = 0.4;

   if(collimator_phys) delete collimator_phys;
   if(collimator_log)  delete collimator_log;
   if(collimator_solid) delete collimator_solid;

   collimator_mat   = nist->FindOrBuildMaterial(fCollimatorMatName);
   //collimator_solid = new G4Tubs("collimator_solid", collimator_ID/2.0, collimator_OD/2.0, collimator_length/2.0, 0.0, 360.*deg );
   collimator_solid = new G4Cons("collimator_solid", collimator_ID/4.0, collimator_OD/2.0, collimator_ID/2.0, collimator_OD/2.0, collimator_length/4.0, 0.0, 360.*deg );
   collimator_log   = new G4LogicalVolume(collimator_solid, collimator_mat,"collimator_log");
   collimator_phys  = new G4PVPlacement(0,collimator_pos, collimator_log, "collimator_phys",world_log,false,0,checkOverlaps);                                  

   G4Colour            collimator_color {red, green, blue, alpha };   // Gray 
   G4VisAttributes   * collimator_vis   = new G4VisAttributes(collimator_color);
   collimator_log->SetVisAttributes(collimator_vis);

   // ------------------------------------------------------------------------
   // Part II : downstream inner cone 
   red       = 250.0/256.0;
   green     = 0.0/256.0;
   blue      = 1.0/256.0;
   alpha     = 0.4;

   if(collimator2_phys) delete collimator2_phys;
   if(collimator2_log)  delete collimator2_log;
   if(collimator2_solid) delete collimator2_solid;

   collimator2_mat   = nist->FindOrBuildMaterial(fCollimatorMatName);
   //collimator2_solid = new G4Tubs("collimator2_solid", collimator2_ID/2.0, collimator2_OD/2.0, collimator2_length/2.0, 0.0, 360.*deg );
   collimator2_solid = new G4Cons("collimator2_solid", collimator_ID/4.0, collimator_OD/2.0, collimator_ID/2.0, collimator_OD/2.0, collimator_length/4.0, 0.0, 360.*deg );
   collimator2_log   = new G4LogicalVolume(collimator2_solid, collimator2_mat,"collimator2_log");
   collimator2_phys  = new G4PVPlacement(0,collimator2_pos, collimator2_log, "collimator2_phys",world_log,false,0,checkOverlaps);                                  

   G4Colour            collimator2_color {red, green, blue, alpha };   // Gray 
   G4VisAttributes   * collimator2_vis   = new G4VisAttributes(collimator2_color);
   collimator2_log->SetVisAttributes(collimator2_vis);


   // ------------------------------------------------------------------------
   // Outer Collimator 
   // ------------------------------------------------------------------------
   red       = 0.0/256.0;
   green     = 256.0/256.0;
   blue      = 1.0/256.0;
   alpha     = 0.4;

   if(outer_collimator_phys)  delete outer_collimator_phys;
   if(outer_collimator_log)   delete outer_collimator_log;
   if(outer_collimator_solid) delete outer_collimator_solid;

   outer_collimator_mat   = nist->FindOrBuildMaterial("G4_Cu");
   outer_collimator_solid = new G4Tubs("outer_collimator_solid", outer_collimator_ID/2.0, outer_collimator_OD/2.0, collimator_length/2.0, 0.0, 360.*deg );
   outer_collimator_log   = new G4LogicalVolume(outer_collimator_solid, outer_collimator_mat,"outer_collimator_log");
   outer_collimator_phys  = new G4PVPlacement(0,outer_collimator_pos, outer_collimator_log, "outer_collimator_phys",world_log,false,0,checkOverlaps);                                  
   G4Colour            outer_collimator_color {red, green, blue, alpha };   // Gray 
   G4VisAttributes   * outer_collimator_vis   = new G4VisAttributes(outer_collimator_color);
   outer_collimator_vis->SetForceWireframe(true);
   outer_collimator_log->SetVisAttributes(outer_collimator_vis);

   // ------------------------------------------------------------------------
   // Scoring volume 
   // ------------------------------------------------------------------------
   red       = 177.0/256.0;
   green     = 104.0/256.0;
   blue      = 177.0/256.0;
   alpha     = 0.4;

   if(scoring_phys) delete scoring_phys;
   if(scoring_log) delete scoring_log;
   if(scoring_solid) delete scoring_solid;
   //if(scoring_det) delete scoring_det;

   scoring_mat   = nist->FindOrBuildMaterial("G4_AIR");
   scoring_solid = new G4Tubs("scoring_solid", 0.0, scoring_diameter/2.0, scoring_length/2.0, 0.0, 360.*deg );
   scoring_log   = new G4LogicalVolume(scoring_solid, scoring_mat,"scoring_log");
   scoring_phys  = new G4PVPlacement(0,scoring_pos, scoring_log, "scoring_phys",world_log,false,0,checkOverlaps);                                  

   G4Colour            scoring_color {red, green, blue, alpha };   // Gray 
   G4VisAttributes   * scoring_vis   = new G4VisAttributes(scoring_color);
   scoring_log->SetVisAttributes(scoring_vis);

   //G4UserLimits * scoring_limits = new G4UserLimits(0.004*um);
   //scoring_log->SetUserLimits(scoring_limits);

   if(!scoring_det) scoring_det = new FakeSD("/FakeSD1");
   SetSensitiveDetector("scoring_log",scoring_det);

   // ------------------------------------------------------------------------
   // window 
   // ------------------------------------------------------------------------
   red       = 5.0/256.0;
   green     = 255.0/256.0;
   blue      = 1.0/256.0;
   alpha     = 0.4;

   if(window_phys) delete window_phys;
   if(window_log) delete window_log;
   if(window_solid) delete window_solid;

   double cone_slope = (collimator_ID/4.0)/(collimator_length/2.0);
   window_mat   = nist->FindOrBuildMaterial("G4_Cu");
   //window_solid  = new G4Tubs("window_solid", 0.0, window_diameter/2.0, window_thickness/2.0, 0.0, 360.*deg );
   window_solid = new G4Cons("collimator2_solid", 0.0, window_diameter/2.0 - cone_slope*window_thickness, 0.0, window_diameter/2.0, window_thickness/2.0, 0.0, 360.*deg );
   window_log   = new G4LogicalVolume(window_solid, window_mat,"window_log");
   window_phys  = new G4PVPlacement(0,window_pos, window_log, "window_phys",world_log,false,0,checkOverlaps);                                  

   G4Colour            window_color {red, green, blue, alpha };   // Gray 
   G4VisAttributes   * window_vis   = new G4VisAttributes(window_color);
   window_log->SetVisAttributes(window_vis);

   // ------------------------------------------------------------------------
   // Scoring volume 
   // ------------------------------------------------------------------------
   red       = 66.0/256.0;
   green     = 174.0/256.0;
   blue      = 208.0/256.0;
   alpha     = 0.4;

   if(scoring2_phys) delete scoring2_phys;
   if(scoring2_log) delete scoring2_log;
   if(scoring2_solid) delete scoring2_solid;
   //if(scoring2_det) delete scoring2_det;

   scoring2_mat   = nist->FindOrBuildMaterial("G4_AIR");
   scoring2_solid  = new G4Tubs("scoring2_solid", 0.0, scoring2_diameter/2.0, scoring2_length/2.0, 0.0, 360.*deg );
   scoring2_log   = new G4LogicalVolume(scoring2_solid, scoring2_mat,"scoring2_log");
   scoring2_phys  = new G4PVPlacement(0,scoring2_pos, scoring2_log, "scoring2_phys",world_log,false,0,checkOverlaps);                                  

   G4Colour            scoring2_color {red, green, blue, alpha };   // Gray 
   G4VisAttributes   * scoring2_vis   = new G4VisAttributes(scoring2_color);
   scoring2_log->SetVisAttributes(scoring2_vis);

   //G4UserLimits * scoring_limits = new G4UserLimits(0.004*um);
   //scoring_log->SetUserLimits(scoring_limits);

   if(!scoring2_det) scoring2_det = new FakeSD("/FakeSD2");
   SetSensitiveDetector("scoring2_log",scoring2_det);


   // --------------------------------------------------------

   fScoringVolume = scoring2_log;
   fHasBeenBuilt = true;

   return world_phys;
}
//___________________________________________________________________

void B1DetectorConstruction::SetCollimatorMaterial(G4String materialName)
{
   fCollimatorMatName = materialName;
   if(fHasBeenBuilt) Rebuild();
}
//______________________________________________________________________________

void     B1DetectorConstruction::SetRadiatorCollimatorGap(G4double l)
{   
   radiator_collimator_gap = l; 
   if(fHasBeenBuilt) Rebuild();
}
//______________________________________________________________________________

void     B1DetectorConstruction::SetInnerCollimatorOD(G4double l)
{   
   collimator_OD       = l;
   outer_collimator_ID = l;
   if(fHasBeenBuilt) Rebuild();
}
//______________________________________________________________________________

void     B1DetectorConstruction::SetCollimatorLength(G4double l)
{   
   collimator_length = l;
   if(fHasBeenBuilt) Rebuild();
}
//______________________________________________________________________________

void     B1DetectorConstruction::PrintConfigInfo() const
{   

   if(fHasBeenBuilt) {
      std::cout << "------------------------------------------------------------------------\n";
      std::cout 
         << " Collimator parameters :  \n"
         << "                   weight : " << collimator_log->GetMass(true)/kg << " kg\n"
         << "                 material : " << fCollimatorMatName               << "\n"
         << "                   length : " << collimator_length/cm             << " cm\n"
         << "           collimator ID  : " << collimator_ID/cm                 << " cm\n"
         << "           collimator OD  : " << collimator_OD/cm                 << " cm\n"
         << " radiator collimator gap  : " << radiator_collimator_gap/cm       << " cm\n"
         << "  collimator target dist  : " << collimator_target_center_gap/cm  << " cm\n"
         << "      radiator thickness  : " << radiator_thickness/cm            << " cm\n";
   } else {
      std::cout << " detector not built yet" << std::endl;
   }

}

