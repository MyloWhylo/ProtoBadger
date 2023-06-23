#!/usr/bin/env python3

import math
import numpy as np
import argparse
from PIL import Image, ImageOps
import rle
import re

# I hate command line arguments, so here we are
parser = argparse.ArgumentParser(
    description='Converts images into the format used by Badger2040.')
parser.add_argument('file', nargs="+", help='input files to convert')
parser.add_argument('--resize', action="store_true",
                    help='force images to 296x128 pixels')
parser.add_argument('--verbose', action="store_true",
                    help='Enables verbose logging')

options = parser.parse_args()

# Line width of .hpp file (so it looks pretty idk)
lineWidth = 32
bitDepth = 4

def cleanString(name): return re.sub(r'\W+|^(?=\d)','_', name)

def writeImage(outFile, values, counts, mono, name):
    if not mono:
        outFile.write(f'const uint8_t {name}Values[{len(values)}] = ')
        writeArray(outFile, values)

    outFile.write(f'\nconst uint8_t {name}Counts[{len(counts)}] = ')
    writeArray(outFile, counts)

    outFile.write(
        f'\nconst MyloImage {name} = {"{"} {len(counts)}, {name}Counts, {"NULL" if mono else name + "Values"}, {"true" if mono else "false"} {"}"};\n\n')


def writeArray(outFile, inData):
    global lineWidth
    numBytes = len(inData)

    outFile.write("{\n")
    numLines = math.floor(numBytes / lineWidth)
    leftover = numBytes % lineWidth

    for startByte in range(0, numBytes - leftover, lineWidth):
        for index, byte in enumerate(inData[startByte:startByte+lineWidth]):
            if index % lineWidth == lineWidth - 1:
                outFile.write(f'{byte},\n')
            else:
                outFile.write(f'{byte}, ')

    for index, byte in enumerate(inData[numLines*lineWidth:]):
        if index == leftover - 1:
            outFile.write(f'{byte}')
        else:
            outFile.write(f'{byte}, ')

    outFile.write("};\n")


# Let's get down to business
with open("image.hpp", "w") as outFile:
    compressedSizes = 0
    originalSizes = 0
    varNames = []

    outFile.write('#include "MyloBadger.hpp"\n\n')
    print("Beginning image conversion.")
    for file in options.file:
        with Image.open(file) as im:
            cleanName = cleanString(((file.split(".")[0]).split("_")[0]).split("-")[0])
            varNames.append(cleanName)

            if options.verbose: print(f'Opened {file}.')
            if options.resize:
                if options.verbose: print("Resizing image...\t", end="", flush=True)
                im = im.resize((296, 128))
                if options.verbose: print("Resized!")

            if options.verbose: print('Finding splitpoints...\t', end="", flush=True)

            # Dithering is bad, as it makes RLE less efficient.
            # Although, dither seems to not actually do anything.
            im = im.quantize(dither=Image.Dither.NONE, colors=4)
            im = im.convert(mode="L")

            im = ImageOps.invert(im)
            histogram = im.histogram()
            values = [ii for ii in range(
                len(histogram)) if histogram[ii] != 0]

            if options.verbose: print("Split!")
            if options.verbose: print("Compressing with RLE...\t", end="", flush=True)

            all = rle.encode(np.array(im).flatten().tolist())
            all = np.array(all)

            finalCounts = []
            if len(all) == 2:
                for currentCounts in all[1]:
                    while currentCounts != 0:
                        if (currentCounts >= 255):
                            finalCounts.append(255)
                            currentCounts -= 255
                        else:
                            finalCounts.append(currentCounts)
                            currentCounts = 0;
                compressedSizes += (len(finalCounts)) + 2 + 1
                writeImage(outFile, None, finalCounts, True, cleanName)
            else:
                bitCompressed = bytearray()
                currentByte = 0
                numProcessed = 0

                for ii, value in enumerate(np.array(all[0]).flatten().tolist()):
                    currentCounts = all[1][ii]
                    while currentCounts != 0:
                        if (currentCounts >= 255):
                            finalCounts.append(255)
                            currentCounts -= 255
                        else:
                            finalCounts.append(currentCounts)
                            currentCounts = 0;

                        currentByte = currentByte << 2
                        for ii, testValue in enumerate(values):
                            if value == testValue:
                                currentByte |= ii

                        numProcessed += 1
                        if numProcessed >= 4:
                            bitCompressed.append(currentByte)
                            numProcessed = 0
                            currentByte = 0

                if numProcessed != 0:
                    while numProcessed < 4:
                        currentByte = currentByte << 2
                        numProcessed += 1

                    bitCompressed.append(currentByte)

                # bitCompressed is 8 bit, lengths are 8 bit, 16 bits for the length of the arrays, 8 bits for boolean
                compressedSizes += len(bitCompressed) + (len(finalCounts)) + 2 + 1
                writeImage(outFile, bitCompressed, finalCounts, False, cleanName)

            # Width * Height * 3 image planes / 8 bits per byte
            originalSizes += int((im.size[0] * im.size[1] * 3) / 8)
            if options.verbose: print("Compressed!")

            if options.verbose: print()

    if not options.verbose: print()
    print(f'Done! Images take up {compressedSizes} bytes, down from {originalSizes} ({round(100 - ((compressedSizes / originalSizes) * 100), 2)}% smaller).')
    print(f'This means you can store {round(originalSizes / compressedSizes, 2)} times more photos than normal.')
    print("\nVariable names:")
    for name in varNames:
        print(f'\t{name}')
