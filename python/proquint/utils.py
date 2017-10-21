import socket
from proquint import quint2uint, uint2quint


def get_my_ip():
    """Return the external IPv4 address of the current host, otherwise 127.0.0.1"""
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(('8.8.8.8', 80))
        ip_addr = s.getsockname()[0]
        s.close()
        return ip_addr
    except OSError:
        return '127.0.0.1'


def ip2uint_str(ipv4_str):
    """Convert IPv4 string to 32-bit integer value"""
    parts = ipv4_str.split('.')
    if len(parts) != 4:
        raise ValueError('Expected IPv4 address in form A.B.C.D, got {}'.
                         format(ipv4_str))
    ip = [0]*4
    for i, part in enumerate(parts):
        try:
            int_part = int(part)
        except ValueError:
            raise ValueError('Part {} of IPv4 address is not an integer'.
                             format(i))
        if int_part < 0 or int_part > 255:
            raise ValueError('Part {} of IPv4 address is not in range 0-255'.
                             format(i))
        ip[i] = int_part
    return (ip[0] << 24) + (ip[1] << 16) + (ip[2] << 8) + ip[3]


def uint2ip_str(uint_val):
    "Covert 32-bit integer value to IPv4 string"
    return '{}.{}.{}.{}'.format(
        (uint_val >> 24)&0xFF,
        (uint_val >> 16)&0xFF,
        (uint_val >> 8)&0xFF,
        uint_val&0xFF)


def uint2quint_str(uint_str, separator='-'):
    return uint2quint(int(uint_str), separator)


def quint2uint_str(quint):
    return str(quint2uint(quint))


def hex2quint_str(hex_str, separator='-'):
    return uint2quint(int(hex_str, 16), separator)


def quint2hex_str(quint):
    return hex(quint2uint(quint))


def convert(str_val, target=None):
    """Convert between proquint, integer, hex or IPv4 string representations.
    Tries to guess the representation from input.

    :param str_val: input representation (string)
    :return: output representation (string)
    """
    if target is not None and target not in {'uint', 'hex', 'ip'}:
        raise ValueError('Convert target should be one of: uint, hex, ip')

    if target == 'uint':
        return quint2uint_str(str_val)

    if target == 'hex':
        return quint2hex_str(str_val)

    if target == 'ip':
        return uint2ip_str(quint2uint(str_val))

    # try to guess the representation
    try:
        return quint2uint_str(str_val)
    except ValueError:
        pass

    try:
        return uint2quint_str(str_val)
    except ValueError:
        pass

    try:
        return hex2quint_str(str_val)
    except ValueError:
        pass

    try:
        return uint2quint_str(ip2uint_str(str_val))
    except ValueError:
        pass

    raise ValueError('Unrecognized input format: {}'.format(str_val))
