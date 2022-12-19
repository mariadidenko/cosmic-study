
#ifndef MathSimSteppingAction_h
#define MathSimSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include <vector>

class MathSimEventAction;
class MathSimDetectorConstruction;
class MathSimRunAction;

/// Stepping action class.
///

class MathSimSteppingAction : public G4UserSteppingAction
{
public:
  MathSimSteppingAction(MathSimDetectorConstruction* detConstruction,
                        MathSimEventAction* eventAction, MathSimRunAction* runAction);
  virtual ~MathSimSteppingAction();

  virtual void UserSteppingAction(const G4Step* step);
  virtual void MultipleScattering(const G4Step* step);
  virtual void MaterialMap(const G4Step* step, G4double stepLength);   
    
private:
  MathSimDetectorConstruction* fDetConstruction;
  MathSimEventAction*  fEventAction;
  MathSimRunAction*  fRunAction;
  G4int  count = 0;
  G4double sum_teta = 0.0;
    
  G4Material* matPreStep;
  G4Material* matPostStep;
  G4StepPoint* prePoint;
  G4StepPoint* postPoint;
  G4double stepL;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
