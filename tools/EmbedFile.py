import sys

path, varname, outpath = sys.argv[1], sys.argv[2], sys.argv[3]

with open(path, "rb") as f:
    data = f.read()

with open(outpath, "w") as f:
    f.write(f"static const unsigned char {varname}[] = {{\n")
    f.write(",".join(str(b) for b in data))
    f.write(f"\n}};\nstatic const unsigned int {varname}_len = {len(data)};\n")
