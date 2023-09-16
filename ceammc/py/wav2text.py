#!/usr/bin/env python

import audiofile
import argparse
import sys
import json


def main():
    parser = argparse.ArgumentParser(description='WAV files to text converter')
    parser.add_argument('FILE', type=str, help='source WAV file')
    args = parser.parse_args()

    try:
        signal, sr = audiofile.read(args.FILE, always_2d=True)

        data = dict()
        data["frames"] = audiofile.samples(args.FILE)
        data["channels"] = audiofile.channels(args.FILE)
        data["bit"] = audiofile.bit_depth(args.FILE)
        data["sr"] = sr
        data["data"] = signal.tolist()

        print(json.dumps(data, indent=2))
    except FileNotFoundError as e:
        print(e)
        sys.exit(1)


if __name__ == '__main__':
    main()
