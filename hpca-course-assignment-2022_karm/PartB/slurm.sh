#!/bin/sh
#SBATCH --job-name=mat_mul
#SBATCH --output=results/mat_mul_4096_tiled_128
nvcc main.cu -o cmm_server -I ./header
./cmm_server data/input_4096.in
nvidia-smi