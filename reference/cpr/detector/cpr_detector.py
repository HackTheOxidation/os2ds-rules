from enum import Flag, auto
from functools import reduce
from typing import Iterable


__all__ = [
    'CPRNumber',
    'CPRDetector',
    ]


_nonzero_digits: str = "123456789"
_digits: str = "0" + _nonzero_digits
_separators: str = ' -/'


def _print_debug(state, c, cpr, previous):
    print(f"state = {state}\n"
          f"c = {c}\n"
          f"cpr = {cpr}\n"
          f"previous = {previous}\n\n")


def _init_cpr() -> dict[int, str | None]:
    ''''''
    return {
            0: None, 1: None, 
            2: None, 3: None, 
            4: None, 5: None, 
            6: None, 7: None, 
            8: None, 9: None, 
            }


def string_to_cpr_dict(string: str):
    '''Takes a string and converts it to a dictionary.'''
    cpr = {}
    for i, c in enumerate(string):
        cpr[i] = c

    return cpr


class CPRNumber:
    def __init__(self, cpr):
        self._cpr = cpr

    def day(self) -> int:
        '''Returns the day of the DDMMYY part.'''
        return int(self._cpr[0] + self._cpr[1])

    def month(self) -> int:
        '''Returns the month of the DDMMYY part.'''
        return int(self._cpr[2] + self._cpr[3])

    def year(self) -> int:
        '''Returns the year of the DDMMYY part.'''
        return int(self._cpr[4] + self._cpr[5])

    def control(self, ret_type = str):
        '''Returns the control part of DDMMYY-CCCC. The return-type can be altered with the "ret_type" parameter.'''
        cpr = self._cpr[6] + self._cpr[7] + self._cpr[8] + self._cpr[9]
        return ret_type(cpr) 

    def __str__(self) -> str:
        return reduce(lambda a, b: a + b,
                      (c for c in self._cpr.values()))

    def __eq__(self, other) -> bool:
        if isinstance(other, CPRNumber):
            return (
                self.day() == other.day() and
                self.month() == other.month() and
                self.year() == other.year() and
                self.control() == other.control()
                    )

        return False


class CPRDetector:
    '''Utility class for detecting Danish CPR-numbers (personnummer) in text.'''

    class _State(Flag):
        '''Internal State for tracking progress of detection.'''
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

    def __init__(self, check_mod11=False, examine_context=False):
        self._state = self._State.EMPTY
        self._check_mod11 = check_mod11
        self._examine_context = examine_context

    def _reset(self):
        '''Resets the internal DetectorState to EMPTY.'''
        self._state = self._State.EMPTY

    def _update(self, c: str, new_state, accepted: Iterable) -> bool:
        '''Updates the internal state to "new_state" if "c" is in the collection of acceptable inputs.'''
        if c in accepted:
            self._state = new_state
            return True
        else:
            self._reset()
            return False

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

    def _check_leap_year(self, cpr):
        year = int(cpr[4] + cpr[5])
        control = int(cpr[6])

        if control < 4 and year == 0:
            self._reset()

        if year % 4 != 0:
            # Nope, not a leap year. This is invalid.
            self._reset() 

    def _is_previous_ok(self, previous) -> bool:
        return previous in ' \n\t\0'

    def _is_next_ok(self, content, i) -> bool:
        if len(content) > i + 1:
            return self._is_previous_ok(content[i + 1])

        return True

    def find_matches(self, content: str) -> Iterable:
        '''Finds all occurances of a CPR-number in content string.'''

        # If the string is too short to contain a CPR-number then return.
        if content is None or len(content) < 10:
            return

        cpr: dict[int, str | None] = _init_cpr()
        previous: str = ""
        begin: int = 0
        allow_separator: bool = False

        for i, c in enumerate(content):
            match self._state:
                case self._State.EMPTY:
                    if not self._is_previous_ok(previous):
                        previous = c
                        continue
                    
                    ok = self._update(c, self._State.FIRST, '0123')
                    previous = c

                    # Record the the first matching symbol and its index.
                    if ok:
                        cpr[0] = c
                        begin = i

                case self._State.FIRST:
                    accepted = []
                    if previous in "12":
                        accepted = _digits
                    elif previous == "0":
                        accepted = _nonzero_digits
                    elif previous == "3":
                        accepted = '01'

                    ok = self._update(c, self._State.SECOND, accepted)
                    previous = c

                    if ok:
                        cpr[1] = c
                        allow_separator = True

                case self._State.SECOND:
                    # We may skip a space.
                    if c == " " and allow_separator:
                        allow_separator = False
                        continue
                    
                    ok = self._update(c, self._State.THIRD, '01')
                    previous = c

                    if ok:
                        cpr[2] = c

                case self._State.THIRD:
                    accepted = []
                    match previous:
                        case '0':
                            accepted = _nonzero_digits
                        case '1':
                            accepted = '012'
                            
                    ok = self._update(c, self._State.FOURTH, accepted)
                    previous = c

                    # Check that the day-month combination is valid.
                    # If it is February 29th, raise a flag so that we can check for leap year later.

                    if ok:
                        cpr[3] = c
                        allow_separator = True
                        leap_year = self._check_day_month(cpr)

                case self._State.FOURTH:
                    # We may skip a space.
                    if c == " " and allow_separator:
                        allow_separator = False
                        continue
                    
                    ok = self._update(c, self._State.FIFTH, _digits)
                    previous = c

                    if ok:
                        cpr[4] = c

                case self._State.FIFTH:
                    accepted = []
                    match previous:
                        case '0':
                            accepted = _nonzero_digits
                        case x if x in _nonzero_digits:
                            accepted = _digits

                    ok = self._update(c, self._State.SIXTH, accepted)
                    previous = c

                    if ok:
                        cpr[5] = c
                        allow_separator = True

                case self._State.SIXTH:
                    # We may skip one of the separator symbols.
                    if allow_separator and c in _separators:
                        allow_separator = False
                        continue
                   
                    ok = self._update(c, self._State.SEVENTH, _digits)
                    previous = c

                    if ok:
                        cpr[6] = c

                    # This may be a leap, so check if it is.
                    if leap_year:
                        self._check_leap_year(cpr)

                case self._State.SEVENTH:
                    ok = self._update(c, self._State.EIGHTH, _digits)
                    previous = c

                    if ok:
                        cpr[7] = c

                case self._State.EIGHTH:
                    ok = self._update(c, self._State.MATCH, _digits)
                    previous = c

                    if ok:
                        cpr[8] = c

                case self._State.MATCH:

                    ok = self._update(c, self._State.MATCH, _digits)
                    previous = c

                    if ok:
                        cpr[9] = c

                        # We won't accept XXXXXX-0000 as a valid CPR-number.
                        control = int(cpr[6] + cpr[7] + cpr[8] + cpr[9])

                        if control > 0:
                            yield {
                                "cpr": cpr,
                                "begin": begin,
                                "end": i,
                                }

                    cpr = _init_cpr()
                    self._reset()
