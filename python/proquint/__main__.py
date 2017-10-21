import argparse
from utils import get_my_ip, convert


def main():
    desc = 'Convert between [integer, hexadecimal, IPv4 address] <-> proquint representations. '\
        'See https://arxiv.org/html/0901.4016'

    parser = argparse.ArgumentParser(description=desc)
    parser.add_argument('-n', '--uint', action='store_true',
                        help='convert from proquint to 32-bit integer', required=False)
    parser.add_argument('-x', '--hex', action='store_true',
                        help='convert from proquint to hexadecimal', required=False)
    parser.add_argument('-i', '--ip', action='store_true',
                        help='convert from proquint to IPv4', required=False)
    parser.add_argument('val', nargs='?', type=str, default=None,
                        help='value to convert (if not specified, ' \
                        'IP address of the current host is printed)')

    args = parser.parse_args()

    target = None
    if args.uint:
        target = 'uint'
    elif args.hex:
        target = 'hex'
    elif args.ip:
        target = 'ip'

    try:
        if args.val is None:
            res = convert(get_my_ip())
        else:
            res = convert(args.val, target)
    except ValueError as e:
        print('{}'.format(str(e)))
    else:
        print('{}'.format(res))


if __name__ == "__main__":
    main()
