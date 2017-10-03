from nose.tools import assert_list_equal, assert_equals, assert_not_equals
from hypothesis import given, strategies as st
from proquint import uint2quint, quint2uint


@given(st.integers(0, 0xFFFFFFFF), st.sampled_from('- |.'))
def test_encoding(uint_val, separator):
    quint = uint2quint(uint_val, separator)
    assert_not_equals(quint, uint_val)
    assert_equals(11, len(quint))

    uint_val1 = quint2uint(quint)
    assert_equals(uint_val, uint_val1)


def ip2int(ip):
    return (ip[0] << 24) + (ip[1] << 16) + (ip[2] << 8) + ip[3]


def test_presets():
    presets = [
        ([127,0,0,1],       'lusab-babad'),
        ([63,84,220,193],   'gutih-tugad'),
        ([63,118,7,35],     'gutuk-bisog'),
        ([140,98,193,141],  'mudof-sakat'),
        ([64,255,6,200],    'haguz-biram'),
        ([128,30,52,45],    'mabiv-gibot'),
        ([147,67,119,2],    'natag-lisaf'),
        ([212,58,253,68],   'tibup-zujah'),
        ([216,35,68,215],   'tobog-higil'),
        ([216,68,232,21],   'todah-vobij'),
        ([198,81,129,136],  'sinid-makam'),
        ([12,110,110,204],  'budov-kuras')]

    quints = [uint2quint(ip2int(t[0])) for t in presets]
    assert_list_equal(quints, [t[1] for t in presets])

    uints = [quint2uint(t[1]) for t in presets]
    assert_list_equal(uints, [ip2int(t[0]) for t in presets])
