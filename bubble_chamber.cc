#include "BubbleChamberSim.h"
#include "B1DetectorConstruction.hh"
#include "B1ActionInitialization.hh"
#include "G4SystemOfUnits.hh"

#include <cstdio>
#include <iostream>
#include "clipp.h"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "QBBC.hh"
#include "G4UIQt.hh"
#include "G4UIterminal.hh"
#include <qmainwindow.h>

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4PhysListFactory.hh"
#include "G4StepLimiterPhysics.hh"
#include "Randomize.hh"

#include "B1ParallelWorldConstruction.hh"
#include "G4ParallelWorldPhysics.hh"

bool fexists(const std::string& filename) {
   std::ifstream ifile(filename.c_str());
   if( ifile ) return true;
   return false;
}
//______________________________________________________________________________

template<class C>
void print_help(C cli)
{
  std::cout << make_man_page(cli, "bubble_chamber") << "\n";
  //std::cout << "usage: bubble_chamber [options] [macro file]    \n";
  //std::cout << "Options:                               \n";
  //std::cout << "    --run=#, -r         set file \"run\" number\n";
  //std::cout << "    --gui=#, -g         set to 1 (default) to use qt gui or\n";
  //std::cout << "                        0 to use command line\n";
  //std::cout << "    --vis=#, -V         set to 1 (default) to visualization geometry and events\n";
  //std::cout << "                        0 to turn off visualization\n";
  //std::cout << "    --interactive, -i   run in interactive mode (default)\n";
  //std::cout << "    --batch, -b         run in batch mode\n"; 
}

//______________________________________________________________________________

struct settings {
  int          run_number        = 0;
  int          number_of_events  = -1;
  std::string  output_file_name  = "";
  std::string  output_tree_name  = "";
  std::string  theRest           = "";
  std::string  macroFile         = "";
  bool         run_manager_init  = false;
  bool         use_gui           = true;
  bool         use_vis           = true;
  bool         is_interactive    = true;
  bool         has_macro_file    = false;
  bool         help              = false;
};

