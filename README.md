# DSP-Filters
"C++ implementation of low-pass, high-pass, and band-pass digital filters. The code also computes and plots the frequency response of each filter using GNU plot. A basic intro to Digital Signal Processing."

# Signal Filtering

This repository contains a C++ program for low-pass, high-pass, and band-pass filtering of signals. The program also calculates and displays the frequency response of each filter.

The code uses the GNU plot library to create graphs of the original and filtered signals and their frequency responses.

## Requirements

The code requires the GNU plot library. You can download it from [here](http://www.gnuplot.info/download.html).

## Installation

To clone the repository, open a terminal and run:

```bash
git clone [https://github.com/<Your GitHub Username>/signal-filtering.git](https://github.com/SoroushJamali/DSP-Filters.git)
```
# Usage

Compile the program with g++:
```bash
g++ main.cpp -o main
```
# Run the program:
```bash
./main
```
# Outputs
The code creates four images representing the original and filtered signals and their frequency responses:

all.jpg: Shows the original signal and the output of the low-pass, high-pass, and band-pass filters.

image/all.jpg

LP.jpg: Shows the frequency response of the low-pass filter.

LP.jpg

HP.jpg: Shows the frequency response of the high-pass filter.

HP.jpg

BP.jpg: Shows the frequency response of the band-pass filter.

BP.jpg
