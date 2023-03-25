from os2ds_rules.name_rule import find_matches
from .utilities import HTML_CONTENT


def test_benchmark_name_rule(benchmark):
    matches = benchmark(find_matches, HTML_CONTENT)
