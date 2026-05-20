# HN Axon Cell Neuromorphic Classifier

Discrete hardware implementation of the HN Axon Cell, a biologically inspired integrate-and-fire neuron for real-time light-direction classification using a 3×3 LDR sensor array, current mirrors, LTspice simulations, Arduino code, CAD models, and experimental data.

---

## Overview

This repository contains all source files, simulation models, experimental data, CAD designs, supplementary materials, and documentation associated with the bachelor's thesis:

**Implementation and Characterisation of a Discrete Neuromorphic Hardware System for Real-Time Light-Direction Classification**

The project investigates how core principles of biological neurons can be implemented using discrete electronic components. The resulting system, referred to as the **HN Axon Cell (HNAC)**, performs temporal integration, threshold detection, spike generation, and membrane reset to produce spike-frequency encoded outputs.

Nine HN Axon Cells were integrated with a curved 3×3 array of light-dependent resistors (LDRs) and an Arduino Uno R4 WiFi to create a complete neuromorphic system capable of real-time light-direction classification.

The prototype includes:

- 9 discrete integrate-and-fire neurons
- 9 light-dependent resistors (LDRs) arranged in a 3×3 sensor array
- CD4051B analogue multiplexer
- Arduino Uno R4 WiFi
- Automatic self-calibration and supervised template learning
- Real-time direction classification
- USB-C power consumption below 1 W

---

## Key Features

- Discrete implementation of an integrate-and-fire neuron
- Current mirror-based input stage
- Spike-frequency encoding of analogue sensor signals
- Real-time classification of light direction using a 3×3 LDR array
- Arduino-based template matching and LED matrix visualization
- LTspice simulation models
- Oscilloscope screenshots and experimental data
- 3D-printable sensor enclosure
- Full thesis documentation and supplementary materials

---

## Biological Inspiration

The HN Axon Cell is based on the leaky integrate-and-fire (LIF) neuron model.

| Biological Concept      | Hardware Implementation         |
|------------------------|--------------------------------|
| Membrane               | Capacitor                      |
| Synaptic input current | PMOS current mirror            |
| Threshold detection    | SN74AHC14 Schmitt trigger      |
| Spike and reset        | BS170 MOSFET                   |
| Refractory shaping     | Optional feedback capacitor    |

---

## Repository Structure

