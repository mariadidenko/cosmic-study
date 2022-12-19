#ifndef MathSimActionInitialization_h
#define MathSimActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class MathSimDetectorConstruction;

/// Action initialization class.
///

class MathSimActionInitialization : public G4VUserActionInitialization
{
  public:
    MathSimActionInitialization(MathSimDetectorConstruction*);
    virtual ~MathSimActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    MathSimDetectorConstruction* fDetConstruction;
};

#endif

    
