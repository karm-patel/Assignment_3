#!/bin/sh
#SBATCH --job-name=mat_mul_2048_tiled
#SBATCH --output=results/iter_9/mat_mul_2048_tiled_8
./objects/mat_mul_2048_tiled_8.o data/input_2048.in
nvidia-smi