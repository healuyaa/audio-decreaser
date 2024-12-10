import os
import sys
import subprocess
from encodec import EncodecModel
from encodec.utils import convert_audio
import torchaudio


def convert_to_24khz(input_file, temp_file):
    """
    Преобразует аудиофайл в формат 24 кГц, стерео с помощью ffmpeg.
    """
    try:
        print(f"Преобразование файла {input_file} в 24 кГц...")
        # Команда для ffmpeg
        command = [
            "ffmpeg", "-i", input_file,
            "-ar", "24000",  # Частота дискретизации
            "-ac", "2",      # Стерео
            temp_file,       # Выходной файл
            "-y"             # Перезаписывать файл без запроса
        ]
        # Выполнение команды
        subprocess.run(command, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        print(f"Файл успешно преобразован: {temp_file}")
        return temp_file
    except subprocess.CalledProcessError as e:
        print(f"Ошибка при преобразовании файла: {e}")
        sys.exit(1)  # Код 1: Ошибка преобразования


def compress_audio(input_file, output_file):
    """
    Сжимает аудиофайл с помощью Encodec.
    """
    try:
        # Загружаем аудио
        print(f"Сжимаем файл {input_file}...")
        wav, sr = torchaudio.load(input_file)
        wav = convert_audio(wav, sr, target_sr=24000, target_channels=2)

        # Инициализация модели Encodec
        model = EncodecModel.encodec_model_24khz()
        model.set_target_bandwidth(6.0)

        # Кодирование и декодирование
        encoded_frames = model.encode(wav)
        decoded_audio = model.decode(encoded_frames)

        # Сохранение результата
        torchaudio.save(output_file, decoded_audio.squeeze(0), sample_rate=24000)
        print(f"Сжатый файл успешно сохранён: {output_file}")
    except Exception as e:
        print(f"Ошибка при сжатии аудио: {e}")
        sys.exit(2)  # Код 2: Ошибка сжатия


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Использование: python script.py input_file.wav output_file.wav")
        sys.exit(100)  # Код 100: Неверное использование

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    # Временный файл для преобразования
    temp_file = "/tmp/temp_24khz.wav"

    # Проверяем существование входного файла
    if not os.path.exists(input_file):
        print(f"Ошибка: входной файл не найден: {input_file}")
        sys.exit(1)

    # Преобразуем файл к 24 кГц
    temp_file = convert_to_24khz(input_file, temp_file)

    # Выполняем сжатие
    compress_audio(temp_file, output_file)

    # Удаляем временный файл
    if os.path.exists(temp_file):
        os.remove(temp_file)
