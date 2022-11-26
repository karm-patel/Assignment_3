#!/bin/sh
#SBATCH --job-name=mat_mul_8192_tiled
#SBATCH --output=results/iter_9/mat_mul_8192_tiled_32
./objects/mat_mul_8192_tiled_32.o data/input_8192.in
nvidia-smi