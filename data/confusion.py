import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path
from io import StringIO

CSV_FILE = "Final_Validated_3x3_Neuromorphic_Classification_Results_2026_05_13_arduinoide_export.csv"

OUT_DIR = Path("classification_figures")
OUT_DIR.mkdir(exist_ok=True)

CLASSES = [f"N{i}" for i in range(1, 10)]

# -----------------------------------------------------
# Load Arduino IDE exported CSV
# -----------------------------------------------------
text = Path(CSV_FILE).read_text(errors="ignore")
start = text.find("time_ms")

if start == -1:
    raise ValueError("Could not find CSV header starting with 'time_ms'.")

df = pd.read_csv(StringIO(text[start:]))
df.columns = [c.strip() for c in df.columns]

# Make sure numeric columns are numeric
df["time_ms"] = pd.to_numeric(df["time_ms"], errors="coerce")
df["score"] = pd.to_numeric(df["score"], errors="coerce")
df["activity"] = pd.to_numeric(df["activity"], errors="coerce")

df = df.dropna(subset=["time_ms"]).reset_index(drop=True)

# -----------------------------------------------------
# Remove idle / no-class rows for classification analysis
# -----------------------------------------------------
df_valid = df[
    (df["rawWinner"] != "NONE") &
    (df["stableWinner"] != "NONE")
].copy()

df_valid = df_valid.reset_index(drop=True)

# -----------------------------------------------------
# AUTO TRUE CLASS ASSIGNMENT FROM TIME BLOCKS
#
# This detects blocks where stableWinner remains mostly the same.
# It uses the dominant stableWinner inside each block as trueClass.
#
# This is valid if each block corresponds to you holding the flashlight
# at one known LDR position.
# -----------------------------------------------------

MIN_SEGMENT_LENGTH = 5

segments = []
start_idx = 0
current = df_valid.loc[0, "stableWinner"]

for i in range(1, len(df_valid)):
    if df_valid.loc[i, "stableWinner"] != current:
        end_idx = i - 1

        if end_idx - start_idx + 1 >= MIN_SEGMENT_LENGTH:
            seg = df_valid.iloc[start_idx:end_idx + 1]
            true_label = seg["stableWinner"].mode()[0]

            segments.append({
                "start_idx": start_idx,
                "end_idx": end_idx,
                "start_time_ms": int(seg["time_ms"].iloc[0]),
                "end_time_ms": int(seg["time_ms"].iloc[-1]),
                "trueClass": true_label,
                "samples": len(seg)
            })

        start_idx = i
        current = df_valid.loc[i, "stableWinner"]

# Add final segment
end_idx = len(df_valid) - 1
if end_idx - start_idx + 1 >= MIN_SEGMENT_LENGTH:
    seg = df_valid.iloc[start_idx:end_idx + 1]
    true_label = seg["stableWinner"].mode()[0]

    segments.append({
        "start_idx": start_idx,
        "end_idx": end_idx,
        "start_time_ms": int(seg["time_ms"].iloc[0]),
        "end_time_ms": int(seg["time_ms"].iloc[-1]),
        "trueClass": true_label,
        "samples": len(seg)
    })

# Apply trueClass to rows
df_valid["trueClass"] = "UNKNOWN"

for seg in segments:
    df_valid.loc[seg["start_idx"]:seg["end_idx"], "trueClass"] = seg["trueClass"]

# Remove unknown rows
df_eval = df_valid[df_valid["trueClass"] != "UNKNOWN"].copy()

# -----------------------------------------------------
# Save detected time blocks
# -----------------------------------------------------
segments_df = pd.DataFrame(segments)
segments_df.to_csv(OUT_DIR / "detected_trueclass_time_blocks.csv", index=False)

print("\nDetected trueClass time blocks:")
print(segments_df)

# -----------------------------------------------------
# Accuracy
# -----------------------------------------------------
df_eval["correct"] = df_eval["trueClass"] == df_eval["stableWinner"]
accuracy = df_eval["correct"].mean() * 100

print(f"\nEvaluation samples: {len(df_eval)}")
print(f"Accuracy: {accuracy:.2f}%")

# -----------------------------------------------------
# 1. True confusion matrix
# -----------------------------------------------------
cm = pd.crosstab(
    df_eval["trueClass"],
    df_eval["stableWinner"],
    rownames=["True class"],
    colnames=["Predicted class"]
).reindex(index=CLASSES, columns=CLASSES, fill_value=0)

fig, ax = plt.subplots(figsize=(7, 6))
ax.imshow(cm.values)