```text
hn-axon-cell-neuromorphic-classifier/
├── arduino/
│   └── hn_axon_cell_classifier.ino
├── ltspice/
│   ├── preliminary_architectures/
│   ├── architecture_1/
│   └── hn_axon_cell/
├── cad/
│   └── ldr_sensor_array_3x3_curved.f3d
├── data/
│   ├── raw_measurements/
│   ├── calibration_templates/
│   ├── processed_results/
│   └── FINALRESULTS.csv
├── figures/
│   ├── oscilloscope/
│   ├── circuit_photos/
│   └── classification_demo/
├── docs/
│   ├── laboratory_notes/
│   │   └── HN_Axon_Cell_Laboratory_Development_Notes.pdf
│   ├── supplementary_material/
│   └── media/
├── thesis/
│   └── Thesis.pdf
├── LICENSE
└── README.md


⸻

Hardware Components
Neuron Circuit
	•	SN74AHC14 Schmitt trigger inverter
	•	BS170 NMOS transistor
	•	BS250 PMOS transistors
	•	Capacitors and resistors
	•	Light-dependent resistors (LDRs)
System Components
	•	Arduino Uno R4 WiFi
	•	CD4051B analogue multiplexer
	•	3×3 curved LDR sensor array
	•	USB 5 V power supply

⸻

Experimental Results
The HN Axon Cell exhibited:
	•	Stable and repeatable integrate-and-fire oscillations
	•	Firing frequency inversely proportional to membrane capacitance
	•	Stable threshold voltages across parameter sweeps
	•	Successful conversion of light intensity into spike frequency
	•	Reliable real-time classification of flashlight position
The completed system consistently identified the illuminated sensor position and displayed the detected direction on the Arduino LED matrix.

⸻

Supplementary Materials

The repository includes extensive supplementary material documenting the complete system and experimental results.
Supplementary Videos
Supplementary_Video_S1_Complete_System_Startup_Self_Training_and_Real_Time_3x3_Neuromorphic_Classification.MOV

Complete end-to-end demonstration showing:
	•	Program startup
	•	Ambient baseline calibration
	•	Self-training on all nine sensor positions
	•	Transition to live mode
	•	Real-time direction classification
Supplementary_Video_S4_Random_Order_3x3_Neuromorphic_Classification_Accuracy_Demonstration.MOV
Demonstrates classification of all nine sensor positions in random order. All positions were classified correctly during the recorded test.

⸻

Representative Neuron Oscilloscope Captures:
Oscilloscope_20260513_004451_Neuron7_IntegrateAndFire_Covered_LDR_Input_USB_C_Powered_5V.jpeg
Neuron 7 response with the corresponding LDR covered (minimal illumination).
Oscilloscope_20260513_004514_Neuron7_IntegrateAndFire_Ambient_Light_LDR_Input_USB_C_Powered_5V.jpeg
Neuron 7 response under ambient room lighting.
Oscilloscope_20260513_004541_Neuron7_IntegrateAndFire_Direct_Flashlight_LDR_Input_USB_C_Powered_5V.jpeg
Neuron 7 response under direct flashlight illumination.
These three captures demonstrate spike-frequency encoding, where the firing rate increases with increasing light intensity.

⸻

System Figures:
3x3_LDR_Sensor_Array_and_Neuromorphic_Neuron_Mapping.png
Physical layout and channel mapping of the 3×3 LDR sensor array.
Self_Training_3x3_Neuromorphic_Classification_System_Idle_Four_Corner_LEDs_USB_C_Powered_5V.png
Idle state of the classifier when no significant light stimulus is detected.
Self_Training_3x3_Neuromorphic_Classification_System_Active_Direction_Detection_USB_C_Powered_5V.png
Example of active real-time direction classification.
System_Power_Measurement_5p158V_0p154A_0p789W_USB_C_Powered.png
USB power measurement of the complete prototype.

⸻

Hardware Photographs:
	•	Neuron1-4.HEIC Photograph of neurons 1 through 4.
	•	Neuron1-4_with_3dprint_plus_LDRs3x3.HEIC Neurons 1 through 4 together with the 3D-printed sensor array.
	•	Neuron4_close_up.HEIC Close-up photograph of a representative neuron.
	•	Neuron5-9_plus_meter_plus_arduino_plus_multiplexer.HEIC Neurons 5 through 9 together with the Arduino, multiplexer, and power meter.
	•	Neuron5-9_plus_meter_plus_arduino_plus_multiplexer_plus_back_3x3LDR_3dprint.HEIC Comprehensive overview of the complete hardware prototype.
	•	Neuron7-9.HEIC Photograph of neurons 7 through 9.

⸻

Data Files:
FINALRESULTS.csv
Final validated dataset collected on May 13, 2026.
Contains sensor readings and classification outputs from the completed self-training 3×3 neuromorphic classification system after all hardware and software optimizations were finalized.

⸻

Power Consumption:
Measured using a KWS-2302C USB power meter.
Parameter
Value
Voltage
5.158 V
Current
0.154 A
Power
0.789 W
Estimated meter overhead: approximately 0.01 to 0.05 W.
Estimated actual system power consumption: approximately 0.74 to 0.78 W.
These measurements confirm that the complete neuromorphic classification system operates well below 1 W.

⸻

System Notes
	•	The system performs self-calibration and retraining at every startup.
	•	The classifier adapts to different ambient lighting conditions.
	•	Sensors N1 to N8 are read through the CD4051B analogue multiplexer.
	•	Sensor N9 is connected directly to Arduino analogue input A1.
	•	The system is powered entirely from a standard 5 V USB-C connection.

⸻

Thesis Abstract
This work demonstrates how biologically inspired neural computation can be implemented using discrete electronic components. The developed HN Axon Cell reproduces the core behaviour of a spiking neuron and forms the basis of a nine-neuron neuromorphic hardware system capable of real-time light-direction classification.
The project bridges theoretical neuroscience, analogue circuit design, simulation, embedded programming, and experimental validation.

⸻

Software and Tools Used
	•	LTspice
	•	Arduino IDE
	•	Microsoft Word
	•	Fusion 360
	•	Rigol DS1202 Oscilloscope

⸻

License
This project is licensed under the MIT License. See the LICENSE file for details.

⸻

Author
Haze Newman Bachelor’s Thesis in Electrical Engineering Uppsala University 2026

⸻

Citation
If you use this work in research or development, please cite the associated bachelor’s thesis and reference this repository.

⸻

Contact
For questions, collaboration, or future research opportunities, please open an issue on GitHub.

