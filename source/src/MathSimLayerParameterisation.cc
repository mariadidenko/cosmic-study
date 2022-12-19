#include "MathSimLayerParameterisation.hh"
#include "MathSimConstants.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimLayerParameterisation::MathSimLayerParameterisation()
: G4VPVParameterisation()
{

   for (auto i=0;i<nofLayers_B0;i++) {
    G4double z1 = layerThickness*0.5 + i*(gapThickness+layerThickness) - moduleLength/2;

    fZLayers[i] = z1;
  }

   G4double final_pos_0 = layerThickness*0.5 + (nofLayers_B0-1)*(gapThickness+layerThickness) - moduleLength/2;

  for (auto i=0;i<nofLayers_B1;i++) {
    G4double z1 = final_pos_0 + gap01Thickness + layerThickness + (i)*(gapThickness+layerThickness);

    fZLayers[i+nofLayers_B0] = z1;
  }

  final_pos_0 = final_pos_0 + gap01Thickness + (nofLayers_B1-1)*(gapThickness+layerThickness) + layerThickness;
  
  for (auto i=0;i<nofLayers_B2;i++) {
    G4double z1 = final_pos_0 + gap12Thickness + (i)*(gapThickness_trigger+layerThickness) + layerThickness;
    
    fZLayers[i+nofLayers_B0+nofLayers_B1] = z1;
  }

  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MathSimLayerParameterisation::~MathSimLayerParameterisation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MathSimLayerParameterisation::ComputeTransformation(
       const G4int copyNo,G4VPhysicalVolume *physVol) const
{
  physVol->SetTranslation(G4ThreeVector(0.,0., fZLayers[copyNo] ));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
