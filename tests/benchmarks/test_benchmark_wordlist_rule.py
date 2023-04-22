from os2ds_rules.wordlist_rule import WordListRule
from .utilities import HTML_CONTENT


rule = WordListRule(["Reference", "Tower", "Building"])


def test_benchmark_wordlist_rule(benchmark):
    matches = benchmark(rule.find_matches, HTML_CONTENT)
