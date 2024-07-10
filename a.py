import subprocess

for i in range(5,25):
    print(f"va en {i}")
    for j in range(1000):
        a=f"./example --jobs 500 --rate 5 --cajas {i} --qrate 10 --fail 50 --tipoa 3 --tipob 5".split()
        compile=subprocess.run(a,capture_output=True, text=True)

        print(str(compile.stdout).strip(),file=open(f"{i}-salida.txt",'+a'))