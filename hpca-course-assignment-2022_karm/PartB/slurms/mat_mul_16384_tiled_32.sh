#!/bin/sh
#SBATCH --job-name=mat_mul_16384_tiled
#SBATCH --output=results/iter_4/mat_mul_16384_tiled_32
./objects/mat_mul_16384_tiled_32.o data/input_16384.in
nvidia-smi