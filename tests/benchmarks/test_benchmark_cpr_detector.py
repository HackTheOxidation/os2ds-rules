from unittest import TestCase
import pytest

from os2ds_rules.cpr_detector import find_matches
from .utilities import read_content, WIKI_HTML, GCC_TXT


class TestCPRDetectorWiki(TestCase):
    @pytest.fixture(autouse=True)
    def setupBenchmark(self, benchmark):
        self.benchmark = benchmark

    def setUp(self):
        self.data = read_content(times=300, path=WIKI_HTML)

    def test_benchmark_cpr_detector_default(self):
        matches = self.benchmark(find_matches, self.data)

    def test_benchmark_cpr_detector_check_mod11(self):
        matches = self.benchmark(find_matches, self.data, True)

    def test_benchmark_cpr_detector_examine_context(self):
        matches = self.benchmark(find_matches, self.data, False, True)


class TestCPRDetectorGCC(TestCase):
    @pytest.fixture(autouse=True)
    def setupBenchmark(self, benchmark):
        self.benchmark = benchmark

    def setUp(self):
        self.data = read_content(times=300, path=GCC_TXT)

    def test_benchmark_cpr_detector_default(self):
        matches = self.benchmark(find_matches, self.data)

    def test_benchmark_cpr_detector_check_mod11(self):
        matches = self.benchmark(find_matches, self.data, True)

    def test_benchmark_cpr_detector_examine_context(self):
        matches = self.benchmark(find_matches, self.data, False, True)