int main(int argc,char** argv)
{
  using namespace clipp;
  using std::cout;
  settings S;

  auto cli = (
    (option("-r", "--run") & value("run_number",S.run_number))      % "set the run number",
    (option("-m", "--macro") & value("macro",S.macroFile).set(S.has_macro_file,true))      % "set the run number",
    option("-b", "--batch")([&](){S.use_gui=false; S.use_vis=false; S.is_interactive=false;}) % "run in batch mode (no gui or vis)",
    (option("-g", "--gui") & integer("gui")([&](auto v){S.use_gui=v;}))                 % "use GUI ",
    option("-i", "--interactive")([&](){S.is_interactive=true;}) % "run in interactivemode ",
    (option("-v", "--vis") & integer("vis")([&](auto v){S.use_vis=v;})) % "use vis ",
    option("-h", "--help").set(S.help,true)
    );

  if(!parse(argc, argv, cli)) {
    cout << make_man_page(cli, argv[0]);
    return 0;
  }

  if(S.help) {
    print_help(cli);
    return 0;
  }


  //---------------------------------------------------------------------------

  //int index = 0;
  //int iarg  = 0;
  //opterr    = 1; //turn off getopt error message
  //const struct option longopts[] =
  //{
  //  {"run",         required_argument,  0, 'r'},
  //  {"gui",         required_argument,  0, 'g'},
  //  {"vis",         required_argument,  0, 'V'},
  //  {"interactive", no_argument,        0, 'i'},
  //  {"batch",       no_argument,        0, 'b'},
  //  {"output",      required_argument,  0, 'o'},
  //  {"tree",        required_argument,  0, 't'},
  //  {"help",        no_argument,        0, 'h'},
  //  {"init",        no_argument,        0, 'I'},
  //  {0,0,0,0}
  //};
  //while(iarg != -1) {
  //  iarg = getopt_long(argc, argv, "o:h:g:r:V:ibhI", longopts, &index);

  //  switch (iarg)
  //  {
  //    case 'b':
  //      is_interactive = false;
  //      use_gui = false;
  //      use_vis = false;
  //      break;

  //    case 'i':
  //      is_interactive = true;
  //      break;

  //    case 'V':
  //      if( atoi(optarg) == 0 ){
  //        use_vis = false;
  //      } else  {
  //        use_vis = true;
  //      }
  //      break;

  //    case 'r':
  //      run_number = atoi( optarg );
  //      break;

  //    case 'g':
  //      if( atoi(optarg) == 0 ){
  //        use_gui = false;
  //      } else  {
  //        use_gui = true;
  //      }
  //      break;

  //    case 't':
  //      output_tree_name = optarg;
  //      break;

  //    case 'I':
  //      run_manager_init = true;
  //      break;

  //    case 'o':
  //      output_file_name = optarg;
  //      if( fexists(output_file_name) ) {
  //        std::cout << "Error : " << output_file_name << " already exist"  << std::endl;
  //        exit(EXIT_FAILURE);
  //      }
  //      break;

  //    case 'h':
  //      print_help();
  //      exit(0);
  //      break;

  //    case '?':
  //      print_help();
  //      exit(EXIT_FAILURE);
  //      break;
  //  }
  //}

  // here we assume the last argument is a macro file 
  if( optind < argc ) {
    S.has_macro_file = true;
  }
  for (int i = optind; i < argc; i++) {
    S.theRest        += argv[i];
  }

  // Get the piped commands
  std::vector<std::string> piped_commands;
  if(!isatty(STDIN_FILENO)) {
    std::cout << "Reading piped commands...\n";
    std::string lineInput;
    while(std::getline(std::cin,lineInput)) {
      piped_commands.push_back(lineInput);
    }
  }

  std::cout << " the rest of the arguments: " << S.theRest << std::endl;
  std::cout << "output : " << S.output_file_name << std::endl;
  std::cout << "  tree : " << S.output_tree_name << std::endl;

  //---------------------------------------------------------------------------

  // Detect interactive mode (if no arguments) and define UI session
  // Note third argument of G4UIExecutive can be ("qt", "xm", "win32", "gag", "tcsh", "csh")
  G4UIExecutive* ui = 0;
  if( S.is_interactive || S.use_gui ) {
    if( S.use_gui ) {
      ui = new G4UIExecutive(argc, argv, "qt");
    } else {
      ui = new G4UIExecutive(argc, argv, "tcsh");
    }
  }

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4long seed = time(NULL);
  CLHEP::HepRandom::setTheSeed(seed);

  // Construct the default run manager
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // Set mandatory initialization classes
  //
  // Detector construction with parallel world 
  G4String                        paraWorldName = "ParallelWorld";
  B1DetectorConstruction        * realWorld     = new B1DetectorConstruction();
  B1ParallelWorldConstruction * parallelWorld = new B1ParallelWorldConstruction(paraWorldName);

  realWorld->RegisterParallelWorld(parallelWorld);
  runManager->SetUserInitialization(realWorld);

  // Physics list
  G4PhysListFactory     factory;
  auto avail_phy_lists = factory.AvailablePhysLists();
  std::cout << " AvailablePhysLists\n";
  for(const auto& l : avail_phy_lists) {
    std::cout << l << std::endl;
  }
  std::cout << " AvailablePhysListsEM\n";
  auto avail_phy_listsEM = factory.AvailablePhysListsEM();
  for(const auto& l : avail_phy_listsEM) {
    std::cout << l << std::endl;
  }
  //QGSP_BIC_EMY,
  //QGSP_BERT_HP_PEN
  //QGSP_BIC_LIV
  //FTFP_BERT_HP
  G4VModularPhysicsList * physicsList =  factory.GetReferencePhysList("Shielding_EMZ");

  // This is needed to make use of the G4UserLimits applied to logical volumes.
  physicsList->RegisterPhysics(new G4StepLimiterPhysics());

  // This connects the phyics to the parallel world (and sensitive detectors)
  physicsList->RegisterPhysics(new G4ParallelWorldPhysics(paraWorldName,/*layered_mass=*/true));
  //physicsList->ReplacePhysics(new G4IonQMDPhysics());
  //physicsList->SetDefaultCutValue(0.005*um);

  //G4VModularPhysicsList* physicsList = new QBBC;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new B1ActionInitialization(S.run_number));

  // Initialize G4 kernel
  runManager->Initialize();

  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  G4UIQt* qui = static_cast<G4UIQt*> (UImanager->GetG4UIWindow());
  if (qui) {
    qui->GetMainWindow()->setVisible(true);
  }

  // Process macro or start UI session

  if( S.has_macro_file ) {
    
    G4String command = "/control/execute ";
    G4String fileName = S.macroFile;
    UImanager->ApplyCommand(command+fileName);

  } else {

    // run init
    // interactive mode
    G4String command   = "/control/macroPath ";
    G4String mac_dir   = BUBBLESIM_MACRO_DIR;
    G4String fileName  = "init_default.mac";

    std::cout << " executing " << command+mac_dir << std::endl;
    UImanager->ApplyCommand(command+mac_dir);

    command = "/control/execute ";

    if( S.use_vis ) { 
      std::cout << " executing " << command+fileName << std::endl;
      UImanager->ApplyCommand(command+fileName);
    }
  }

  // run the piped commands
  for(auto cmd : piped_commands){
    std::cout << cmd << std::endl;
    UImanager->ApplyCommand(G4String(cmd));
  }

  // run the set number of events
  if( S.number_of_events > 0) {
    G4String command = "/run/beamOn " + std::to_string(S.number_of_events);
    UImanager->ApplyCommand( command );
  }

  if( S.is_interactive )  {
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !

  delete visManager;
  delete runManager;
}
//______________________________________________________________________________
