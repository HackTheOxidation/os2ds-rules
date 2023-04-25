from unittest import TestCase
import pytest

from os2ds_rules.wordlist_rule import WordListRule
from .utilities import read_content, WIKI_HTML, GCC_TXT


rule = WordListRule(["Reference", "Tower", "Building"])


class TestWordListRuleWiki(TestCase):
    @pytest.fixture(autouse=True)
    def setupBenchmark(self, benchmark):
        self.benchmark = benchmark

    def setUp(self):
        self.data = read_content(times=300, path=WIKI_HTML)
    
    def test_benchmark_wordlist_rule(self):
        matches = self.benchmark(rule.find_matches, self.data)


class TestWordListRuleGCC(TestCase):
    @pytest.fixture(autouse=True)
    def setupBenchmark(self, benchmark):
        self.benchmark = benchmark

    def setUp(self):
        self.data = read_content(times=300, path=GCC_TXT)
    
    def test_benchmark_wordlist_rule(self):
        matches = self.benchmark(rule.find_matches, self.data)
