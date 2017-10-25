from nose.tools import assert_list_equal, assert_equals, assert_not_equals, assert_raises
from hypothesis import given, strategies as st
from proquint import uint2quint, quint2uint, ip2uint_str, uint2ip_str, convert


@given(st.integers(0, 0xFFFFFFFF), st.sampled_from('- |.'))
def test_encoding(uint_val, separator):
    quint = uint2quint(uint_val, separator)
    assert_not_equals(quint, uint_val)
    assert_equals(11, len(quint))

    uint_val1 = quint2uint(quint)
    assert_equals(uint_val, uint_val1)


def test_presets():
    presets = [
        ('127.0.0.1',       'lusab-babad'),
        ('63.84.220.193',   'gutih-tugad'),
        ('63.118.7.35',     'gutuk-bisog'),
        ('140.98.193.141',  'mudof-sakat'),
        ('64.255.6.200',    'haguz-biram'),
        ('128.30.52.45',    'mabiv-gibot'),
        ('147.67.119.2',    'natag-lisaf'),
        ('212.58.253.68',   'tibup-zujah'),
        ('216.35.68.215',   'tobog-higil'),
        ('216.68.232.21',   'todah-vobij'),
        ('198.81.129.136',  'sinid-makam'),
        ('12.110.110.204',  'budov-kuras')]

    quints = [uint2quint(ip2uint_str(t[0])) for t in presets]
    assert_list_equal(quints, [t[1] for t in presets])

    uints = [quint2uint(t[1]) for t in presets]
    assert_list_equal(uints, [ip2uint_str(t[0]) for t in presets])


def test_incorrect_input():
    with assert_raises(ValueError):
        quint2uint('foo-barr')

    with assert_raises(ValueError):
        quint2uint('')

    with assert_raises(ValueError):
        quint2uint('1234')

    with assert_raises(ValueError):
        quint2uint('foo')

    with assert_raises(ValueError):
        quint2uint('xxxx-yyyy')

    with assert_raises(TypeError):
        quint2uint(42)


@given(st.integers(0, 0xFFFFFFFF))
def test_ipv4_conversion(uint_val):
    ip = uint2ip_str(uint_val)
    assert_equals(4, len(ip.split('.')))
    assert_equals(uint_val, ip2uint_str(ip))

    assert_equals(0xFF00FF01, ip2uint_str('255.0.255.1'))
    assert_equals('255.0.255.1', uint2ip_str(0xFF00FF01))


def test_convert():
    assert_equals('2130706433', convert('lusab-babad'))
    assert_equals('2130706433', convert('lusab-babad', 'uint'))
    assert_equals('0x7f000001', convert('lusab-babad', 'hex'))
    assert_equals('127.0.0.1', convert('lusab-babad', 'ip'))

    assert_equals('lusab-babad', convert('127.0.0.1'))
    assert_equals('lusab-babad', convert('2130706433'))
    assert_equals('lusab-babad', convert('0x7f000001'))
