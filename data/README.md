README.txt
==========

3×3 Neuromorphic Light-Direction Classification Data and Analysis
=================================================================

This folder contains the raw Arduino IDE serial export and the Python script
used to generate the supplementary classification figures and tables presented
in the appendix of the thesis.

Raw Data File
-------------

Final_Validated_3x3_Neuromorphic_Classification_Results_2026_05_13_arduinoide_export.csv

This file is the original serial data export from the Arduino IDE. It contains:

- Timestamp (time_ms)
- Raw sensor readings (N1–N9)
- Activity metric
- Raw classifier output (rawWinner)
- Stable classifier output (stableWinner)
- Similarity score

The file includes both calibration messages and live classification data.

Python Analysis Script
----------------------

confusion.py

Running this script automatically:

1. Extracts the tabular data from the Arduino IDE export.
2. Detects classification segments using the stable classifier output.
3. Assigns a trueClass label to each segment.
4. Computes classification accuracy.
5. Generates confusion matrices, plots, and summary tables.

Generated Output Files
----------------------

The following files are created in:

generated_classification_figures/

CSV Files
---------

generated_classification_figures/per_class_sample_counts.csv
generated_classification_figures/confusion_matrix_true_vs_predicted.csv
generated_classification_figures/accuracy_by_class.csv
generated_classification_figures/classification_data_with_trueClass.csv
generated_classification_figures/detected_trueclass_time_blocks.csv
generated_classification_figures/similarity_score_summary.csv

Image Files
-----------

generated_classification_figures/classification_stability_over_time.png
generated_classification_figures/confusion_matrix_true_vs_predicted.png
generated_classification_figures/similarity_score_distribution.png
generated_classification_figures/per_class_sample_counts.png
generated_classification_figures/accuracy_by_class.png

Summary of Results
------------------

The analysis produced the following key results:

- 392 valid classification samples
- 100.00% overall classification accuracy
- 100.00% accuracy for all nine classes (N1–N9)
- Zero misclassifications
- Perfect diagonal confusion matrix

These results confirm that the neuromorphic light-direction classification
system operated reliably under the controlled experimental conditions used
in this study.

Requirements
------------

The Python script requires the following packages:

- pandas
- numpy
- matplotlib

Install them using:

pip install pandas numpy matplotlib

Usage
-----

Place the following files in the same directory:

- confusion.py
- Final_Validated_3x3_Neuromorphic_Classification_Results_2026_05_13_arduinoide_export.csv

Run:

python confusion.py

A new folder named generated_classification_figures will be created containing
all generated figures and summary tables.

Author
------

Haza Newman
Uppsala University
Degree Project in Electrical Engineering
2026