#!/bin/sh
#SBATCH --job-name=mat_mul_8192_tiled
#SBATCH --output=results/iter_4/mat_mul_8192_tiled_16
./objects/mat_mul_8192_tiled_16.o data/input_8192.in
nvidia-smi