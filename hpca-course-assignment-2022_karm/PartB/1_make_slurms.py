mat_sizes = [512,1024,2048,4096,8192,16384]
tile_sizes = [8,16,32]

op_dir = "slurms/"

def slurm_content(mat_size, kernel, tile_size): 
    return f'''#!/bin/sh
#SBATCH --job-name=mat_mul_{mat_size}_{kernel}
#SBATCH --output=results/iter_0/mat_mul_{mat_size}_{kernel}_{tile_size}
./objects/mat_mul_{mat_size}_{kernel}_{tile_size}.o data/input_{mat_size}.in
nvidia-smi'''

for tile_size in tile_sizes:
    for mat_size in mat_sizes:
        slurm_c = slurm_content(mat_size, "tiled", tile_size)
        with open(op_dir + f"mat_mul_{mat_size}_tiled_{tile_size}.sh", "w") as fp:
            fp.write(slurm_c)

for mat_size in mat_sizes:
    slurm_c = slurm_content(mat_size, "naive", "")
    with open(op_dir + f"mat_mul_{mat_size}_naive_.sh", "w") as fp:
        fp.write(slurm_c)




