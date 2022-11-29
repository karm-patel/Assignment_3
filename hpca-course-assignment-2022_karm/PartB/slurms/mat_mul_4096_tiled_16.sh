#!/bin/sh
#SBATCH --job-name=mat_mul_4096_tiled
#SBATCH --output=results/iter_9/mat_mul_4096_tiled_16
./objects/mat_mul_4096_tiled_16.o data/input_4096.in
nvidia-smi