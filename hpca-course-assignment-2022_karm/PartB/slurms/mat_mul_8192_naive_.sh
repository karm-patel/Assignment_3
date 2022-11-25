#!/bin/sh
#SBATCH --job-name=mat_mul_8192_naive
#SBATCH --output=results/iter_3/mat_mul_8192_naive_
./objects/mat_mul_8192_naive_.o data/input_8192.in
nvidia-smi