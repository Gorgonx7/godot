import unittest

import methods


class test_add_source_files(unittest.TestCase):
    def test_returns_encoded_files(self):
        methods.add_source_file(["./main/main.cpp"], "*.cpp")
        self.assertEqual()
