#!/bin/sh
#SBATCH --job-name=mat_mul_4096_tiled
#SBATCH --output=results/iter_9/mat_mul_4096_tiled_8
./objects/mat_mul_4096_tiled_8.o data/input_4096.in
nvidia-smi