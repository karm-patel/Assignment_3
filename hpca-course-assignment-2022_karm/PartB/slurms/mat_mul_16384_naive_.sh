#!/bin/sh
#SBATCH --job-name=mat_mul_16384_naive
#SBATCH --output=results/mat_mul_16384_naive_
./objects/mat_mul_16384_naive_.o data/input_16384.in
nvidia-smi