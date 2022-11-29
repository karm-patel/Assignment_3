import regex as re
import os
ip_file = "header/gpu_thread.h"
with open(ip_file, 'r') as fp:
    content = fp.read()


if not os.path.isdir("objects"):
    os.system("mkdir objects")

mat_sizes = [512,1024,2048,4096,8192,16384]
tile_sizes = [8,16,32]
kernels = ["tiled", "naive"]

if "tiled" in kernels:
    for tile_size in tile_sizes:
        for mat_size in mat_sizes:
            content = re.sub(r"int kernel = [0-9];", f"int kernel = {1};", content)
            content = re.sub(r"int TILE = [0-9]*;", f"int TILE = {tile_size};", content)
            with open(ip_file, 'w') as fp:
                fp.write(content)
            
            cmd = f"nvcc main.cu -o objects/mat_mul_{mat_size}_tiled_{tile_size}.o -I ./header"
            print("executing... " + cmd)
            os.system(cmd)
        #     break
        # break


if "naive" in kernels:
    content = re.sub(r"int kernel = [0-9];", f"int kernel = {0};", content)
    with open(ip_file, 'w') as fp:
        fp.write(content)
    for mat_size in mat_sizes:
        cmd = f"nvcc main.cu -o objects/mat_mul_{mat_size}_naive_.o -I ./header"
        print("executing... " + cmd)
        os.system(cmd)