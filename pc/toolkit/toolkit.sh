#!/bin/bash
rm context.csv output.csv  plot_after.png  plot_before.png  stream.csv
./converter
./context
./calc
./plot_after
./plot_before
