#!/bin/sh
#SBATCH --job-name=mat_mul_1024_tiled
#SBATCH --output=results/iter_4/mat_mul_1024_tiled_32
./objects/mat_mul_1024_tiled_32.o data/input_1024.in
nvidia-smi