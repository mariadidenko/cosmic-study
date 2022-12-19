#ifndef MathSimConstants_h
#define MathSimConstants_h 1

#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

constexpr G4int nofLayers_B0 = 2;
constexpr G4int	nofLayers_B1 = 2;
constexpr G4int	nofLayers_B2 = 6;

constexpr  G4double SCLayerThickness = 2.*cm; // scintillator layer thickness
constexpr  G4double SupLayerThickness = 2.*cm; // supporting layer thickness
constexpr  G4double layerThickness = SCLayerThickness+SupLayerThickness; // total layer thickness
constexpr  G4double gapThickness =  1*m - SCLayerThickness - SupLayerThickness; // between layers
constexpr  G4double gapThickness_trigger =  80*cm - SCLayerThickness - SupLayerThickness; // between layers

constexpr  G4double gap01Thickness = 20*m - 2*layerThickness - gapThickness;// 20m
constexpr  G4double gap12Thickness = 5*m - layerThickness;

constexpr  G4double barThickness = 30.*cm;
constexpr  G4double moduleSizeXY  = 100*m;

constexpr G4int kNofChambers = nofLayers_B0+nofLayers_B1+nofLayers_B2;
constexpr G4int nofBars = 4;

//constexpr  G4double moduleLength = (kNofChambers-3)*gapThickness+kNofChambers*layerThickness+gap01Thickness+gap12Thickness;
constexpr  G4double moduleLength = (nofLayers_B0+nofLayers_B1-2)*gapThickness + (nofLayers_B2 - 1 )*gapThickness_trigger + kNofChambers*layerThickness+gap01Thickness+gap12Thickness;
constexpr  G4double rockLength = 100*m;
constexpr  G4double rockDistance = 0*m;

#endif
