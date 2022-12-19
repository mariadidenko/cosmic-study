#include "MathSimActionInitialization.hh"
#include "MathSimPrimaryGeneratorAction.hh"
#include "MathSimRunAction.hh"
#include "MathSimEventAction.hh"
#include "MathSimSteppingAction.hh"
#include "MathSimDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimActionInitialization::MathSimActionInitialization
                            (MathSimDetectorConstruction* detConstruction)
 : G4VUserActionInitialization(),
   fDetConstruction(detConstruction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimActionInitialization::~MathSimActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MathSimActionInitialization::BuildForMaster() const
{
  MathSimEventAction *eventAction = new MathSimEventAction();
  MathSimPrimaryGeneratorAction *primaryAction = new MathSimPrimaryGeneratorAction(eventAction);
  SetUserAction(new MathSimRunAction(eventAction, fDetConstruction, primaryAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MathSimActionInitialization::Build() const
{
  MathSimEventAction *eventAction = new MathSimEventAction();
  MathSimPrimaryGeneratorAction *primaryAction = new MathSimPrimaryGeneratorAction(eventAction);
  SetUserAction(primaryAction);

  SetUserAction(eventAction);

  MathSimRunAction *runAction = new MathSimRunAction(eventAction, fDetConstruction, primaryAction);
  SetUserAction(runAction);

  SetUserAction(new MathSimSteppingAction(fDetConstruction,eventAction,runAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
