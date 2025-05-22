import os
import librosa
import soundfile as sf
from tqdm import tqdm
import random

SOURCE_DIRS = {
    "songs": "audio/songs",
    "melody": "audio/melody",
    "voice": "audio/voice"
}

OUTPUT_DIR = "dataset"
SAMPLE_RATE = 24000
FRAGMENT_DURATION = 1.0 
TRAIN_RATIO = 0.8

def ensure_dir(path):
    os.makedirs(path, exist_ok=True)

def slice_audio(file_path, label):
    y, sr = librosa.load(file_path, sr=SAMPLE_RATE, mono=True)
    total_samples = len(y)
    fragment_samples = int(SAMPLE_RATE * FRAGMENT_DURATION)
    fragments = []

    for start in range(0, total_samples - fragment_samples + 1, fragment_samples):
        fragment = y[start:start + fragment_samples]
        if len(fragment) == fragment_samples:
            fragments.append(fragment)

    return fragments

def save_fragments(fragments, label):
    random.shuffle(fragments)
    split_index = int(len(fragments) * TRAIN_RATIO)
    train_fragments = fragments[:split_index]
    test_fragments = fragments[split_index:]

    for subset, data in zip(['train', 'test'], [train_fragments, test_fragments]):
        out_dir = os.path.join(OUTPUT_DIR, subset, label)
        ensure_dir(out_dir)
        for i, frag in enumerate(data):
            file_path = os.path.join(out_dir, f"{label}_{i:05d}.wav")
            sf.write(file_path, frag, SAMPLE_RATE)

def main():
    print("Начинаем обработку аудио...")

    for label, folder in SOURCE_DIRS.items():
        print(f"\n▶ Обрабатываем категорию: {label}")
        fragments = []
        files = [os.path.join(folder, f) for f in os.listdir(folder) if f.lower().endswith(('.wav', '.mp3', '.flac'))]

        for file_path in tqdm(files, desc=f"  Файлы в {label}"):
            file_fragments = slice_audio(file_path, label)
            fragments.extend(file_fragments)

        print(f"  → Найдено {len(fragments)} фрагментов для {label}")
        save_fragments(fragments, label)

    print("\n✅ Нарезка и разбиение завершены!")

if __name__ == "__main__":
    main()
