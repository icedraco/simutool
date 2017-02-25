"""
This module is responsible for tracing local requirements of a C/C++ source
file and building a subset of files that fulfill said requirements.

We are using this module to take initial C++ files we need to move to another
source tree, and find all the files that we ALSO need to move in order for the
source tree to actually compile.

This helps us minimally affect the other source tree and not move unnecessary
files that would otherwise interfere with it.
"""

import os
import re
import logging


class Tracer(object):
    RE_INCLUDE = re.compile('''#include "([^\"]+)"''')

    def __init__(self, code_path='.'):
        """
        :param str code_path: code path
        """
        self.log = logging.getLogger(__name__ + '.' + self.__class__.__name__)
        self.code_paths = []
        self.files = set()
        self.requirements = dict()
        self.excluded_files = set()

        self.addpath(code_path)

    @property
    def items_processed(self):
        """
        :return: list of files found and processed
        :rtype: list[str]
        """
        return self.requirements.keys()

    @property
    def items_required(self):
        """
        :return: set of files required in total by the entire source tree
        :rtype: set[str]
        """
        return set(sum(map(list, [self.requirements[k].values() for k in self.requirements.keys()]), []))

    def addpath(self, path):
        """
        NOTE: Paths will be looked up in the order they were added!

        :param str path: code lookup path
        """
        self.code_paths.append(path)

    def exclude(self, files):
        """
        :param str|list[str] files: file(s) to exclude from traversal
        """
        e = files if type(files) is not str else [files]
        self.excluded_files = self.excluded_files.union(e)

    def load(self, filename):
        """
        :param str filename: file, whose inclusion tree to process
        """
        excluded_files = self.excluded_files
        code_paths = self.code_paths

        def process(fname):
            self.log.debug("Trying to load {}...".format(fname))

            if fname in excluded_files:
                self.log.debug("  - Skipped (excluded)")
                return

            items = []
            for code_path in code_paths:
                path = os.path.join(code_path, fname)
                self.log.debug("  > Trying {}...".format(path))

                if os.path.isfile(path):
                    items = self.get_includes_from_file(path)
                    [self.log.debug("    {}".format(f)) for f in items]
                    self.log.debug("      -> {} requirements found...".format(len(items)))
                    self.log.debug("")
                    self.add(fname, path, items)
                    self.log.debug("")
                    continue

            for item in items:
                if item not in self.items_processed:
                    process(item)

        process(filename)
        return self

    def add(self, source_file, path, required_files=None):
        """
        :param str source_file: source file
        :param str path: actual location of the source file
        :param str|list[str]|None required_files: (optional) file or files required by `source_file`
        """
        if required_files is None:
            required_files = []
        elif type(required_files) is str:
            required_files = [required_files]

        req_set = self.requirements.get(source_file, set())
        self.requirements[source_file] = req_set.union(required_files)
        self.files.add(path)

    @classmethod
    def get_includes_from_file(cls, filename):
        """
        :param str filename: filename to check
        :return: list of all files included by this file
        :rtype: list[str]
        """
        return cls.RE_INCLUDE.findall(open(filename, 'r').read())
