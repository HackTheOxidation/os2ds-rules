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
from dataclasses import dataclass


def is_nonzero_digit(c) -> bool:
	return c in "123456789"


def is_digit(c) -> bool:
	return c in "0123456789"
	
	
def is_space(c) -> bool:
	return c == ' '
	

def is_separator(c) -> bool:
	return c in ' -/'


class State(Flag):
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


@dataclass
class Result:
	cpr: str
	begin: int
	end: int


# Helper functions

def reset() -> State:
	# Reset detector state to default state (Empty)
	return State.Empty


def update(c, new_state, &old_state, is_acceptable) -> (str, State):
	# Update old_state to new_state if c is an acceptable token.
	# if ok then return c else return None.

	if is_acceptable(c):
	    old_state = new_state
		return c, new_state
	else:
		return '', reset()


def check_day_month(cpr, state) -> (bool, State):
	# Check that the day/month combination is valid.
	# Return true if it is February 29th else return false.
	
	day = int(cpr[0:2])
	month = int(cpr[2:4])
	
	if month is 2:
	    if day is 29:
			return True, state
		elif day > 29:
			return False, reset()
	elif (day > 30 
		and not((month > 7 and month % 2 == 0) or
			    (month < 8 and month % 2 != 0))):
		return False, reset()
	
	return False, state


def check_leap_year(cpr, state) -> State:
	# Verify that the read year is a leap year.
	# otherwise reset the detector state.
	year = int(cpr[4:6])
	control = int(cpr[6])
	
	if control < 4 and year == 0:
		return reset()
	
	if year % 4 != 0:
		return reset()
		
	return state


def check_and_append_cpr(cpr, begin, end) -> Result:
	# Check that the control digits are valid, then
	# create and append a result to the list of results.
	int control = int(cpr[6:10])
	
	if control > 0:
	    return Result(cpr, begin, end)
	
	return None


def is_previous_ok(previous):
	# Check that the previous character is not alpha-numeric, etc.
	return previous in ' \n\t\0'


def is_next_ok(content, i):
	# Check that the next character is not alpha-numeric, etc.
	if len(content) > i + 1:
		return is_previous_ok(content[i + 1])
	
	return True
	

# Main function
def find_cpr_numbers(content: str) -> [Result]:
	results = []

	state = State.Empty
	cpr = ""
	previous = ''
	begin = 0
	allow_separator = False
	leap_year = False
	is_acceptable = lambda _: False
	
	for i, c in enumerate(content):
	    match state:
		    case State.Empty:
				if not is_previous_ok(previous)
					previous = c
				
				is_acceptable = lambda ch: ch in '0123'
				previous, state = update(c, State.First, state, is_acceptable)
				
				if state is State.First:
				    cpr[0] = c
					begin = i
			case State.First:
				match previous:
				    case '0':
						is_acceptable = is_nonzero_digit
				    case '1' | '2':
						is_acceptable = is_digit
					case '3': 
						is_acceptable = lambda c -> c is '0' | '1'
					case _:
						state = reset()
						previous = c
						continue
				
				cpr[1], state = update(c, State.Second, state, is_acceptable)
				previous = cpr[1]
				
				if previous:
					allow_separator = true
			case State.Second:
				if is_space(c) and allow_separator:
					allow_separator = False
					continue
				
				is_acceptable = lambda c: c in '01'
				cpr[2], state = update(c, State.Third, state, is_acceptable)
				previous = cpr[2]
			case State.Third:
				match previous:
				    case '0':
						is_acceptable = is_nonzero_digit
					case '1': 
						is_acceptable = lambda c: c in '012'
					case _:
						state = reset()
						previous = ''
						continue
					
				cpr[3], state = update(c, State.Fourth, state, is_acceptable)
				previous = cpr[3]
				
				leap_year, state = check_day_month(cpr, state)
				
				if previous:
					allow_separator = True
			case State.Fourth:
				if is_space(c) and allow_separator:
				    allow_separator = False
					continue
				
				is_acceptable = is_digit
				
				cpr[4], state = update(c, State.Fifth, state, is_acceptable)
				previous = cpr[4]
			case State.Fifth:
				match previous:
					case '0':
						is_acceptable = is_nonzero_digit,
					case _:
						is_acceptable = is_digit
					
				cpr[5], state = update(c, State.Sixth, state, is_acceptable)
				previous = cpr[5]
				
				if previous:
					allow_separator = True
					
			case State.Sixth:
				if allow_separator and is_separator(c):
					allow_separator = false;
					continue
				
				is_acceptable = is_digit
				cpr[6], state = update(c, State.Seventh, state, is_acceptable)
				previous = cpr[6]

				if leap_year:
					state = check_leap_year(cpr, state)

			case State.Seventh:
				is_acceptable = is_digit
				cpr[7], state = update(c, State.Eighth, state, is_acceptable)
				previous = cpr[7]
			case Eighth:
				is_acceptable = is_digit
				cpr[8], state = update(c, State.Match, state, is_acceptable)
				previous = cpr[8]
			case Match:
				cpr[9] = c if is_digit(c) else ''
				
				if is_next_ok(content[i+1])
					result = check_and_append_cpr(cpr, begin, i)
					if result:
						results.append(result)
					
				previous = c
				allow_separator = False;
				state = reset()
			
	return results
}
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
of the `check_and_append_cpr()`-function (and `find_matches()`) and adding a new helper 
function called `check_mod11()`:

```

modulus11_factors = [4, 3, 2, 7, 6, 5, 4, 3, 2, 1]


def check_mod11(result) -> bool:
	# Calculate the Modulus 11 sum and check that 11 divides it.
	return sum([a*b for a, b in zip(result.cpr, modulus11_factors)]) % 11 == 0

def check_and_append_cpr(cpr, begin, end, do_check_mod11) -> Result:
	# Check that the control digits are valid, then
	# create and append a result to the list of results.
	int control = int(cpr[6:10])
	
	if control > 0:
	    result =  Result(cpr, begin, end)
		
		if do_check_mod11 and not check_mod11(result):
			return None
			
		return result
	
	return None
```

This code does a simple calculation of the sum of each of the digits in a CPR-number
multiplied by the modulus 11 factor as specified by CPR Administrationen and checks
that 11 divides this sum.

As in the case of the `check_mod11`-option, the behavior of the `examine_context`-option can
also be integrated into the algorithm by modifying the `check_and_append_cpr()`-function.
However, this is insufficient as this doesn't check of the occurance of a blacklisted word
that is not in the vicinty of a match. So, the `find_matches()`-function also needs to be
altered.

We will however, delay the introduction to this definition, as it relies on a concept that
will be introduced in the coming sections.

# A unified replacement for the underlying mechanism of the Name Rule, Address Rule and Health Rule

## Name Rule

## Address Rule

## Health Rule (Ordered Wordlist)
