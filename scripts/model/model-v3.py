import argparse
from pathlib import Path

import torchaudio
import torch

from encodec.compress import compress, decompress
from encodec.utils import save_audio, convert_audio
from encodec.model import EncodecModel

SUFFIX = '.ecdc'

def encodec_model_24khz(model_name: str):
    target_bandwidths = [1.5, 3., 6, 12., 24.]
    sample_rate = 24_000
    channels = 1

    model = EncodecModel._get_model(
        target_bandwidths, sample_rate, channels,
        causal=True, model_norm='weight_norm', audio_normalize=False,
        name='encodec_24khz')
    
    device = torch.device('cpu')
    state_dict = torch.load(model_name, map_location=device)
    model.load_state_dict(state_dict)
    model.to(device)

    model.eval()

    return model

def encodec_model_48khz(model_name: str):
    target_bandwidths = [3., 6., 12., 24.]    
    sample_rate = 48_000
    channels = 2

    model = EncodecModel._get_model(
        target_bandwidths, sample_rate, channels,
        causal=False, model_norm='time_group_norm', audio_normalize=True,
        segment=1., name='encodec_48khz')
    
    device = torch.device('cpu')
    state_dict = torch.load(model_name, map_location=device)
    model.load_state_dict(state_dict)
    model.to(device)

    model.eval()

    return model

MODELNAMES = {
    'encodec_24khz': 'models/checkpoint_24khz.th',
    'encodec_48khz': 'models/encodec_48khz.th',
}

def get_parser():
    parser = argparse.ArgumentParser('utility for compress')
    parser.add_argument('input_path', type=Path, help='Input file, simple path')
    parser.add_argument('output_path', type=Path, help='Output file, simple path')
    parser.add_argument('-q', '--hq', action='store_true', help='hq - 48khz model')
    return parser

def main():
    args = get_parser().parse_args()

    if args.input_path.suffix.lower() == SUFFIX:
        if args.output_path is None:
            args.output_path = args.input_path.with_name(args.input_path.stem + args.decompress_suffix).with_suffix('.wav')                

        out, out_sample_rate = decompress(args.input_path.read_bytes())        
        save_audio(out, args.output_path, out_sample_rate, False)
    else:
        if args.output_path is None:
            args.output_path = args.input_path.with_suffix(SUFFIX)

        model_name = 'encodec_48khz' if args.hq else 'encodec_24khz'
        if model_name == 'encodec_24khz':
            model = encodec_model_24khz(MODELNAMES[model_name])
        else:
            model = encodec_model_48khz(MODELNAMES[model_name])
            
        model.set_target_bandwidth(6.0)

        wav, sr = torchaudio.load(args.input_path)
        wav = convert_audio(wav, sr, model.sample_rate, model.channels)
        compressed = compress(model, wav, False)

        if args.output_path.suffix.lower() == SUFFIX:
            args.output_path.write_bytes(compressed)
        else:
            assert args.output_path.suffix.lower() == '.wav'
            out, out_sample_rate = decompress(compressed)            
            save_audio(out, args.output_path, out_sample_rate, False)


if __name__ == '__main__':
    main()
