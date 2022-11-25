#!/bin/sh
#SBATCH --job-name=mat_mul_1024_naive
#SBATCH --output=results/mat_mul_1024_naive_
./objects/mat_mul_1024_naive_.o data/input_1024.in
nvidia-smi