#include "BubbleChamberSim.h"
#include "B1DetectorConstruction.hh"
#include "B1ActionInitialization.hh"
#include "G4SystemOfUnits.hh"

#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <tuple>
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

#include "GPSConfig.hh"

template<class C>
void print_help(C cli)
{
  std::cout << make_man_page(cli, "bubble_chamber") << "\n";
}

//______________________________________________________________________________


//______________________________________________________________________________

bool fexists(const std::string& filename) {
   std::ifstream ifile(filename.c_str());
   if( ifile ) return true;
   return false;
}
//______________________________________________________________________________

std::string exec(const char* cmd) {
   std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
   if (!pipe) return "ERROR";
   char buffer[128];
   std::string result = "";
   while (!feof(pipe.get())) {
      if (fgets(buffer, 128, pipe.get()) != NULL)
         result += buffer;
   }
   return result;
}
//______________________________________________________________________________


void copy_files(const std::vector<std::string>& files)
{
  for(const auto& f: files) {
    std::string cmd = std::string("cp ") + f + " .";
    if( fexists(f) ) {
      std::cout << "copying " << f << std::endl;
      exec(cmd.c_str());
    }
  }
}
//______________________________________________________________________________

template<typename T>
void print_usage(T cli, const char* argv0 )
{
  //used default formatting
  std::cout << "Usage:\n" << usage_lines(cli, argv0)
            << "\nOptions:\n" << documentation(cli) << '\n';
}
//______________________________________________________________________________

template<typename T>
void print_man_page(T cli, const char* argv0 ){
  //all formatting options (with their default values)
  auto fmt = clipp::doc_formatting{}
  .start_column(8)                           //column where usage lines and documentation starts
  .doc_column(30)                            //parameter docstring start col
  .indent_size(4)                            //indent of documentation lines for children of a documented group
  .line_spacing(0)                           //number of empty lines after single documentation lines
  .paragraph_spacing(1)                      //number of empty lines before and after paragraphs
  .flag_separator(", ")                      //between flags of the same parameter
  .param_separator(" ")                      //between parameters 
  .group_separator(" ")                      //between groups (in usage)
  .alternative_param_separator("|")          //between alternative flags 
  .alternative_group_separator(" | ")        //between alternative groups 
  .surround_group("(", ")")                  //surround groups with these 
  .surround_alternatives("(", ")")           //surround group of alternatives with these
  .surround_alternative_flags("", "")        //surround alternative flags with these
  .surround_joinable("(", ")")               //surround group of joinable flags with these
  .surround_optional("[", "]")               //surround optional parameters with these
  .surround_repeat("", "...");                //surround repeatable parameters with these
  //.surround_value("<", ">")                  //surround values with these
  //.empty_label("")                           //used if parameter has no flags and no label
  //.max_alternative_flags_in_usage(1)         //max. # of flags per parameter in usage
  //.max_alternative_flags_in_doc(2)           //max. # of flags per parameter in detailed documentation
  //.split_alternatives(true)                  //split usage into several lines for large alternatives
  //.alternatives_min_split_size(3)            //min. # of parameters for separate usage line
  //.merge_alternative_flags_with_common_prefix(false)  //-ab(cdxy|xy) instead of -abcdxy|-abxy
  //.merge_joinable_flags_with_common_prefix(true);    //-abc instead of -a -b -c

  auto mp = make_man_page(cli, argv0, fmt);
  mp.prepend_section("DESCRIPTION", "Bubble chamber simulation");
  mp.append_section("EXAMPLES", " $ bubble_chamber -h ");
  std::cout << mp << "\n";
}
//______________________________________________________________________________
//auto define_cli()
//{
//  using namespace clipp;
//  using std::cout;
//  using CopyMode = Settings::CopyMode;
//  using Mode     = Settings::Mode;
//  return std::move(std::make_tuple(S, cli));
//}


