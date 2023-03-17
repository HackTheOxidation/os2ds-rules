"""Utilities for benchmarking."""
import os

# This is the html of the longest page on Wikipedia:
# "List of victims of the September 11 attacks"
# which is 2.3MB of plain text where
# irrelevant images and scripts have been removed.
THIS = os.path.dirname(__file__)
BIG_HTML = os.path.join(THIS, 'data/list_9_11_victims.html')


def read_content(path):
    """Helper function that reads some content into memory."""
    content = ""
    with open(path, "r", encoding="utf-8",
                   errors="ignore") as file_pointer:
        content = file_pointer.read()

    return content

# Read the contents into memory. Yes, this will hurt.
HTML_CONTENT = read_content(BIG_HTML)


def eval_find_matches(detector, content=HTML_CONTENT):
    '''Matches a detector against content with eager evaluation.'''
    generator = detector.find_matches(content)
    # Force eager evaluation by casting generator into a list.
    return list(generator)
