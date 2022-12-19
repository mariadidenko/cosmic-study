#ifndef MathSimLayerParameterisation_H
#define MathSimLayerParameterisation_H 1

#include "MathSimConstants.hh"

#include "globals.hh"
#include "G4VPVParameterisation.hh"

#include <array>

class G4VPhysicalVolume;

///  Mathusla module layer parameterisation

class MathSimLayerParameterisation : public G4VPVParameterisation
{
  public:
    MathSimLayerParameterisation();
    virtual ~MathSimLayerParameterisation();
    
    virtual void ComputeTransformation(
                   const G4int copyNo,G4VPhysicalVolume *physVol) const;
    
  private:
    std::array<G4double, kNofChambers> fZLayers; 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
