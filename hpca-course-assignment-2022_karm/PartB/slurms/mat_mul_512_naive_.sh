#!/bin/sh
#SBATCH --job-name=mat_mul_512_naive
#SBATCH --output=results/iter_3/mat_mul_512_naive_
./objects/mat_mul_512_naive_.o data/input_512.in
nvidia-smi