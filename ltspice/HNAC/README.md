HNAC_SIMULATION_ONLY
====================

Description
-----------
This LTspice file contains a functional simulation of the HN Axon Cell (HNAC), a discrete-component neuromorphic neuron developed as part of a Bachelor's thesis in Electrical Engineering at Uppsala University.

The purpose of this simulation is to demonstrate the fundamental integrate-and-fire behaviour of the architecture in a clear and reproducible manner.

Important Note
--------------
This simulation file does NOT represent the exact component values used in the final breadboard implementation.

Several component values were intentionally adjusted to obtain stable and visually clear integrate-and-fire behaviour in LTspice. These modifications were made solely for simulation and demonstration purposes.

The separate file HNAC_SCHEMATIC_ONLY contains the circuit topology and nominal component values corresponding more closely to the physical implementation.

Purpose of the Simulation
-------------------------
The simulation was created to verify the following sequence of operations:

1. Input current charges the membrane capacitor (Cmem).
2. The membrane voltage (Vmem) increases over time.
3. A Schmitt trigger detects when the threshold is reached.
4. A second inverter restores the correct signal polarity.
5. An NMOS transistor rapidly discharges the membrane capacitor.
6. The cycle repeats, producing integrate-and-fire oscillations.

This behaviour reproduces the core computational principles of the leaky integrate-and-fire neuron model.

Simulation-Specific Modifications
---------------------------------
The following adjustments may differ from the final hardware implementation:

- Current mirror bias resistors were tuned to provide sufficient charging current.
- Additional resistors were modified to improve convergence and stability.
- Simulation models were used for:
  - BS250 PMOS transistors
  - BS170 NMOS transistor
  - SN74AHC14 Schmitt trigger inverter
- Initial conditions were specified to ensure proper startup.
- Transient analysis settings were selected to clearly display repetitive firing.

These changes were made to validate the architecture rather than to exactly replicate measured experimental values.

Files Included
--------------
HNAC_SIMULATION_ONLY.asc
    LTspice simulation file with adjusted values for functional validation.

HNAC_SCHEMATIC_ONLY.asc
    Clean schematic showing the architecture and nominal hardware values.

README.txt
    This document.

Models Used
-----------
- BS250.philips.lib
- SN74AHC14.cir
- BS170 model defined directly in the schematic

Interpretation of Results
-------------------------
The most important output is the membrane voltage Vmem.

A successful simulation produces a sawtooth waveform:
- gradual charging of the membrane capacitor,
- threshold crossing,
- rapid reset,
- repeated oscillation.

This confirms correct integrate-and-fire behaviour.

Thesis Context
--------------
This simulation supports the statement that the HN Axon Cell architecture is capable of reproducing biologically inspired spiking neuron dynamics using discrete commercially available electronic components.

The simulation should be interpreted as a functional validation of the design rather than an exact digital twin of the physical breadboard implementation.

Author
------
Haze
Bachelor's Thesis in Electrical Engineering
Uppsala University
2026