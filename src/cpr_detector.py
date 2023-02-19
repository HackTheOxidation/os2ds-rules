from enum import Flag, auto


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


def CPRNumber:
    def __init__(self, cpr):
        self._cpr = cpr

    def __str__(self):
        return ""


def init_cpr():
    ''''''
    return {
            0: None, 1: None, 
            2: None, 3: None, 
            4: None, 5: None, 
            6: None, 7: None, 
            8: None, 9: None, 
            }


def digits():
    ''''''
    return "0" + nonzero_digits()

def nonzero_digits():
    ''''''
    return "123456789"


def delimiters():
    ''''''
    return ' -/'


class CPRDetector:
    ''''''
    def __init__(self):
        self._state = DetectorState.EMPTY

    def _reset(self):
        self._state = DetectorState.EMPTY

    def _update(self, c: str, new_state: DetectorState, accepted):
        if c in accepted:
            self._state = new_state
            return c
        else:
            self._reset()
            return None

    def _check_day_month(self, cpr):
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

    def find_match(self, content: str, reset: bool = True):
        if content is None or len(content) < 10:
            return

        cpr = init_cpr()
        previous = None
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
                        accepted = '0' | '1'

                    previous = cpr[1] = self._update(c, DetectorState.SECOND, accepted)

                case DetectorState.SECOND:
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

                case DetectorState.FOURTH:
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

                case DetectorState.SIXTH:
                    previous = cpr[6] = self._update(c, DetectorState.SEVENTH, accepted)
                case DetectorState.SEVENTH:
                    previous = cpr[7] = self._update(c, DetectorState.EIGHTH, accepted)
                case DetectorState.EIGHTH:
                    previous = cpr[8] = self._update(c, DetectorState.MATCH, accepted)
                case DetectorState.MATCH:
                    previous = cpr[9] = self._update(c, DetectorState.MATCH, accepted)

                    control = int(cpr[6] + cpr[7] + cpr[8] + cpr[9])

                    if control <= 0:
                        self._reset()
                        continue

                    return {
                        "cpr": cpr,
                        "begin": begin,
                        "end": i,
                        }

