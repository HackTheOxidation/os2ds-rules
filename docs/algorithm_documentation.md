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

The fact that CPR-numbers are regular have two important implications. Firstly,
Any regular language can be recognized by a Deterministic Finite Automata (DFA).
Secondly, the DFA can be implemented such that it recognizes the corresponding
regular language in linear time, $\mathcal{O}(n)$, in relation to the size/length 
of the search space, $n$.

# Name Rule

# Address Rule

# Health Rule (Ordered Wordlist)
