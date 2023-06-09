"""Utilities for benchmarking."""
import os

# This is the html of the longest page on Wikipedia:
# "List of victims of the September 11 attacks"
# which is 2.3MB of plain text where
# irrelevant images and scripts have been removed.
THIS = os.path.dirname(__file__)
WIKI_HTML = os.path.join(THIS, 'data/list_9_11_victims.html')
GCC_TXT = os.path.join(THIS, 'data/gcc.txt')


def read_content(times=1, path=WIKI_HTML):
    """Helper function that reads some content into memory."""
    content = ""
    with open(path, "r", encoding="utf-8",
                   errors="ignore") as file_pointer:
        text = file_pointer.read()
        for _ in range(times):
            content += text

    return content
