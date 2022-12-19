#ifndef MathSimPrimaryGeneratorAction_h
#define MathSimPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "MathSimEventAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "/Users/mdidenko/Geant4/cry_v1.7/src/CRYSetup.h"
#include "/Users/mdidenko/Geant4/cry_v1.7/src/CRYGenerator.h"
#include "/Users/mdidenko/Geant4/cry_v1.7/src/CRYParticle.h"
#include "/Users/mdidenko/Geant4/cry_v1.7/src/CRYUtils.h"
#include "vector"
#include "RNGWrapper.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"


//class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;
class MathSimEventAction;

/// The primary generator action class with particle gum.
///
/// It defines a single particle which hits the calorimeter 
/// perpendicular to the input face. The type of the particle
/// can be changed via the G4 build-in commands of G4ParticleGun class 
/// (see the macros provided with this example).

class MathSimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  MathSimPrimaryGeneratorAction(MathSimEventAction* eventAction);
  virtual ~MathSimPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event* event);
  virtual void InputCRY();
  virtual void UpdateCRY(std::string* MessInput);
  virtual void CRYFromFile(G4String newValue);
  G4ParticleGun* GetParticleGun() {return fParticleGun;};
    
//  G4GeneralParticleSource* GetParticleGun(){return mParticleGun;};
    
  // set methods
  void SetRandomFlag(G4bool value);

private:
  G4ParticleGun*  fParticleGun; // G4 particle gun
  MathSimEventAction*  fEventAction;
  std::vector<CRYParticle*> *vect; // vector of generated particles
  G4ParticleTable* particleTable;
  CRYGenerator* gen;
  G4int InputState;
    
  G4ParticleDefinition *fParticleDefinition;
  G4double fParticleEnergy;
  G4double fParticleMass;
  G4double fParticlePosX;
  G4double fParticlePosY;
  G4double fParticlePosZ;
  G4double fParticleMomentumDirectionU;
  G4double fParticleMomentumDirectionV;
  G4double fParticleMomentumDirectionW;
  G4double fParticleTime;
//  PrimaryGeneratorMessenger* gunMessenger;
//  G4GeneralParticleSource* mParticleGun;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
