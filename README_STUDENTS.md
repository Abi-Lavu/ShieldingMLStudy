# Aerospace Shielding Sim -- Student Edition

A Geant4 simulation of a proton beam hitting a shield material, then depositing
dose in a block of soft tissue behind it. You're testing: *for a given
material and thickness, how much radiation dose reaches the tissue?*

## Running a sweep (no rebuild needed for this part)

1. Edit `sweep_config.txt` (in the same folder as the `sim` executable, or
   pass a path as an argument: `./sim my_config.txt`).
2. Run `./sim`.
3. Open the CSV file it writes (`output_csv` in the config) in Excel, Python,
   whatever -- one row per (material, thickness, proton energy) combination.

You can change materials, thickness range, energy range, and event count in
`sweep_config.txt` and just rerun -- the C++ program itself doesn't change.

## Testing a new material

Composition has to be compiled in (it's baked into the physics setup), so
this is the one step that needs a rebuild. Material names are **not** typed
in by hand -- they're generated automatically from the composition, so a
material can never end up mislabeled relative to what it's actually made of.

1. Open `MaterialLibrary.h`.
2. Add a row to the array at the top with the material's density (g/cm3) and
   elemental composition as **mass percentages** of Carbon, Hydrogen, Oxygen,
   Nitrogen, and Silicon. They don't need to add to exactly 100 -- the code
   renormalizes -- but they should be close, or your density won't
   correspond to the composition you think it does.
3. Rebuild (`make -j4` from your `build/` folder).
4. Run `./sim --list-materials` to see the name the code generated for your
   material -- something like `C60H40`.
5. Put that generated name in `materials =` in `sweep_config.txt` and rerun
   `./sim` -- no further rebuilding needed.

Example: adding a hypothetical 60% carbon / 40% hydrogen polymer at 1.05 g/cm3:

```cpp
{1.05, 60.0, 40.0, 0.0, 0.0, 0.0},
```

`./sim --list-materials` would then show it as `C60H40`, and that's what
goes in `sweep_config.txt`.

## What's in the output CSV

| column | meaning |
|---|---|
| `shield_material` | material name from the sweep |
| `shield_thickness_cm` | shield thickness for this row |
| `proton_energy_MeV` | beam energy for this row |
| `number_of_primary_protons` | how many protons were simulated |
| `organ_dose_per_proton_Gy` | **the number you care about** -- mean tissue dose per incident proton |
| `organ_energy_deposited_total_MeV` | total energy deposited in tissue across the whole run |
| `organ_mass_kg` | mass of the tissue block |
| `scored_organ_events` | how many of the simulated protons actually deposited measurable energy in the tissue |

There's no separate skin layer in this version -- the detector is a single
uniform block of soft tissue (`G4_TISSUE_SOFT_ICRP`), so the dose numbers are
whole-organ, not skin-vs-organ split.

## Files you'll actually touch

- `sweep_config.txt` -- sweep parameters, edit freely, no rebuild.
- `MaterialLibrary.h` -- material compositions, edit + rebuild to add a material.

## Files you probably won't need to touch

`MaterialLibrary.cpp` (turns composition into a name and a G4Material),
`construction.cpp/.h` (geometry), `generator.cpp/.h` (beam), `RunAction.cpp/.h`
(dose bookkeeping), `TissueSensitiveDetector.cpp/.h` (scoring), `action.cpp/.h`
(wiring), `SweepConfig.cpp/.h` (config file parser), `sim.cpp` (main/sweep loop).
