from utils import test_new_file

print("\t--MAGIC--")
test_new_file("\x7f\x45\x4c\x46\x04", 1, "", "File class '4' not supported\n", "class_4_not_supported")
test_new_file("\x7f\x45\x4c\x46\x02", 0, "", "", "valid_elf64")
test_new_file("\x7f\x45\x4c\x46\x01", 0, "", "", "valid_elf32")
print("")
