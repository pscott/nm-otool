import subprocess
import sys
import os
from termcolor import colored

def write_to_file(filename, data):
    with open(filename, 'w') as f:
        f.write(data)

def close_file(filename):
    try:
        os.remove(filename)
    except:
        print("could not remove file {}.".format([filename]))

def test(errno, stdout_expected, stderr_expected, filename):
    try:
        output = subprocess.check_output(["./nm", filename], timeout=3, stderr=subprocess.PIPE).decode("utf-8")
        assert(stdout_expected == output), "Expected: success stdout `{}`, got: `{}`".format(stdout_expected, output)
    except subprocess.CalledProcessError as exc:
        returncode = exc.returncode
        assert(errno == exc.returncode), "Expected errno `{}`, got: `{}`".format(errno, returncode)
        stdout = exc.output.decode(sys.getfilesystemencoding())
        assert(stdout_expected == stdout), "Expected stdout `{}`, got: `{}`".format(stdout_expected, stdout)
        stderr = exc.stderr.decode(sys.getfilesystemencoding())
        assert(stderr_expected == stderr), "Expected stderr `{}`, got: `{}`".format(stderr_expected, stderr)
    print(colored("OK\t\t" + filename, 'green'))

def test_new_file(data, errno, stdout_expected, stderr_expected, filename):
    write_to_file(filename, data)
    test(errno, stdout_expected, stderr_expected, filename)
    close_file(filename)