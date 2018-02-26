#include "B1DetectorConstruction.hh"
#include "B1DetectorMessenger.hh"

#include "BubbleChamberSim.h"
#include "G4Material.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

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
#include "CADMesh.hh"


//___________________________________________________________________


B1DetectorConstruction::B1DetectorConstruction() : 
   G4VUserDetectorConstruction(), 
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

   chamber_port_flange_mat    = nullptr;  
   chamber_port_flange_solid  = nullptr;
   chamber_port_flange_log    = nullptr;  
   chamber_port_flange_phys   = nullptr;  

   chamber_port_mat    = nullptr;  
   chamber_port_solid  = nullptr;
   chamber_port_log    = nullptr;  
   chamber_port_phys   = nullptr;  

   port_collimator_mat    = nullptr;  
   port_collimator_solid  = nullptr;
   port_collimator_log    = nullptr;  
   port_collimator_phys   = nullptr;  

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
   beampipe_pos         = { 0, 0, -beampipe_length/2.0 - radiator_thickness/2.0 };
   radiator_pos         = { 0, 0, 0.0 };
   collimator_pos       = { 0, 0, collimator_length/2.0 + radiator_thickness/2.0 + radiator_collimator_gap };
   collimator_z_end     = collimator_length + radiator_thickness/2.0 + radiator_collimator_gap;

   // Chamber port flange is the outer part that holds the port
   chamber_port_flange_pos = { 0, 0, radiator_thickness/2.0
                                  + radiator_collimator_gap
                                  + collimator_length
                                  + collimator_chamber_port_gap};
   chamber_port_flange_offset = {0,0, 0.0*chamber_port_flange_length/2.0 };

   // Chamber port is the part which has the window to the chamber and holds the
   // insert
   chamber_port_pos = chamber_port_flange_pos + G4ThreeVector(0,0,
                                  0.0*chamber_flange_port_offset // the port face is recessed by a few mm from the flange face
                                  );
   chamber_port_offset = {0,0, 0.0*chamber_port_length/2.0 };

   // The collimator insert
   G4double insert_extra = std::abs(collimator_insert_length - flange_collimator_length);//+chamber_flange_port_offset;
   port_collimator_pos  = chamber_port_pos + G4ThreeVector(0,0,insert_extra/1.1);// insert almost all the way
   port_collimator_offset = {0,0, 0.0*collimator_insert_length/2.0 };

   steel_chamber_pos    = chamber_port_flange_pos + G4ThreeVector(0,
                                                                  steel_vessel_length/2.0 - steel_vessel_height2 + 
                                                                  steel_vessel_port_top_offset + steel_vessel_port_diameter/2.0,
                                                                  chamber_port_flange_thickness + steel_vessel_length);
   steel_chamber_offset = {-1.0*steel_vessel_length/2.0,-1.0*steel_vessel_length/2.0,0};

   exit_port_flange_pos = chamber_port_flange_pos + G4ThreeVector(0,0,steel_vessel_length + 2.0*chamber_port_flange_thickness);
   exit_port_flange_offset = {0,0,0};

   exit_port_collimator_pos = chamber_port_flange_pos + G4ThreeVector(0,0,steel_vessel_length + 2.0*chamber_port_flange_thickness);
   exit_port_collimator_offset = {0,0,0};

   glass_cell_pos = chamber_port_flange_pos + G4ThreeVector(0,0,chamber_port_flange_thickness + steel_vessel_length/2.0);
   glass_cell_offset = {0,glass_cell_vertical_offset-glass_cell_height_to_tube-glass_cell_tupe_height/2.0,0};

   photon_dump_pos    = glass_cell_pos + G4ThreeVector(0,0,chamber_photon_dump_sep);
   photon_dump_offset = {0,0,0};

   Pb_pig_pos = glass_cell_pos;
   Pb_pig_offset = {559.0*mm, -414.0*mm - 52.0*mm - (367.0-52)*mm/2.0,0};

   std::cout << " Pb_pig_pos    : " << Pb_pig_pos    << std::endl;
   std::cout << " Pb_pig_offset : " << Pb_pig_offset << std::endl;

   scoring_pos          = { 0, 0, radiator_thickness/2.0 + radiator_collimator_gap/2.0 };
   window_pos           = { 0, 0, collimator_z_end - window_thickness/2.0 };
   scoring2_pos         = { 0, 0, collimator_z_end + collimator_target_center_gap };
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
   (*world_vis) = G4VisAttributes::GetInvisible();
   world_vis->SetForceWireframe(true);
   world_log->SetVisAttributes(world_vis);

   // ------------------------------------------------------------------------
   // beam vacuum  
   // ------------------------------------------------------------------------
   density     = universe_mean_density;
   pressure    = 1.e-19*bar;
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
   // Collimator 
   // ------------------------------------------------------------------------
   red       = 250.0/256.0;
   green     = 0.0/256.0;
   blue      = 1.0/256.0;
   alpha     = 0.4;

   if(collimator_phys)  delete collimator_phys;
   if(collimator_log)   delete collimator_log;
   if(collimator_solid) delete collimator_solid;

   collimator_mat   = nist->FindOrBuildMaterial(fCollimatorMatName);
   collimator_solid = new G4Tubs("collimator_solid", collimator_ID/2.0, collimator_OD/2.0, collimator_length/2.0, 0.0, 360.*deg );
   collimator_log   = new G4LogicalVolume(collimator_solid, collimator_mat,"collimator_log");
   collimator_phys  = new G4PVPlacement(0,collimator_pos, collimator_log, "collimator_phys",world_log,false,0,checkOverlaps);                                  

   G4Colour            collimator_color {red, green, blue, alpha };   // Gray 
   G4VisAttributes   * collimator_vis   = new G4VisAttributes(collimator_color);
   collimator_log->SetVisAttributes(collimator_vis);


   // ------------------------------------------------------------------------
   // Chamber Port Flange
   red       = 0.0/256.0;
   green     = 0.0/256.0;
   blue      = 200.0/256.0;
   alpha     = 0.4;

   auto flange_mesh = CADMesh::TessellatedMesh::FromPLY(BUBBLESIM_GEOMETRY_DIR "/plug_flange_beam_port_mod.ply");
   flange_mesh->SetScale( mm );
   flange_mesh->SetOffset( chamber_port_flange_offset );

   chamber_port_flange_mat = nist->FindOrBuildMaterial("G4_Cu");
   chamber_port_flange_solid = flange_mesh->GetSolid();
   chamber_port_flange_log   = new G4LogicalVolume(chamber_port_flange_solid, chamber_port_flange_mat, "chamber_port_flange_log", 0, 0, 0);
   chamber_port_flange_phys  = new G4PVPlacement(0, chamber_port_flange_pos, chamber_port_flange_log,
                                    "chamber_port_flange_phys", world_log, false, 0);

   G4Colour         chamber_port_flange_color {red, green, blue, alpha };
   G4VisAttributes* chamber_port_flange_vis   = new G4VisAttributes(chamber_port_flange_color);
   chamber_port_flange_log->SetVisAttributes(chamber_port_flange_vis);

   // ------------------------------------------------------------------------
   // Chamber Port (which holds an insert) and has window into chamber
   red       = 250.0/256.0;
   green     = 0.0/256.0;
   blue      = 1.0/256.0;
   alpha     = 0.4;

   auto mesh = CADMesh::TessellatedMesh::FromPLY(BUBBLESIM_GEOMETRY_DIR "/internal_coll_and_port.ply");
   mesh->SetScale( mm );
   mesh->SetOffset( chamber_port_offset );

   chamber_port_mat = nist->FindOrBuildMaterial("G4_Cu");
   chamber_port_solid = mesh->GetSolid();
   chamber_port_log   = new G4LogicalVolume(chamber_port_solid, chamber_port_mat, "chamber_port_log", 0, 0, 0);
   chamber_port_phys  = new G4PVPlacement(0, chamber_port_pos, chamber_port_log,
                                    "chamber_port_phys", world_log, false, 0);

   G4Colour         chamber_port_color {red, green, blue, alpha };
   G4VisAttributes* chamber_port_vis   = new G4VisAttributes(chamber_port_color);
   chamber_port_log->SetVisAttributes(chamber_port_vis);


   // -----------------------------------------------
   // Port collimator insert
   red       = 250.0/256.0;
   green     = 250.0/256.0;
   blue      = 0.0/256.0;
   alpha     = 0.4;

   auto mesh2 = CADMesh::TessellatedMesh::FromPLY(BUBBLESIM_GEOMETRY_DIR "/collimator_silver_insert.ply");
   mesh2->SetScale( mm );
   mesh2->SetOffset( port_collimator_offset );

   // Tungsten alloy
   // https://www.tungsten.com/products/tungsten-alloy/
   // 18.5 gm/cc
   //97% W, 2.1% Ni, 0.9% Fe
   density = 18.5*g/cm3;
   G4Material *tungsten_alloy = new G4Material("Tungsten Alloy",density,3);
   tungsten_alloy->AddElement(nist->FindOrBuildElement("W" ), 0.97);
   tungsten_alloy->AddElement(nist->FindOrBuildElement("Ni"), 0.021);
   tungsten_alloy->AddElement(nist->FindOrBuildElement("Fe"), 0.009);

   port_collimator_mat   = tungsten_alloy;
   port_collimator_solid = mesh2->GetSolid();
   port_collimator_log   = new G4LogicalVolume(port_collimator_solid, port_collimator_mat, "port_collimator_log", 0, 0, 0);
   port_collimator_phys  = new G4PVPlacement(0, port_collimator_pos, port_collimator_log,
                                    "port_collimator_phys", world_log, false, 0);

   G4Colour         port_collimator_color {red, green, blue, alpha };
   G4VisAttributes* port_collimator_vis   = new G4VisAttributes(port_collimator_color);
   port_collimator_log->SetVisAttributes(port_collimator_vis);

   // -----------------------------------------------
   // Steel Chamber
   red       = 250.0/256.0;
   green     = 250.0/256.0;
   blue      = 0.0/256.0;
   alpha     = 0.4;

   auto mesh3 = CADMesh::TessellatedMesh::FromPLY(BUBBLESIM_GEOMETRY_DIR "/steel_vessel.ply");
   mesh3->SetScale( mm );
   mesh3->SetOffset( steel_chamber_offset );

   density = 8.02*g/cm3 ;
   G4Material *stainless = new G4Material("Stainless steel",density,5);
   stainless->AddElement(nist->FindOrBuildElement("Mn")/*Mn*/, 0.02);
   stainless->AddElement(nist->FindOrBuildElement("Si")/*Si*/, 0.01);
   stainless->AddElement(nist->FindOrBuildElement("Cr")/*Cr*/, 0.19);
   stainless->AddElement(nist->FindOrBuildElement("Ni")/*Ni*/, 0.10);
   stainless->AddElement(nist->FindOrBuildElement("Fe")/*Fe*/, 0.68);
   G4RotationMatrix* rot3 = new G4RotationMatrix( CLHEP::HepRotationX(90.0*deg) );
    
   steel_chamber_mat   = stainless;//nist->FindOrBuildMaterial("G4_Ag");
   steel_chamber_solid = mesh3->GetSolid();
   steel_chamber_log   = new G4LogicalVolume(steel_chamber_solid, steel_chamber_mat, "steel_chamber_log", 0, 0, 0);
   steel_chamber_phys  = new G4PVPlacement(rot3, steel_chamber_pos+steel_chamber_offset,  steel_chamber_log,
                                             "steel_chamber_phys", world_log, false, 0);

   G4Colour         steel_chamber_color {red, green, blue, alpha };
   G4VisAttributes* steel_chamber_vis   = new G4VisAttributes(steel_chamber_color);
   steel_chamber_log->SetVisAttributes(steel_chamber_vis);

   // ------------------------------------------------------------------------
   // Chamber Exit Port Flange
   red       = 0.0/256.0;
   green     = 0.0/256.0;
   blue      = 200.0/256.0;
   alpha     = 0.4;

   auto exit_flange_mesh = CADMesh::TessellatedMesh::FromPLY(BUBBLESIM_GEOMETRY_DIR "/beam_exit_flange.ply");
   exit_flange_mesh->SetScale( mm );
   exit_flange_mesh->SetOffset( exit_port_flange_offset );

   G4RotationMatrix* rot4 = new G4RotationMatrix( CLHEP::HepRotationY(180.0*deg) );

   exit_port_flange_mat = nist->FindOrBuildMaterial("G4_Cu");
   exit_port_flange_solid = exit_flange_mesh->GetSolid();
   exit_port_flange_log   = new G4LogicalVolume(exit_port_flange_solid, exit_port_flange_mat, "exit_port_flange_log", 0, 0, 0);
   exit_port_flange_phys  = new G4PVPlacement(rot4, exit_port_flange_pos, exit_port_flange_log,
                                    "exit_port_flange_phys", world_log, false, 0);

   G4Colour         exit_port_flange_color {red, green, blue, alpha };
   G4VisAttributes* exit_port_flange_vis   = new G4VisAttributes(exit_port_flange_color);
   exit_port_flange_log->SetVisAttributes(exit_port_flange_vis);

   // ------------------------------------------------------------------------
   // Chamber exit port
   red       = 250.0/256.0;
   green     = 0.0/256.0;
   blue      = 1.0/256.0;
   alpha     = 0.4;

   auto mesh5 = CADMesh::TessellatedMesh::FromPLY(BUBBLESIM_GEOMETRY_DIR "/beam_exit_port_and_collimator.ply");
   mesh5->SetScale( mm );
   mesh5->SetOffset( exit_port_collimator_offset );

   exit_port_collimator_mat = nist->FindOrBuildMaterial("G4_Cu");
   exit_port_collimator_solid = mesh5->GetSolid();
   exit_port_collimator_log   = new G4LogicalVolume(exit_port_collimator_solid, exit_port_collimator_mat, "exit_port_collimator_log", 0, 0, 0);
   exit_port_collimator_phys  = new G4PVPlacement(rot4, exit_port_collimator_pos, exit_port_collimator_log,
                                    "exit_port_collimator_phys", world_log, false, 0);

   G4Colour         exit_port_collimator_color {red, green, blue, alpha };
   G4VisAttributes* exit_port_collimator_vis   = new G4VisAttributes(exit_port_collimator_color);
   exit_port_collimator_log->SetVisAttributes(exit_port_collimator_vis);

   // ------------------------------------------------------------------------
   // Glass Cell
   red       =   0.0/256.0;
   green     = 200.0/256.0;
   blue      =   0.0/256.0;
   alpha     = 0.4;

   auto mesh6 = CADMesh::TessellatedMesh::FromPLY(BUBBLESIM_GEOMETRY_DIR "/glass_to_kovar_single_fluid.ply");
   mesh6->SetScale( mm );
   mesh6->SetOffset( {0,0,0} );

   G4RotationMatrix* rot6 = new G4RotationMatrix( CLHEP::HepRotationX(90.0*deg) );

   glass_cell_mat = nist->FindOrBuildMaterial("G4_Pyrex_Glass");
   glass_cell_solid = mesh6->GetSolid();
   glass_cell_log   = new G4LogicalVolume(glass_cell_solid, glass_cell_mat, "glass_cell_log", 0, 0, 0);
   glass_cell_phys  = new G4PVPlacement(rot6, glass_cell_pos+glass_cell_offset, glass_cell_log,
                                    "glass_cell_phys", world_log, false, 0);

   G4Colour         glass_cell_color {red, green, blue, alpha };
   G4VisAttributes* glass_cell_vis   = new G4VisAttributes(glass_cell_color);
   glass_cell_log->SetVisAttributes(glass_cell_vis);

   // ------------------------------------------------------------------------
   // Photon beam dump
   red       =   0.0/256.0;
   green     =   0.0/256.0;
   blue      = 255.0/256.0;
   alpha     = 0.2;

   photon_dump_mat = nist->FindOrBuildMaterial("G4_Al");
   photon_dump_solid = new G4Box( "photon_dump_dolid", 0.5*4.0*2.54*cm, 0.5*4.0*2.54*cm, 0.5*16.0*2.54*cm );
   photon_dump_log   = new G4LogicalVolume(photon_dump_solid, photon_dump_mat, "photon_dump_log", 0, 0, 0);
   photon_dump_phys  = new G4PVPlacement(0, photon_dump_pos+photon_dump_offset, photon_dump_log,
                                    "photon_dump_phys", world_log, false, 0);

   G4Colour         photon_dump_color {red, green, blue, alpha };
   G4VisAttributes* photon_dump_vis   = new G4VisAttributes(photon_dump_color);
   photon_dump_log->SetVisAttributes(photon_dump_vis);

   // ------------------------------------------------------------------------
   // Pb Pig
   red       =   0.0/256.0;
   green     = 200.0/256.0;
   blue      = 100.0/256.0;
   alpha     = 0.4;

   auto mesh7 = CADMesh::TessellatedMesh::FromPLY(BUBBLESIM_GEOMETRY_DIR "/Pb_pig.ply");
   mesh7->SetScale( mm );
   mesh7->SetOffset( {0,0,0} );

   G4RotationMatrix* rot7 = new G4RotationMatrix( CLHEP::HepRotationX(0.0*deg) );

   Pb_pig_mat = nist->FindOrBuildMaterial("G4_Pb");
   Pb_pig_solid = mesh7->GetSolid();
   Pb_pig_log   = new G4LogicalVolume(Pb_pig_solid, Pb_pig_mat, "Pb_pig_log", 0, 0, 0);
   Pb_pig_phys  = new G4PVPlacement(rot7, Pb_pig_pos+Pb_pig_offset, Pb_pig_log,
                                    "Pb_pig_phys", world_log, false, 0);

   G4Colour         Pb_pig_color {red, green, blue, alpha };
   G4VisAttributes* Pb_pig_vis   = new G4VisAttributes(Pb_pig_color);
   Pb_pig_log->SetVisAttributes(Pb_pig_vis);

   //// ------------------------------------------------------------------------
   //// Part II : downstream inner cone 

   //if(collimator2_phys) delete collimator2_phys;
   //if(collimator2_log)  delete collimator2_log;
   //if(collimator2_solid) delete collimator2_solid;

   //collimator2_mat   = nist->FindOrBuildMaterial(fCollimatorMatName);
   //collimator2_solid = new G4Cons( "collimator2_solid", 
   //      (collimator_upstream_ID)/2.0, collimator_OD/2.0, 
   //      (collimator_downstream_ID)/2.0, collimator_OD/2.0, 
   //      collimator_length/4.0, 0.0, 360.*deg );
   ////collimator2_solid = new G4Cons("collimator2_solid", collimator_ID/2.0, collimator_OD/2.0, collimator_ID/2.0, collimator_OD/2.0, collimator_length/4.0, 0.0, 360.*deg );
   //collimator2_log   = new G4LogicalVolume(collimator2_solid, collimator2_mat,"collimator2_log");
   //collimator2_phys  = new G4PVPlacement(0,collimator2_pos, collimator2_log, "collimator2_phys",world_log,false,0,checkOverlaps);                                  

   //G4Colour            collimator2_color {red, green, blue, alpha };   // Gray 
   //G4VisAttributes   * collimator2_vis   = new G4VisAttributes(collimator2_color);
   //collimator2_log->SetVisAttributes(collimator2_vis);


   //// ------------------------------------------------------------------------
   //// Outer Collimator 
   //// ------------------------------------------------------------------------
   //red       = 0.0/256.0;
   //green     = 256.0/256.0;
   //blue      = 1.0/256.0;
   //alpha     = 0.4;

   //if(outer_collimator_phys)  delete outer_collimator_phys;
   //if(outer_collimator_log)   delete outer_collimator_log;
   //if(outer_collimator_solid) delete outer_collimator_solid;

   //outer_collimator_mat   = nist->FindOrBuildMaterial("G4_Cu");
   //outer_collimator_solid = new G4Tubs("outer_collimator_solid", outer_collimator_ID/2.0, outer_collimator_OD/2.0, collimator_length/2.0, 0.0, 360.*deg );
   //outer_collimator_log   = new G4LogicalVolume(outer_collimator_solid, outer_collimator_mat,"outer_collimator_log");
   //outer_collimator_phys  = new G4PVPlacement(0,outer_collimator_pos, outer_collimator_log, "outer_collimator_phys",world_log,false,0,checkOverlaps);                                  
   //G4Colour            outer_collimator_color {red, green, blue, alpha };   // Gray 
   //G4VisAttributes   * outer_collimator_vis   = new G4VisAttributes(outer_collimator_color);
   //outer_collimator_vis->SetForceWireframe(true);
   //outer_collimator_log->SetVisAttributes(outer_collimator_vis);

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

   window_diameter = collimator_ID;
   window_mat   = nist->FindOrBuildMaterial("G4_Cu");
   //window_solid = new G4Cons("collimator2_solid", 
   //      0.0, window_diameter/2.0 - cone_slope*window_thickness, 
   //      0.0, window_diameter/2.0,
   //      window_thickness/2.0, 0.0, 360.*deg );
   //window_log   = new G4LogicalVolume(window_solid, window_mat,"window_log");
   //window_phys  = new G4PVPlacement(0,window_pos, window_log, "window_phys",world_log,false,0,checkOverlaps);                                  

   //G4Colour            window_color {red, green, blue, alpha };   // Gray 
   //G4VisAttributes   * window_vis   = new G4VisAttributes(window_color);
   //window_log->SetVisAttributes(window_vis);

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

void     B1DetectorConstruction::SetCollimatorOD(G4double l)
{   
   collimator_OD       = l;
   if(fHasBeenBuilt) Rebuild();
}
//______________________________________________________________________________

void     B1DetectorConstruction::SetCollimatorID(G4double l)
{   
   collimator_ID       = l;
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
         << "            collimator ID : " << collimator_ID/cm        << " cm\n"
         << "            collimator OD : " << collimator_OD/cm      << " cm\n"
         << " radiator collimator gap  : " << radiator_collimator_gap/cm       << " cm\n"
         << "  collimator target dist  : " << collimator_target_center_gap/cm  << " cm\n"
         << "      radiator thickness  : " << radiator_thickness/cm            << " cm\n"
         << " chamber_port_pos Z       : " << chamber_port_pos.z()/cm << " cm\n"
         << "chamber port exit z pos   : " << chamber_port_pos.z()/cm + chamber_port_length/cm<< " cm\n";
   } else {
      std::cout << " detector not built yet" << std::endl;
   }

}

