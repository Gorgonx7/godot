#!/usr/bin/env python

import unittest
import methods
from SCons.Script import Dir, Environment


class test_add_source_files(unittest.TestCase):
    def test_returns_encoded_files(self):
         
        self.assertEqual(False, False)

class test_using_clang(unittest.TestCase):
    def test_returns_true(self):
        env = {"CC" : "clang"}
        self.assertEqual(methods.using_clang(env), True)
    def test_returns_false(self):
        env = {"CC" : "gcc"}
        self.assertEqual(methods.using_clang(env), False)

class test_using_gcc(unittest.TestCase):
    def test_returns_true(self):
        env = {"CC" : "gcc"}
        self.assertEqual(methods.using_gcc(env), True)
    def test_returns_false(self):
        env = {"CC" : "clang"}
        self.assertEqual(methods.using_gcc(env), False)

class test_disable_warnings(unittest.TestCase):
    def test_msvc_w_flag_added(self):
        env_base = Environment()
        env_base.msvc = True
        env_base.__class__.disable_warnings = methods.disable_warnings
        env_base.disable_warnings()
        self.assertTrue(env_base.msvc)
        print(env_base["CCFLAGS"])
        self.assertTrue("/w" in env_base["CCFLAGS"])
    def test_msvc_warn_flags(self):
        env_base = Environment()
        env_base.msvc = True
        
        
