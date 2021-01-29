from utils import test

print("\t--IO--")
filename = "src"
test(1, "", "nm: error while trying to map file '{}'\n".format(filename), filename)
filename = "/dev/null"
test(1, "", "nm: error while trying to map file '{}'\n".format(filename), filename)
filename = "doest_not_exist"
test(1, "", "nm: error while trying to open '{}'\n".format(filename), filename)
print("")