import librosa
import librosa.display
import matplotlib.pyplot as plt
import numpy as np

y1, sr1 = librosa.load("s4.wav", sr=None) # main file
y2, sr2 = librosa.load("s4_o.wav", sr=None) # compress file

min_len = min(len(y1), len(y2))
y1 = y1[:min_len]
y2 = y2[:min_len]

diff = y1 - y2

plt.figure(figsize=(14, 8))

plt.subplot(3, 1, 1)
librosa.display.waveshow(y1, sr=sr1, alpha=0.6)
plt.title("Сигнал 1")

plt.subplot(3, 1, 2)
librosa.display.waveshow(y2, sr=sr2, alpha=0.6, color='orange')
plt.title("Сигнал 2")

plt.subplot(3, 1, 3)
plt.plot(diff, color='red')
plt.title("Разница между сигналами")
plt.tight_layout()

plt.show()
