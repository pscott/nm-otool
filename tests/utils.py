import subprocess
import sys
import os
from termcolor import colored

def modify_template_file(file, offset, new_value):
    return file[0:offset] + new_value + file[offset + 1:]

VALID_ELF64 = "\x7f\x45\x4c\x46\x02\x01\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x3e\x00\x01\x00\x00\x00\xd0\x5c\x00\x00\x00\x00\x00\x00\x40\x00\x00\x00\x00\x00\x00\x00\x98x\f3\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x40\x00\x38\x00\x0d\x00\x40\x00\x1e\x00\x1d\x00"
VALID_ELF32 = modify_template_file(VALID_ELF64, 4, "\x01")

def write_to_file(filename, data):
    with open(filename, 'w') as f:
        f.write(data)

def delete_file(filename):
    try:
        os.remove(filename)
    except:
        print("could not remove file {}.".format([filename]))

'''
Checks that stdout and stderr are the equal to those passed in as parameters.
If the one passed in as a parameter is `None`, then the check is not done.
'''
def test(errno, stdout_expected, stderr_expected, filename):
    try:
        output = subprocess.check_output(["./nm", filename], timeout=3, stderr=subprocess.PIPE).decode("utf-8")
        if (stdout_expected) is not None:
            assert(stdout_expected == output), "Test: {} :: Expected: success stdout `{}`, got: `{}`".format(filename, stdout_expected, output)
    except subprocess.CalledProcessError as exc:
        if (stdout_expected) is not None:
            stdout = exc.output.decode(sys.getfilesystemencoding())
            assert(stdout_expected == stdout), "Test: {} :: Expected stdout `{}`, got: `{}`".format(filename, stdout_expected, stdout)
        if (stderr_expected) is not None:
            stderr = exc.stderr.decode(sys.getfilesystemencoding())
            assert(stderr_expected == stderr), "Test: {} :: Expected stderr `{}`, got: `{}`".format(filename, stderr_expected, stderr)
        returncode = exc.returncode
        assert(errno == exc.returncode), "Test: {} :: Expected errno `{}`, got: `{}`".format(filename, errno, returncode)
    print(colored("OK\t\t" + filename, 'green'))

def test_new_file(data, errno, stdout_expected, stderr_expected, filename):
    write_to_file(filename, data)
    try:
        test(errno, stdout_expected, stderr_expected, filename)
    except AssertionError as e:
        delete_file(filename)
        raise e
    delete_file(filename)