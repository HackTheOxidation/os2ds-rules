from os2ds_rules.cpr_detector import find_matches
from .utilities import HTML_CONTENT


def test_benchmark_cpr_detector(benchmark):
    matches = benchmark(find_matches, HTML_CONTENT)
    assert len(matches) == 1000
