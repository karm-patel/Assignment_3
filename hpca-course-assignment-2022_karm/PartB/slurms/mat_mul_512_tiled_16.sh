#!/bin/sh
#SBATCH --job-name=mat_mul_512_tiled
#SBATCH --output=results/mat_mul_512_tiled_16
./objects/mat_mul_512_tiled_16.o data/input_512.in
nvidia-smi