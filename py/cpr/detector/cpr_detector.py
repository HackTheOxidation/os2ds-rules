from enum import Flag, auto
from functools import reduce
from typing import Iterable


class DetectorState(Flag):
    ''''''
    EMPTY = auto()
    FIRST = auto()
    SECOND = auto()
    THIRD = auto()
    FOURTH = auto()
    FIFTH = auto()
    SIXTH = auto()
    SEVENTH = auto()
    EIGHTH = auto()
    MATCH = auto()


class CPRNumber:
    def __init__(self, cpr):
        self._cpr = cpr

    def __str__(self):
        return reduce(lambda a, b: a + b,
                      (c for c in self._cpr.values()))

    def day(self) -> int:
        '''Returns the day of the DDMMYY part.'''
        return int(self._cpr[0] + self._cpr[1])

    def month(self) -> int:
        '''Returns the month of the DDMMYY part.'''
        return int(self._cpr[2] + self._cpr[3])

    def year(self) -> int:
        '''Returns the year of the DDMMYY part.'''
        return int(self._cpr[4] + self._cpr[5])


def init_cpr() -> dict[int, str | None]:
    ''''''
    return {
            0: None, 1: None, 
            2: None, 3: None, 
            4: None, 5: None, 
            6: None, 7: None, 
            8: None, 9: None, 
            }


def digits() -> str:
    ''''''
    return "0" + nonzero_digits()

def nonzero_digits() -> str:
    ''''''
    return "123456789"


def separators() -> str:
    ''''''
    return ' -/'


class CPRDetector:
    '''Utility class for detecting Danish CPR-numbers (personnummer) in text.'''
    def __init__(self):
        self._state = DetectorState.EMPTY

    def _reset(self):
        '''Resets the internal DetectorState to EMPTY.'''
        self._state = DetectorState.EMPTY

    def _update(self, c: str, new_state: DetectorState, accepted: Iterable) -> str:
        '''Updates the internal state to "new_state" if "c" is in the collection of acceptable inputs.'''
        if c in accepted:
            self._state = new_state
            return c
        else:
            self._reset()
            return ""

    def _check_day_month(self, cpr) -> bool:
        '''Checks that the first four digits of the supplied CPR-number constitutes a valid day-month pair.'''
        day = int(cpr[0] + cpr[1])
        month = int(cpr[2] + cpr[3])

        match month:
            case 2 | 4 | 6 | 9 | 11 if day > 30:
                self._reset()
            case 2 if day > 29:
                self._reset()
            case 2 if day == 29:
                return True

        return False

    def find_matches(self, content: str) -> Iterable:
        '''Finds all occurances of a CPR-number in content string.'''

        # If the string is too short to contain a CPR-number then return.
        if content is None or len(content) < 10:
            return

        cpr: dict[int, str | None] = init_cpr()
        previous: str = ""
        begin: int = 0
        allow_separator: bool = False

        for i, c in enumerate(content):
            match self._state:
                case DetectorState.EMPTY:
                    previous = cpr[0] = self._update(c, DetectorState.FIRST, '0123')
                    if previous:
                        begin = i

                case DetectorState.FIRST:
                    accepted = []
                    if previous in "12":
                        accepted = digits()
                    elif previous == "0":
                        accepted = nonzero_digits()
                    elif previous == "3":
                        accepted = '01'

                    previous = cpr[1] = self._update(c, DetectorState.SECOND, accepted)

                    allow_separator = True

                case DetectorState.SECOND:
                    if c == " " and allow_separator:
                        allow_separator = False
                        continue
                    
                    previous = cpr[2] = self._update(c, DetectorState.THIRD, '01')

                case DetectorState.THIRD:
                    accepted = []
                    match previous:
                        case '0':
                            accepted = nonzero_digits()
                        case '1':
                            accepted = '012'
                            
                    previous = cpr[3] = self._update(c, DetectorState.FOURTH, accepted)
                    leap_year = self._check_day_month(cpr)

                    allow_separator = True

                case DetectorState.FOURTH:
                    if c == " " and allow_separator:
                        allow_separator = False
                        continue
                    
                    previous = cpr[4] = self._update(c, DetectorState.FIFTH, digits())

                case DetectorState.FIFTH:
                    accepted = []
                    match previous:
                        case '0':
                            accepted = nonzero_digits()
                        case '1':
                            accepted = digits()

                    previous = cpr[5] = self._update(c, DetectorState.SIXTH, accepted)

                    if leap_year:
                        year = int(cpr[4] + cpr[5])
                        if year % 4 != 0:
                           self._reset() 

                    allow_separator = True

                case DetectorState.SIXTH:
                    if allow_separator and c in separators():
                        allow_separator = False
                        continue
                    
                    previous = cpr[6] = self._update(c, DetectorState.SEVENTH, digits())

                case DetectorState.SEVENTH:
                    previous = cpr[7] = self._update(c, DetectorState.EIGHTH, digits())

                case DetectorState.EIGHTH:
                    previous = cpr[8] = self._update(c, DetectorState.MATCH, digits())

                case DetectorState.MATCH:
                    previous = cpr[9] = self._update(c, DetectorState.MATCH, digits())

                    control = int(cpr[6] + cpr[7] + cpr[8] + cpr[9])

                    if control <= 0:
                        yield {
                            "cpr": CPRNumber(cpr),
                            "begin": begin,
                            "end": i,
                            }

                    self._reset()
