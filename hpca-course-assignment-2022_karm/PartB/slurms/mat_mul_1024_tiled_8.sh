#!/bin/sh
#SBATCH --job-name=mat_mul_1024_tiled
#SBATCH --output=results/mat_mul_1024_tiled_8
./objects/mat_mul_1024_tiled_8.o data/input_1024.in
nvidia-smi