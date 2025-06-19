from importlib.resources import files
import sys

from _nilsimsa import compare_hexdigests


__version__ = files(__package__).joinpath('VERSION').read_text()
__all__ = ['compare_hexdigests', '__version__']
