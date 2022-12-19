
#ifndef MathSimScintillatorSD_h
#define MathSimScintillatorSD_h 1

#include "G4VSensitiveDetector.hh"

#include "MathSimScintillatorHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

/// Scintillator layer --  sensitive detector

class MathSimScintillatorSD : public G4VSensitiveDetector
{
  public:
    MathSimScintillatorSD(G4String name);
    virtual ~MathSimScintillatorSD();
    
    virtual void Initialize(G4HCofThisEvent*HCE);
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
    
  private:
    MathSimScintillatorHitsCollection* fHitsCollection;
    G4int fHCID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