int main(int argc,char** argv)
{
  using namespace clipp;
  using std::cout;
  using CopyMode = Settings::CopyMode;
  using Mode     = Settings::Mode;
  Settings  S;
  GPSConfig gps_conf;

  auto cli_basics = (
    ( option("-r", "--run") & value("run_number",S.run_number) )      % "set the run number",
    ( option("-E", "--energy") & number("energy",gps_conf.energy) )      % "set beam energy [MeV]",
    ( option("-e", "--delta") & number("delta",S.delta_E) )      % "beam energy spread[MeV]",
    (option("-V","--vertex").set(S.vertex_set) & value("x", S.vertex[0]) & value("y",  S.vertex[1]) & value("z", S.vertex[2])) % "vertex position [mm]",
    (option("-R","--vertexRMS").set(S.vertex_set) & value("x", S.vertex_rms[0]) & value("y",  S.vertex_rms[1]) & value("z", S.vertex_rms[2])) % "vertex position RMS [mm]",
    ( option("-m", "--macro") & value("macro",S.macroFile).set(S.has_macro_file,true) )      % "set the run number",
    option("-b", "--batch")([&](){S.use_gui=false; S.use_vis=false; S.is_interactive=false;}) % "run in batch mode (no gui or vis)",
    (option("-g", "--gui") & integer("gui")([&](auto v){S.use_gui=v;}))                 % "use GUI ",
    option("-i", "--interactive")([&](){S.is_interactive=true;}) % "run in interactivemode ",
    (option("-v", "--vis") & integer("vis")([&](auto v){S.use_vis=v;})) % "use vis ",
    option("-h", "--help").set(S.selected,Mode::help)
    );
  
  auto help_mode = command("help").set(S.selected, Mode::help);
  auto copy_mode = (command("copy").set(S.selected, Mode::copy) % "Copy example/stock files to the current directory.",
                    (command("macro") % "selec macro to copy",
                     option("-v","--vis").set(S.copy_sel,CopyMode::vis)  % "all vis macro.",
                     option("-v1","--vis1").set(S.copy_sel,CopyMode::v1) % "only vis.mac is copied."
                    "This macro visualizes the detector using the qt gui and runs 1000",
                     option("-v2","--vis2").set(S.copy_sel,CopyMode::v2) % "vis2 mac only")
                   );
  auto gps_build_energy = (
    "Mono energetic GPS distributated like I∝δ(E−E0) with one parameter" % (
      command("Mono")                   % "type name"  &
      value("E0")                       % "E0 fixed energy parameter" & 
      ( option("MeV")|option("GeV") ) % "Units [default:MeV]" )
    |
    "Gaussian distributed  distribution with two parameters. I=(2πσ)−12exp[−(E/E0)2/σ2] Mean energy E0, std deviation σ" % (
      command("Gauss")                  % "type name"  & 
      value("E0")                       % "mean energy" & 
      ( option("MeV")|option("GeV") ) % "Units [default:MeV]"  &
      value("sigma")                    % "sigma: std deviation " & 
      ( option("MeV")|option("GeV") ) % "Units [default:MeV]" )
    |
    "Exp  - exponential    I∝exp(−E/E0) Energy scale-height E0" % (
      command("Exp")                    % " typename " & 
      value("E0")                       % "E0 fixed energy parameter" & 
      ( option("MeV")|option("GeV") ) % "Units [default:MeV]" )
    |
    "Lin  - linear         I∝I0+m×E Intercept I0 , slope m" % ( 
      command("Lin")                    % "type name" & 
      value("I0")                       % "I0 intercept" & 
      value("m")                        % "slope" & 
      ( option("MeV")|option("GeV") ) % "inverse Units [default:MeV]" )
    |
    "Pow  - power-law      I∝Eα Spectral index α" % (
      command("Pow")                    % " typename" &
      value("alpha")                    % "alpha parameter" & 
      ( option("MeV")|option("GeV") ) % "Units [default:MeV]")
    //command("brem")        % "Brem - bremsstrahlung I=∫2E2[h2c2(exp(−E/kT)−1)]−1 Temperature T",
    //command("Bbody")       % "Bbody- black body     I∝(kT)12Eexp(−E/kT) Temperature T",
    //command("cdg")         % "Cdg  - cosmic diffuse gamma ray I∝[(E/Eb)α1+(E/Eb)α2]−1 Energy range Emin to Emax; Eb and indices α1 and α2 are fixed"
    ) ;
  
  auto gps_mode = "gps mode:" % (
    command("gps") & 
    "GPS energy confugration" % (
      command("energy") % "Energy Dist Type. See G4 docs for details (https://tinyurl.com/ydgsc5qj)" & gps_build_energy 
      )
      //|
      //command("angle") % "Angular Dist Type " &
      //"List of Angular Distributions:" % (command("iso") % "iso"| command("flat") % "flat" )
    );
  auto cli = ( help_mode % "print help this help"
              |copy_mode % "Copy mode"
              |gps_mode  
              |cli_basics % "typical style commands");

  assert( cli.flags_are_prefix_free() );

  auto result = parse(argc, argv, cli);

  auto doc_label = [](const parameter& p) {
    if(!p.flags().empty()) return p.flags().front();
    if(!p.label().empty()) return p.label();
    return doc_string{"<?>"};
  };

  cout << "args -> parameter mapping:\n";
  ;
  for(const auto& m0 : result) {
    std::cout << "#" << m0.index() << " " << m0.arg() << " -> ";
    auto p = m0.param();
    if(p) {
      std::cout << doc_label(*p) << " \t";
      if(m0.repeat() > 0) {
        std::cout << (m0.bad_repeat() ? "[bad repeat " : "[repeat ")
        <<  m0.repeat() << "]";
      }
      if(m0.blocked())  std::cout << " [blocked]";
      if(m0.conflict()) std::cout << " [conflict]";
      std::cout << '\n';
    }
    else {
      std::cout << " [unmapped]\n";
    }
  }

  cout << "missing parameters:\n";
  for(const auto& m0 : result.missing()) {
    auto p = m0.param();
    if(p) {
      std::cout << doc_label(*p) << " \t";
      std::cout << " [missing after " << m0.after_index() << "]\n";
    }
  }
  if(!result) {
    print_man_page(cli, argv[0]);
    return 0;
  }

  switch(S.selected) {
    case Mode::help : 
      print_man_page<decltype(cli)>(cli,argv[0]);
      return 0;
      break;
    case Mode::copy :
      if(S.copy_sel == CopyMode::vis) {
        copy_files( S.copymode_file_names[S.copy_sel] );
      }
      return 0;
      break;
    case Mode::none : 
      break;
  }


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
  auto user_action_init = new B1ActionInitialization(S);
  user_action_init->SetBeamEnergy(gps_conf.energy);
  runManager->SetUserInitialization(user_action_init);

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
