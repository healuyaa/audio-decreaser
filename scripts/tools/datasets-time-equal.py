import os
import mutagen
from collections import defaultdict

def get_audio_duration(filepath):
    try:
        audio = mutagen.File(filepath)
        if audio is not None and hasattr(audio, 'info') and hasattr(audio.info, 'length'):
            return audio.info.length
    except Exception as e:
        print(f"Ошибка при чтении {filepath}: {e}")
    return 0

def is_audio_file(filename):
    audio_extensions = ('.mp3', '.wav', '.flac', '.aac', '.ogg', '.m4a')
    return filename.lower().endswith(audio_extensions)

def collect_durations(root_dir):
    durations = defaultdict(float)
    
    for dirpath, _, filenames in os.walk(root_dir):
        rel_path = os.path.relpath(dirpath, root_dir)
        top_level = rel_path.split(os.sep)[0] if rel_path != "." else "."
        
        for file in filenames:
            if is_audio_file(file):
                full_path = os.path.join(dirpath, file)
                duration = get_audio_duration(full_path)
                durations[top_level] += duration
    
    return durations

def format_duration(seconds):
    hours = int(seconds // 3600)
    minutes = int((seconds % 3600) // 60)
    secs = int(seconds % 60)
    return f"{hours:02}:{minutes:02}:{secs:02}"

if __name__ == "__main__":
    root_directory = "./audio"
    durations = collect_durations(root_directory)

    for folder, total_seconds in durations.items():
        print(f"{folder}: {format_duration(total_seconds)}")
