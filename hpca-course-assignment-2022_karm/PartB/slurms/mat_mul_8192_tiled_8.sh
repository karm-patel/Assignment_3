#!/bin/sh
#SBATCH --job-name=mat_mul_8192_tiled
#SBATCH --output=results/mat_mul_8192_tiled_8
./objects/mat_mul_8192_tiled_8.o data/input_8192.in
nvidia-smi