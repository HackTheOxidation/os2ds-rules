from unittest import TestCase
import pytest

from os2ds_rules.name_rule import find_matches
from .utilities import read_content, WIKI_HTML, GCC_TXT


class TestNameRuleWiki(TestCase):
    @pytest.fixture(autouse=True)
    def setupBenchmark(self, benchmark):
        self.benchmark = benchmark

    def setUp(self):
        self.data = read_content(times=300, path=WIKI_HTML)
    
    def test_benchmark_name_rule(self):
        matches = self.benchmark(find_matches, self.data)


class TestNameRuleGCC(TestCase):
    @pytest.fixture(autouse=True)
    def setupBenchmark(self, benchmark):
        self.benchmark = benchmark

    def setUp(self):
        self.data = read_content(times=300, path=GCC_TXT)
    
    def test_benchmark_name_rule(self):
        matches = self.benchmark(find_matches, self.data)
