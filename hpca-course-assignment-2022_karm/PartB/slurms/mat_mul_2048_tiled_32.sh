#!/bin/sh
#SBATCH --job-name=mat_mul_2048_tiled
#SBATCH --output=results/iter_3/mat_mul_2048_tiled_32
./objects/mat_mul_2048_tiled_32.o data/input_2048.in
nvidia-smi