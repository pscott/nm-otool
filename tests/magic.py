from utils import test_new_file

print("\t--MAGIC--")
test_new_file("\x7f\x45\x4c\x46\x04", 1, "", "File class '4' not supported\n", "class_4_not_supported")
test_new_file("\x7f\x45\x4c\x46\x02", 0, "", "", "valid_elf64")
test_new_file("\x7f\x45\x4c\x46\x01", 0, "", "", "valid_elf32")
test_new_file("\x7f\x44\x4c\x46\x01", 1, "", "error: file is not ELF\n", "invalid_elf64")
test_new_file("\x7f\x45\x4c\x48\x01", 1, "", "error: file is not ELF\n", "invalid_elf32")
test_new_file("", 1, "", "nm: error while trying to map file 'empty_file'\n", "empty_file")
test_new_file("\x7f\x45\x4c\x46", 1, "", "Could not read class number\n", "four_bytes_file")
print("")
