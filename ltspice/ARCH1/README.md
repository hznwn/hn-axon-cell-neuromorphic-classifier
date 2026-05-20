ARCH1_SIMULATION_ONLY
=====================

Description
-----------
This LTspice file contains a functional simulation of the preliminary voltage-driven integrate-and-fire neuron developed during the early stages of this thesis project.

The circuit served as the first proof-of-concept implementation and was used to verify that a discrete electronic circuit could reproduce the essential behaviour of a biological spiking neuron.

Important Note
--------------
This simulation file is intended for functional validation only and does NOT necessarily represent the exact component values used in the final experimental implementation.

Several component values were adjusted to obtain stable and visually clear integrate-and-fire behaviour in LTspice. The simulation should therefore be interpreted as a conceptual demonstration rather than an exact replication of the physical prototype.

The corresponding file ARCH1_SCHEMATIC_ONLY contains a clean schematic of the architecture and includes both the fixed resistive input and the optional LDR-based voltage divider used in early experiments.

Purpose of the Simulation
-------------------------
The simulation was created to verify the following sequence of operations:

1. The membrane capacitor is charged through a resistive input stage.
2. The membrane voltage (Vmem) increases over time.
3. A Schmitt trigger detects when the threshold is reached.
4. A second inverter restores the correct signal polarity.
5. An NMOS transistor rapidly discharges the membrane capacitor.
6. The cycle repeats, producing integrate-and-fire oscillations.

This behaviour reproduces the fundamental principles of the leaky integrate-and-fire neuron model.

Architecture Overview
---------------------
The preliminary architecture consists of:

- Input resistor or LDR-based voltage divider
- Membrane capacitor (Cmem)
- Two SN74AHC14 Schmitt-trigger inverter stages
- BS170 NMOS reset transistor
- Supporting resistors

The same threshold and reset subsystem was later reused in the HN Axon Cell, where the resistive input stage was replaced by a PMOS current mirror.

Simulation Files
----------------
ARCH1_SCHEMATIC_ONLY.asc
    Clean schematic of the preliminary voltage-driven architecture.

ARCH1_SIMULATION_ONLY.asc
    Functional LTspice simulation used to demonstrate integrate-and-fire behaviour.

README.txt
    This document.

Models Used
-----------
- SN74AHC14.cir
- BS170 model defined directly in the schematic

Interpretation of Results
-------------------------
The most important output is the membrane voltage Vmem.

A successful simulation produces a sawtooth waveform showing:

- gradual charging of the membrane capacitor,
- threshold crossing,
- rapid reset,
- repeated oscillation.

The output of the Schmitt trigger stages appears as a digital spike train.

Relationship to the HN Axon Cell
--------------------------------
This preliminary architecture was used to validate the fundamental integrate-and-fire mechanism and to perform early experiments with push-button and photoresistor inputs.

After successful testing, the resistive input stage was replaced by a PMOS current mirror, resulting in the HN Axon Cell (HNAC), the final neuron architecture presented in this thesis.

Author
------
Haze Newman
Bachelor's Thesis in Electrical Engineering
Uppsala University
2026