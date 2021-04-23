#!/usr/bin/env python

import unittest
import methods
import glob
import os

from SCons.Script import Dir, Environment
from unittest.mock import patch, mock_open, call, MagicMock

class test_add_source_files(unittest.TestCase):
    def test_returns_encoded_file(self):
        env_base = Environment()
        env_base.__class__.add_source_files = methods.add_source_files
        sources = []
        files = ["main/main.cpp"]
        env_base.add_source_files(sources, files)
        dir_path = env_base.Dir(".").abspath
        testOBJ = [env_base.Object(dir_path + "/" + files[0])]
        self.assertTrue(sources[0] in testOBJ)
        
    @patch('builtins.print')
    def test_warns_dupes(self, mock_print):
        env_base = Environment()
        env_base.__class__.add_source_files = methods.add_source_files
        sources = []
        files = ["main/main.cpp", "main/main.cpp"]
        env_base.add_source_files(sources, files)
        mock_print.assert_called_with('WARNING: Object "{}" already included in environment sources.'.format(sources[0]))
    
    @patch('builtins.print')
    def test_err_wildcards(self, mock_print):
        env_base = Environment()
        env_base.__class__.add_source_files = methods.add_source_files
        sources = []
        files = "#*"
        env_base.add_source_files(sources, files)
        mock_print.assert_called_with("ERROR: Wildcards can't be expanded in SCons project-absolute path: '{}'".format(files))
    def test_string(self):
        env_base = Environment()
        env_base.__class__.add_source_files = methods.add_source_files
        sources = []
        files = "main/main.cpp"
        env_base.add_source_files(sources, files)
        dir_path = env_base.Dir(".").abspath
        testOBJ = [env_base.Object(dir_path + "/" + files)]
        self.assertTrue(sources[0] in testOBJ)
    def test_multiple_files(self):
        env_base = Environment()
        env_base.__class__.add_source_files = methods.add_source_files
        sources = []
        files = ["main/main.cpp", "main/performance.cpp"]
        env_base.add_source_files(sources, files)
        dir_path = env_base.Dir(".").abspath
        testOBJ = [env_base.Object(dir_path + "/" + files[0]), env_base.Object(dir_path + "/" + files[1])]
        self.assertTrue(sources[0] in testOBJ)
        self.assertTrue(sources[1] in testOBJ)

class test_update_version(unittest.TestCase):
    #@patch('builtins.open')
    #@patch('builtins.print')
    #def test_custom_build_name(self, mock_print, mock_open):
    #    os.environ["BUILD_NAME"] = "CustomBuildName"
    #    methods.update_version()
    #    mock_print.assert_called_with("Using custom build name: " + "CustomBuildName")
    @patch('methods.write_version_file')
    @patch('methods.write_version_hash_file')
    def test_version_generated(self, mock_version_hash_file, mock_write_version_file):
        methods.update_version()
        mock_version_hash_file.assert_called_once()
        mock_write_version_file.assert_called_once()
    @patch('methods.get_version_hash')
    def test_write_version_hash_file(self, mock_get_version_hash):
        mock_get_version_hash.side_effect = ["test"]
        with patch('builtins.open', unittest.mock.mock_open()) as m:
            methods.write_version_hash_file()
        m.assert_called_with("core/version_hash.gen.h", "w")
        mock_get_version_hash.assert_called_once()
        handle = m()
        calls = [call("/* THIS FILE IS GENERATED DO NOT EDIT */\n"), 
                 call("#ifndef VERSION_HASH_GEN_H\n"), 
                 call("#define VERSION_HASH_GEN_H\n"), 
                 call('#define VERSION_HASH "' + "test" + '"\n'),
                 call("#endif // VERSION_HASH_GEN_H\n")]
        handle.write.assert_has_calls(calls)
        

        

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
    def test_msvc_w_flag_added_CCFLAGS(self):
        env_base = Environment()
        env_base.msvc = True
        env_base.__class__.disable_warnings = methods.disable_warnings
        env_base.disable_warnings()
        self.assertTrue(env_base.msvc)
        self.assertTrue("/w" in env_base["CCFLAGS"])
    def test_msvc_w_flag_added_CFLAGS(self):
        env_base = Environment()
        env_base.msvc = True
        env_base.__class__.disable_warnings = methods.disable_warnings
        env_base.disable_warnings()
        self.assertTrue(env_base.msvc)
        self.assertTrue("/w" in env_base["CFLAGS"])
    def test_msvc_w_flag_added_CXXFLAGS(self):
        env_base = Environment()
        env_base.msvc = True
        env_base.__class__.disable_warnings = methods.disable_warnings
        env_base.disable_warnings()
        self.assertTrue(env_base.msvc)
        self.assertTrue("/w" in env_base["CXXFLAGS"])
    def test_msvc_warn_flags_removed_CCFLAGS(self):
        env_base = Environment()
        env_base.msvc = True
        env_base["CCFLAGS"] = ["/Wall", "/W4", "/W3", "/W2", "/W1", "/WX"]
        env_base.__class__.disable_warnings = methods.disable_warnings
        env_base.disable_warnings()
        self.assertEqual(env_base["CCFLAGS"],['/w'])
    def test_msvc_warn_flags_removed_CFLAGS(self):
        env_base = Environment()
        env_base.msvc = True
        env_base["CFLAGS"] = ["/Wall", "/W4", "/W3", "/W2", "/W1", "/WX"]
        env_base.__class__.disable_warnings = methods.disable_warnings
        env_base.disable_warnings()
        self.assertEqual(env_base["CFLAGS"],['/w'])
    def test_msvc_warn_flags_removed_CXXFLAGS(self):
        env_base = Environment()
        env_base.msvc = True
        env_base["CXXFLAGS"] = ["/Wall", "/W4", "/W3", "/W2", "/W1", "/WX"]
        env_base.__class__.disable_warnings = methods.disable_warnings
        env_base.disable_warnings()
        self.assertEqual(env_base["CXXFLAGS"],['/w'])
    def test_not_msvc_build(self):
        env_base = Environment()
        env_base.msvc = False
        env_base["CFLAGS"] = ["/Wall"]
        env_base["CXXFLAGS"] = ["/Wall", "/W4", "/W3", "/W2", "/W1", "/WX"]
        env_base.__class__.disable_warnings = methods.disable_warnings 
        env_base.disable_warnings()
        self.assertEqual(env_base["CCFLAGS"], ["/nologo","-w"])
        self.assertEqual(env_base["CFLAGS"], ["/Wall"] + ["-w"])
        self.assertEqual(env_base["CXXFLAGS"], ["/Wall", "/W4", "/W3", "/W2", "/W1", "/WX"] + ["-w"])
        
class test_add_module_version_string(unittest.TestCase):
    def test_add_string(self):
         env_base = Environment()
         env_base.module_version_string = "This is a Test "
         env_base.__class__.add_module_version_string = methods.add_module_version_string
         env_base.add_module_version_string("OK")
         self.assertEqual(env_base.module_version_string, "This is a Test .OK")