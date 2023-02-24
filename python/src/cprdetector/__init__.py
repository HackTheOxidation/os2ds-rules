from .detector import find_matches


class CPRDetector:
    '''Drop-in replacement for CPRRule.'''

    def __init__(self, check_mod11: bool = False, examine_context: bool = False):
        self._check_mod11 = check_mod11
        self._examine_context = examine_context

    def find_matches(self, content: str):
        yield from find_matches(content)
