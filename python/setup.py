from setuptools import setup, find_packages

__version__ = '0.2.0'

tests_require = [
    'nose >= 1.3',
    'hypothesis >= 3.22'
]

setup(
    name = 'proquint',
    packages = find_packages(),
    version = __version__,
    description = 'Proquints: Identifiers that are Readable, Spellable, and Pronounceable.',
    author = 'Ruslan Shestopalyuk',
    author_email = 'rushbits@gmail.com',
    url = 'https://github.com/dsw/proquint',
    keywords = [],
    classifiers = [],
    tests_require=tests_require,
    test_suite="nose.collector",
    entry_points = {
        'console_scripts': [
            'proquint = proquint.__main__:main'
        ],
    }
)
