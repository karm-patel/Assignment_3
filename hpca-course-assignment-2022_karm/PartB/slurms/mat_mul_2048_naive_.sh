#!/bin/sh
#SBATCH --job-name=mat_mul_2048_naive
#SBATCH --output=results/iter_9/mat_mul_2048_naive_
./objects/mat_mul_2048_naive_.o data/input_2048.in
nvidia-smi