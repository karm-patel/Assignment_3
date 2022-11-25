#!/bin/sh
#SBATCH --job-name=mat_mul_1024_tiled
#SBATCH --output=results/iter_5/mat_mul_1024_tiled_16
./objects/mat_mul_1024_tiled_16.o data/input_1024.in
nvidia-smi