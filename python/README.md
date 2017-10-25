### What is this

"Proquints: Identifiers that are Readable, Spellable, and Pronounceable"

See [the corresponding article by Daniel Wilkerson](https://arxiv.org/html/0901.4016)

### Installing 

```bash
pip install proquint
```

### Usage

```python
>>> from proquint import uint2quint
>>> uint2quint(0x7F000001)
lusab-babad
```

### Command-line tool

The package also installs a script `proquint` to do the conversion via command line:

```
usage: proquint [-h] [-n] [-x] [-i] [val]

Convert between [integer, hexadecimal, IPv4 address] <-> proquint
representations. See https://arxiv.org/html/0901.4016

positional arguments:
  val         value to convert (if not specified, IP address of the current
              host is printed)

optional arguments:
  -h, --help  show this help message and exit
  -n, --uint  convert from proquint to 32-bit integer
  -x, --hex   convert from proquint to hexadecimal
  -i, --ip    convert from proquint to IPv4
```

Examples:

```bash
$ proquint
safom-bador

$ proquint safom-bador
3232235627

$ proquint safom-bador -x
0xc0a8006b

$ proquint safom-bador -n
3232235627

$ proquint safom-bador -i
192.168.0.107

$ proquint 127.0.0.1
lusab-babad

$ proquint 0xffffffff
zuzuz-zuzuz

$ proquint 12345678
bafus-kajav

```