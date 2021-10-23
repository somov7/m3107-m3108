import random

random.seed()
tests_num = 5000
max_value = 2 ** 1024 - 1


def sub_test_gen():
    test_file = open("test_sub.data", "w")
    i = 0
    while i < tests_num:
        a, b = get_rand_nums()
        a, b = max(a, b), min(a, b)
        test = str(a) + "_" + str(b) + "_" + str(a - b) + "_ end\n"
        test_file.write(test)
        i += 1
    test_file.close()


def add_test_gen():
    test_file = open("test_add.data", "w")

    i = 0
    while i < tests_num:
        a, b = get_rand_nums()
        if a + b > max_value:
            continue
        test = str(a) + "_" + str(b) + "_" + str(a + b) + "_ end\n"
        test_file.write(test)
        i += 1
    test_file.close()


def mul_test_gen():
    test_file = open("test_mul.data", "w")
    i = 0
    while i < tests_num:
        a, b = get_rand_nums()
        if a * b > max_value:
            continue
        test = str(a) + "_" + str(b) + "_" + str(a * b) + "_ end\n"
        test_file.write(test)
        i += 1
    test_file.close()


def get_rand_nums():
    rand_pow = random.randint(0, 1024)
    val1 = 2 ** rand_pow
    rand_pow = random.randint(0, 1024)
    val2 = 2 ** rand_pow
    if val1 == max_value:
        val1 -= 1
    if val2 == max_value:
        val2 -= 1

    a = random.randint(0, val1)
    b = random.randint(0, val2)
    return a, b


add_test_gen()
sub_test_gen()
mul_test_gen()
