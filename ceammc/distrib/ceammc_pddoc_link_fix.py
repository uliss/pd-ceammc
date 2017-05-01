#!/usr/bin/env python

import re
import argparse
import sys

lines_re = re.compile("^(#(.*?)[^\\\])\r?\n?;\r?\n", re.MULTILINE | re.DOTALL)

def unquote(s):
    if len(s) == 0:
        return s

    if s[0] == '"' and s[-1] == '"':
        return s[1:-1]

    return s

def uilink2pddplink(ln, url_from=None, url_to=None):
    def obj_fix(l):
        h, o, x, y = l.split(' ')[0:4]
        return ' '.join([h, o, str(int(x)), str(int(y) - 3), "pddp/pddplink"])

    def cleanup(l):
        l = l.strip().replace("\n", " ")
        n = l.index(' ')
        k = l[0:n]
        v = l[n+1:]
        return k, v

    pd_obj = obj_fix(ln.split('@')[0])
    props = map(cleanup, ln.split('@')[1:])
    d = {key: value for (key, value) in props}

    title = unquote(d['title'])
    url = unquote(d['url'])

    if url_from is not None and url in url_from:
        idx = url_from.index(url)
        url = url_to[idx]

    return ' '.join([pd_obj, url, '-text', title + ';'])

def main(): 
    arg_parser = argparse.ArgumentParser(description='pddoc link replace tool')
    arg_parser.add_argument('--uilink', '-uilink', type=bool, help='chane ui.link properties', default=True)
    arg_parser.add_argument('--src-url', '-surl', metavar='URL', nargs='+', help='source URL')
    arg_parser.add_argument('--dest-url', '-durl', metavar='URL', nargs='+', help='destination URL')
    args = vars(arg_parser.parse_args())

    replace_url = False

    if args['src_url'] and args['dest_url']:
        if len(args['src_url']) == len(args['dest_url']):
            replace_url = True;

    lines = sys.stdin.read()
    lines += '\n'

    for found in lines_re.finditer(lines):
        ln = found.group(1)
        if "ui.link" not in ln:
            print ln + ';'
            continue

        if replace_url:
            print uilink2pddplink(ln, url_from=args['src_url'], url_to=args['dest_url'])
        else:
            print uilink2pddplink(ln)


if __name__ == '__main__':
    main() 

