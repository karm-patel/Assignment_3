#!/bin/sh
#SBATCH --job-name=mat_mul
#SBATCH --output=mat_mul_16384
nvcc main.cu -o cmm_server -I ./header
./cmm_server data/input_16384.in
nvidia-smi