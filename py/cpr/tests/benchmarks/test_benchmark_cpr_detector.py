from ...detector.cpr_detector import CPRDetector
from .utilities import eval_find_matches


def test_benchmark_cpr_detector(benchmark):
    detector = CPRDetector()
    benchmark(eval_find_matches, detector)
