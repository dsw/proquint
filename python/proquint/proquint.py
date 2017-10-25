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
    """Convert 32-bit integer value into corresponding proquint string identifier.

    >>> uint2quint(0x7F000001, '-')
    lusab-babad

    :param uint_val: 32-bit integer value to encode
    :param separator: string to separate character quintets
    :return: proquint string identifier
    """
    if uint_val < 0 or uint_val > 0xFFFFFFFF:
        raise ValueError('uint_val should be in range 0-0xFFFFFFFF')
    return _uint16_to_quint(uint_val >> 16) + \
           separator + \
           _uint16_to_quint(uint_val)


def quint2uint(quint):
    """Convert proquint string identifier into corresponding 32-bit integer value.

    >>> hex(quint2uint('lusab-babad'))
    '0x7F000001'

    :param quint: proquint string identifier to decode
    :return: 32-bit integer representation of the proquint encoded value
    """
    nchar = len(quint)
    if nchar < 10 or nchar > 11:
        raise ValueError('Proquint should be in form of two quintets + optional separator')

    res = 0
    for i, c in enumerate(quint):
        mag = CONSONANT_TO_UINT.get(c)
        if mag is not None:
            res <<= 4
            res += mag
        else:
            mag = VOWEL_TO_UINT.get(c)
            if mag is not None:
                res <<= 2
                res += mag
            elif i != 5:
                raise ValueError('Bad proquint format')
    return res
