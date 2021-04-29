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

    def test_get_git_folder(self):
        folder = methods.get_git_folder()
        self.assertEqual(folder, ".git")

    @patch('os.path.isfile')
    def test_get_git_folder(self, mock_is_file): 
        mock_is_file.side_effect = [True]
        with patch('builtins.open', unittest.mock.mock_open(read_data='gitdir: .gitfolder')) as m:
            folder = methods.get_git_folder()
        self.assertEqual(folder, ".gitfolder")

    @patch('methods.get_git_folder')
    @patch('methods.read_first_line')
    @patch('os.path.isfile')
    def test_get_version_hash_ref(self, mock_is_file, mock_first_line, mock_get_git_folder):
        mock_is_file.side_effect = [True, True]
        mock_first_line.side_effect = ["ref: fake/ref/directory", "fakeHash"]
        hash = methods.get_version_hash()
        self.assertEqual(hash, "fakeHash")

    @patch('methods.get_git_folder')
    @patch('methods.read_first_line')
    @patch('os.path.isfile')
    def test_get_version_hash_ref(self, mock_is_file, mock_first_line, mock_get_git_folder):
        mock_is_file.side_effect = [True]
        mock_first_line.side_effect = ["fake/ref/directory"]
        hash = methods.get_version_hash()
        self.assertEqual(hash, "fake/ref/directory")

    @patch('methods.get_git_folder')
    @patch('os.path.isfile')
    def test_get_version_hash_ref(self, mock_is_file, mock_get_git_folder):
        mock_is_file.side_effect = [False]
        hash = methods.get_version_hash()
        self.assertEqual(hash, "")

    @patch('methods.get_git_folder')
    @patch('methods.read_first_line')
    @patch('os.path.isfile')
    def test_get_version_hash_hash_file_not_exists(self, mock_is_file, mock_first_line, mock_get_git_folder):
        mock_is_file.side_effect = [True, False]
        mock_first_line.side_effect = ["ref: fake/ref/directory"]
        hash = methods.get_version_hash()
        self.assertEqual(hash, "")

    def test_read_first_line(self):
        with patch('builtins.open', unittest.mock.mock_open(read_data='gitdir: .gitfolder')) as m:
            head = methods.read_first_line("test_path")
            self.assertEqual(head,'gitdir: .gitfolder')

    @patch('builtins.print')
    @patch('os.getenv')
    def test_custom_build_name(self, mock_get_env, mock_print):
        custom_name = "CustomeBuildName"
        mock_get_env.side_effect = [custom_name]
        build_name = methods.get_build_name()
        mock_print.assert_called_with("Using custom build name: " + custom_name)
        self.assertEqual(build_name, custom_name)

    @patch('os.getenv')
    def test_custom_build_name(self, mock_get_env):
        mock_get_env.side_effect = [None]
        build_name = methods.get_build_name()
        self.assertEqual(build_name, "custom_build")
    @patch('methods.get_build_name')
    def test_write_version_file(self, mock_get_build_name):
        mock_get_build_name.side_effect = ["test_name"]
        version_name = "test_version"
        import version
        with patch('builtins.open', unittest.mock.mock_open()) as m:
            methods.write_version_file(version_name)

        m.assert_called_with("core/version_generated.gen.h", "w")
        
        handler = m()
        calls = [call("/* THIS FILE IS GENERATED DO NOT EDIT */\n"),
                 call("#ifndef VERSION_GENERATED_GEN_H\n"),
                 call("#define VERSION_GENERATED_GEN_H\n"),
                 call('#define VERSION_SHORT_NAME "' + str(version.short_name) + '"\n'),
                 call('#define VERSION_NAME "' + str(version.name) + '"\n'),
                 call("#define VERSION_MAJOR " + str(version.major) + "\n"),
                 call("#define VERSION_MINOR " + str(version.minor) + "\n"),
                 call("#define VERSION_PATCH " + str(version.patch) + "\n"),
                 call('#define VERSION_STATUS "' + str(version.status) + '"\n'),
                 call('#define VERSION_BUILD "' + "test_name" + '"\n'),
                 call('#define VERSION_MODULE_CONFIG "' + str(version.module_config) + version_name + '"\n'),
                 call("#define VERSION_YEAR " + str(version.year) + "\n"),
                 call('#define VERSION_WEBSITE "' + str(version.website) + '"\n'),
                 call("#endif // VERSION_GENERATED_GEN_H\n")]
        handler.close.assert_called_once()
        handler.write.assert_has_calls(calls)
        

        

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