#!/bin/sh
#SBATCH --job-name=mat_mul_4096_naive
#SBATCH --output=results/iter_4/mat_mul_4096_naive_
./objects/mat_mul_4096_naive_.o data/input_4096.in
nvidia-smi