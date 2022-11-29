import regex as re
n = 3

# for iter in range(n):

n_iter = 3
mat_sizes = [512,1024,2048,4096,8192,16384]
tile_sizes = [8,16,32]


mat_size = 512
kernel = "tiled"
tile_size = 32

for iter in range(n_iter):
    for mat_size in mat_sizes:
        for tile_size in tile_sizes:
            dir = f"results/iter_{iter}/"
            file = dir + f"mat_mul_{mat_size}_{kernel}_{tile_size}"
            with open(file, "r") as fp:
                print(file)
                content = fp.read()
                find = re.findall(r"GPU execution time: ([0-9]*.[0-9]*) ms", content)
                print(find)