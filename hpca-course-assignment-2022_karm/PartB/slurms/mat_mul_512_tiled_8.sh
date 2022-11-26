#!/bin/sh
#SBATCH --job-name=mat_mul_512_tiled
#SBATCH --output=results/iter_9/mat_mul_512_tiled_8
./objects/mat_mul_512_tiled_8.o data/input_512.in
nvidia-smi