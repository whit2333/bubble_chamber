#ifndef SimulationManager_h
#define SimulationManager_h 1

#include "G4Types.hh"
//class G4SDmessenger;

class SimulationManager 
{
  public: // with description
      static SimulationManager* GetManager();
      // Returns the pointer to the singleton object.
  //public:
  //    static SimulationManager* GetSDMpointerIfExist();

  protected:
      SimulationManager();

  public:
      ~SimulationManager();

  private: 
      static G4ThreadLocal SimulationManager * fSDManager;
      G4int verboseLevel;

      G4messenger* theMessenger;

  public:
      inline void SetVerboseLevel(G4int vl) 
      { 
        verboseLevel = vl; 
        treeTop->SetVerboseLevel(vl);
      }

private:
    //Disable copy constructor and assignment operator
    G4SDManager( const G4SDManager& );
    G4SDManager& operator=(const G4SDManager&);

};




#endif

