#!/usr/bin/env python3

from scipy import signal
import numpy as np
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='freqz script')

    parser.add_argument('-n', action='store', default=16, type=int,
        help='Number of samples')
    parser.add_argument('-b', default=[], nargs='+', type=float,
            help='"b.." coefficents')
    parser.add_argument('-a', default=[1], nargs='+', type=float,
        help='"a.." coefficents')
    parser.add_argument('--amp', action='store_true', default=True,
            help='output amp response')
    parser.add_argument('--phase', action='store_true', default=False,
            help='output phase response')
    args = parser.parse_args()

    w, hw = signal.freqz(args.b, args.a, args.n, include_nyquist=True)

    if args.phase:
        for h in np.angle(hw):
            print(h)
    elif args.amp:
        for h in hw:
            print(abs(h))



