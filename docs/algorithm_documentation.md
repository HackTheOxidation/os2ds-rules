---
title: "Proposals for new Algorithms for OS2datascanner's rule system."
author: "Tomas Hagenau Andersen"
date: "2023-03-02"
---

# Introduction

This paper documents the proposals for new algorithms for the OS2datascanner's
rule system, which aims to simplify and optimize the time complexities of each
of the rules.

# `CPRRule`

The proposal for a new algorithm for detecting CPR-numbers relies on the fact that
a CPR-number can be expressed with a regular expression. That is, the "language" which
describes all valid CPR-numbers in the format `DDMMYYCCCC` is a regular language.
A proof of this, along with a formal definition of "the language of CPR-numbers", is
presented in "CPR-numbers as a regular language: A Proof".

The fact that CPR-numbers are regular have two important implications. Firstly,
Any regular language can be recognized by a Deterministic Finite Automata (DFA).
Secondly, the DFA can be implemented such that it recognizes the corresponding
regular language in linear time, $\mathcal{O}(n)$, in relation to the size/length 
of the search space, $n$.

Now, since a regular language is also a context-free language, one could also choose
an implementation based on a Push-Down Automaton (PDA) or even a Turing Machine.
These would certainly be possible, but it would also have undesirable consequences,
since it would worsen both the time and space complexity (see the Chomsky Hierarchy).

The new algorithm is based on a finite automaton with 10 states, one for each digit
in a CPR-number. Three of these states have inner states in cases where parts of a
CPR-number may be separated by whitespace, a '/' or a hyphen.

![Finite Automata for CPR Algorithm](cpr_state_machine.png "Finite Automata for CPR Algorithm")

The input string is fed to this automaton, character by character. If the automaton reaches the
match state and the input is valid, a result is emitted. Otherwise, the automaton is reset.
This continues until there are no more input characters left.

The reference implementation for the algorithm itself is shown below using python:

```python
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
```

In the code above, besides using a finite automaton as a basis, a few other interesting
programming concepts are used to make the algorithm work. First and foremost, it uses
the look-ahead (and look-behind) technique which is often seen in lexers and parsers.
Also, it makes use of some ideas from functional programming such as lambdas 
(anonymous functions/closures), pattern matching (a control flow structure similar to
a switch-statement), first-class functions and pure functions.

Note that this implementation omits the options `check_mod11` and `examine_context`,
but has the `ignore_irrelevant`-option fundamentally built in (cannot be turned of).

Adding the behavior of the `check_mod11`-option can be done by altering the definition
of the `find_matches()`-function and adding a new helper 
function called `_do_check_mod11()`:

```
# Factors for the Modulus 11 rule.
modulus11_factors = [4, 3, 2, 7, 6, 5, 4, 3, 2, 1]


def _do_check_mod11(self, cpr) -> bool:
	'''Calculate the Modulus 11 sum and check that 11 divides it.'''
	return sum([int(a)*b for a, b in zip(cpr, modulus11_factors)]) % 11 == 0
	
...
# In find_matches():
                case self._State.MATCH:

                    ok = self._update(c, self._State.MATCH, _digits)
                    previous = c

                    if ok:
                        cpr[9] = c

                        # We won't accept XXXXXX-0000 as a valid CPR-number.
                        control = int(cpr[6] + cpr[7] + cpr[8] + cpr[9])
						
						if self._check_mod11 and not self._do_check_mod11(cpr):
							cpr = _init_cpr()
							self._reset()
							continue

                        if control > 0:
                            yield {
                                "cpr": cpr,
                                "begin": begin,
                                "end": i,
                                }

                    cpr = _init_cpr()
                    self._reset()
```

This code does a simple calculation of the sum of each of the digits in a CPR-number
multiplied by the modulus 11 factor as specified by CPR Administrationen and checks
that 11 divides this sum.

As in the case of the `check_mod11`-option, the behavior of the `examine_context`-option can
also be integrated into the algorithm by modifying the `find_matches()`-function.
However, this is insufficient as this doesn't check of the occurance of a blacklisted word
that is not in the vicinty of a match. 

We will however, delay the introduction to this definition, as it relies on a concept that
will be introduced in the coming sections.

# A unified replacement for the underlying mechanism of the Name Rule, Address Rule and Health Rule

In essence, the Name Rule, the Address Rule and the Health Rule (and thus the Ordered Wordlist Rule)
try to solve the same underlying problem, or at least very similar problems. 
In the current version, these rules are implemented using different strategies. That is, search a
text and find occurances of a finite set of specified substrings.
From an algorithmic perspective, this isn't necessarily a problem. However, from a software design
perspective, it is, since code reuse is generally encouraged and having three implementations that
share no code at all isn't great.

Therefore, we propose a unification of the strategy behind these rules.

## Name Rule

## Address Rule

## Health Rule (Ordered Wordlist)
