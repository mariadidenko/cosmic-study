
#ifndef MathSimRunAction_h
#define MathSimRunAction_h 1

#include "MathSimEventAction.hh"
#include "MathSimDetectorConstruction.hh"
#include "MathSimPrimaryGeneratorAction.hh"
#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class MathSimEventAction;
class MathSimDetectorConstruction;
class MathSimPrimaryGeneratorAction;

/// Run action class
///
/// It accumulates statistic and computes dispersion of the energy deposit, 
/// step information and track lengths of particles with use of analysis tools

class MathSimRunAction : public G4UserRunAction
{
  public:
    explicit MathSimRunAction(MathSimEventAction*, MathSimDetectorConstruction*, MathSimPrimaryGeneratorAction*);
    virtual ~MathSimRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
    
    G4double ComputeMscHighland(G4double pathLength);
    
    void sumPathLen(G4double truesl, G4int reset){
        fTotalCount++;
        fStepLen += truesl;
        
        fTotalCount = fTotalCount*reset;
        fStepLen = fStepLen*reset;
    };
    
    G4double GetPathLength(){return fStepLen;}
    G4int GetTotalCount(){return fTotalCount;}
    
    void SumLateralDisplacement (G4double displa)
         {fLDispl += displa; fLDispl2 += displa*displa;}
         
    void SumPsi (G4double psi)
         {fPsiSpa += psi; fPsiSpa2 += psi*psi;}
         
    void SumTetaPlane (G4double teta)
         {fTetPrj += teta; fTetPrj2 += teta*teta;}
                  
    void SumPhiCorrel (G4double correl)
         {fPhiCor += correl; fPhiCor2 += correl*correl;}
    
    void GetCurrentMat(G4Material* matVolume){
        fCurrentMat = matVolume;
    }
    
    
private:
  /// Pointer to the event action to retrieve vectors
  MathSimEventAction *fEventAction;
  MathSimDetectorConstruction *fDetConstruction;
  MathSimPrimaryGeneratorAction* fPrimary;
    
//  G4int    fTotalCount;
//  G4double fTruePL, fTruePL2;
  G4double fStepLen;
  G4int    fTotalCount;
  G4double fGeomPL, fGeomPL2;
  G4double fLDispl, fLDispl2;
  G4double fPsiSpa, fPsiSpa2;
  G4double fTetPrj, fTetPrj2;
  G4double fPhiCor, fPhiCor2;
    
  G4Material* fCurrentMat;
    
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