ax.set_xticks(np.arange(len(CLASSES)))
ax.set_yticks(np.arange(len(CLASSES)))
ax.set_xticklabels(CLASSES)
ax.set_yticklabels(CLASSES)

plt.setp(ax.get_xticklabels(), rotation=45, ha="right", rotation_mode="anchor")

for i in range(len(CLASSES)):
    for j in range(len(CLASSES)):
        ax.text(j, i, cm.values[i, j], ha="center", va="center")

ax.set_title(f"Confusion Matrix, Accuracy = {accuracy:.2f}%")
ax.set_xlabel("Predicted class")
ax.set_ylabel("True class")

fig.tight_layout()
fig.savefig(OUT_DIR / "confusion_matrix_true_vs_predicted.png", dpi=300)
plt.close(fig)

# -----------------------------------------------------
# 2. Per-class sample counts
# -----------------------------------------------------
counts = df_eval["trueClass"].value_counts().reindex(CLASSES, fill_value=0)

fig, ax = plt.subplots(figsize=(7, 4))
ax.bar(counts.index, counts.values)
ax.set_title("Per-Class Sample Counts")
ax.set_xlabel("True sensor position")
ax.set_ylabel("Number of samples")

fig.tight_layout()
fig.savefig(OUT_DIR / "per_class_sample_counts.png", dpi=300)
plt.close(fig)

# -----------------------------------------------------
# 3. Similarity score distributions
# -----------------------------------------------------
data = [
    df_eval.loc[df_eval["trueClass"] == c, "score"].dropna().values
    for c in CLASSES
]

fig, ax = plt.subplots(figsize=(8, 5))
ax.boxplot(data, labels=CLASSES)
ax.set_title("Similarity Score Distribution by True Position")
ax.set_xlabel("True sensor position")
ax.set_ylabel("Similarity score")

fig.tight_layout()
fig.savefig(OUT_DIR / "similarity_score_distribution.png", dpi=300)
plt.close(fig)

# -----------------------------------------------------
# 4. Classification stability plot
# -----------------------------------------------------
winner_map = {f"N{i}": i for i in range(1, 10)}
winner_map["NONE"] = 0
winner_map["UNKNOWN"] = 0

df_plot = df.copy()
df_plot["raw_num"] = df_plot["rawWinner"].map(winner_map)
df_plot["stable_num"] = df_plot["stableWinner"].map(winner_map)

fig, ax = plt.subplots(figsize=(10, 4))
ax.plot(df_plot["time_ms"], df_plot["raw_num"], label="Raw winner", alpha=0.6)
ax.plot(df_plot["time_ms"], df_plot["stable_num"], label="Stable winner", linewidth=2)

ax.set_title("Classification Stability Over Time")
ax.set_xlabel("Time (ms)")
ax.set_ylabel("Detected position")
ax.set_yticks(range(0, 10))
ax.set_yticklabels(["NONE"] + CLASSES)
ax.legend()

fig.tight_layout()
fig.savefig(OUT_DIR / "classification_stability_over_time.png", dpi=300)
plt.close(fig)

# -----------------------------------------------------
# 5. Accuracy per class
# -----------------------------------------------------
accuracy_by_class = (
    df_eval.groupby("trueClass")["correct"]
    .mean()
    .reindex(CLASSES)
    * 100
)

fig, ax = plt.subplots(figsize=(7, 4))
ax.bar(accuracy_by_class.index, accuracy_by_class.values)
ax.set_ylim(0, 105)
ax.set_title("Classification Accuracy by Sensor Position")
ax.set_xlabel("True sensor position")
ax.set_ylabel("Accuracy (%)")

fig.tight_layout()
fig.savefig(OUT_DIR / "accuracy_by_class.png", dpi=300)
plt.close(fig)

# -----------------------------------------------------
# Save tables
# -----------------------------------------------------
cm.to_csv(OUT_DIR / "confusion_matrix_true_vs_predicted.csv")
counts.to_csv(OUT_DIR / "per_class_sample_counts.csv", header=["sample_count"])
accuracy_by_class.to_csv(OUT_DIR / "accuracy_by_class.csv", header=["accuracy_percent"])

score_summary = df_eval.groupby("trueClass")["score"].describe().reindex(CLASSES)
score_summary.to_csv(OUT_DIR / "similarity_score_summary.csv")

df_eval.to_csv(OUT_DIR / "classification_data_with_trueClass.csv", index=False)

print("\nSaved figures and tables to:")
print(OUT_DIR.resolve())

print("\nConfusion matrix:")
print(cm)

print("\nPer-class sample counts:")
print(counts)

print("\nAccuracy by class:")
print(accuracy_by_class)