import sys

def parse_file(fpath):
    histogram = {}
    with open(fpath, 'r') as f:
        for line in f:
            if line in histogram:
                histogram[line] += 1
            else:
                histogram[line] = 1

    return histogram


if __name__ == "__main__":
    if len(sys.argv[:]) != 3:
        sys.stderr.write('Usage: {} FILE1 FILE2\n'.format(sys.argv[0]))
        sys.exit(1)

    file1 = parse_file(sys.argv[1])
    file2 = parse_file(sys.argv[2])

    if file1 != file2:
        print("Compare outputs: Error! Files are not same!")
        sys.exit(1)

    print("Compare outputs: Ok")
    sys.exit(0)
