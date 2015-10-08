#ifndef B1Run_h
#define B1Run_h 1

#include "G4Run.hh"
#include "globals.hh"

class G4Event;

class B1Run : public G4Run
{
   private:
      G4int     fRunNumber;
      G4double  fEdep;
      G4double  fEdep2;

   public:
      B1Run(G4int rn = 0);
      virtual ~B1Run();

      virtual void Merge(const G4Run*);
      virtual void RecordEvent(const G4Event*);

      void AddEdep (G4double edep); 

      // get methods
      G4double GetEdep()  const { return fEdep; }
      G4double GetEdep2() const { return fEdep2; }

};


#endif

