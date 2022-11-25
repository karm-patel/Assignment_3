#!/bin/sh
#SBATCH --job-name=mat_mul_16384
#SBATCH --output=results/mat_mul_16384_naive
nvcc main.cu -o cmm_server -I ./header
./cmm_server data/input_16384.in
nvidia-smi