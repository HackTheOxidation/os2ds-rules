from os2ds_rules.cpr_detector import find_matches
from .utilities import HTML_CONTENT


def test_benchmark_cpr_detector_default(benchmark):
    matches = benchmark(find_matches, HTML_CONTENT)


def test_benchmark_cpr_detector_check_mod11(benchmark):
    matches = benchmark(find_matches, HTML_CONTENT, True)
