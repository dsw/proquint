def _char_list_to_dict(char_list):
    return {c: k for (c, k) in zip(char_list, range(len(char_list)))}


UINT_TO_CONSONANT = 'bdfghjklmnprstvz'
UINT_TO_VOWEL = 'aiou'

CONSONANT_TO_UINT = _char_list_to_dict(UINT_TO_CONSONANT)
VOWEL_TO_UINT = _char_list_to_dict(UINT_TO_VOWEL)

MASK_LAST4 = 0xF
MASK_LAST2 = 0x3

CHARS_PER_CHUNK = 5

def _uint16_to_quint(uint16_val):
    val = uint16_val
    res = ['?']*CHARS_PER_CHUNK
    for i in range(CHARS_PER_CHUNK):
        if i&1:
            res[-i - 1] = UINT_TO_VOWEL[val & MASK_LAST2]
            val >>= 2
        else:
            res[-i - 1] = UINT_TO_CONSONANT[val & MASK_LAST4]
            val >>= 4
    return ''.join(res)


def uint2quint(uint_val, separator='-'):
    """Convert 32-bit integer value to character quintet representation.

    >>> uint2quint(0x7F000001, '-')
    lusab-babad

    :param uint_val: value to encode
    :param separator: string to separate quintets
    :return: encoded string value
    """
    if uint_val < 0 or uint_val > 0xFFFFFFFF:
        raise ValueError('uint_val should be in range 0-0xFFFFFFFF')
    return _uint16_to_quint(uint_val >> 16) + \
           separator + \
           _uint16_to_quint(uint_val)


def quint2uint(quint):
    """Convert "proquint" string into the corresponding uint representation.

    >>> hex(quint2uint('lusab-babad'))
    '0x7F000001'

    :param quint: "proquint" value to decode
    :return: uint32 representation of the proquint encoded value
    """
    res = 0
    for c in quint:
        mag = CONSONANT_TO_UINT.get(c)
        if mag is not None:
            res <<= 4
            res += mag
        else:
            mag = VOWEL_TO_UINT.get(c)
            if mag is not None:
                res <<= 2
                res += mag
    return res
