from utils import test_new_file, modify_template_file, VALID_ELF64, VALID_ELF32

print("\t--MAGIC--")
invalid_class = modify_template_file(VALID_ELF64, 4, "\x04")

test_new_file(invalid_class, 1, "", "Unsupported ELF format\n", "invalid_class")
invalid_elf = modify_template_file(VALID_ELF64, 1, "\x44")
test_new_file("", 1, "", "nm: error while trying to map file 'empty_file'\n", "empty_file")
print("")
