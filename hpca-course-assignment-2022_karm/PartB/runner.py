ip_file = "header/gpu_thread.h"

mat_sizes = [512,1024,2048,4096,8192,16384]
tile_sizes = [8,16,32]
kernels = ["tiled", "naive"]

def make_command(kernel, mat_size, tile_size):
    if kernel == "tiled":
        slurm_file = f"slurms/mat_mul_{mat_size}_tiled_{tile_size}.sh" 
    else:
        slurm_file = f"slurms/mat_mul_{mat_size}_naive_"
    
    command = f"sbatch {slurm_file}"
    return command


commands = []
if "tiled" in kernels:
    for tile_size in tile_sizes:
        for mat_size in mat_sizes:
            commands.append(make_command("tiled", mat_size, tile_size))
             

if "naive" in kernels:
    for mat_size in mat_sizes:
        commands.append(make_command("naive", mat_size, tile_size))
    
print(commands, len(commands))

        
        


