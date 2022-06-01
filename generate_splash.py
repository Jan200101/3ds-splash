"""
generate a splash.bin to test again
"""
import sys

COLORS = [
    [b"\x00", b"\x00", b"\x00"],
    [b"\xFF", b"\x00", b"\x00"],
    [b"\xFF", b"\xFF", b"\x00"],
    [b"\x00", b"\xFF", b"\x00"],
    [b"\x00", b"\xFF", b"\xFF"],
    [b"\x00", b"\x00", b"\xFF"],
    [b"\xFF", b"\x00", b"\xFF"],
]

def main():
    if len(sys.argv) < 2:
        print("{} [output]".format(sys.argv[0]))
        return

    output = sys.argv[1]

    height = 400
    width = 240
    with open(output, "wb") as fd:
        for h in range(height):
            for w in range(width):
                i = w % len(COLORS)

                for x in COLORS[i]:
                    fd.write(x)


if __name__ == "__main__":
    main()