ls
top
gcc -fopenmp omp_solver.c -o solver
$OMP_NUM_THREADS
OMP_NUM_THREADS
export OMP_NUM_THREADS=4
./solver 
ls
gcc -fopenmp omp_solver.c -o omp_solver
gcc -fopenmp solver.c -o solver

export OMP_NUM_THREADS=32
./solver 
./omp_solver
gcc -fopenmp omp_solver.c -o omp_solver
./omp_solver
gcc -fopenmp omp_solver.c -o omp_solver
./omp_solver
gcc -fopenmp omp_solver.c -o omp_solver
./omp_solver
gcc -fopenmp omp_solver_static.c -o omp_solver_static
gcc -fopenmp omp_solver_dynamic.c -o omp_solver_dynamic
gcc -fopenmp omp_solver_guided.c -o omp_solver_guided
ls
vim runner
ls
chmod runner 
chmod --help
chmod +x runner 
ls
./runner 
vim ./runner 
for i in 1..4; do echo $i; done
for i in 1:4; do echo $i; done
for i in 'seq 1 10'; do echo $i; done
for 'seq 1 10'; do echo $i; done
help for
seq 1 10
for i in 'seq 1 10'; do echo $i; done
for i in {seq 1 10}; do echo $i; done
for i in "seq 1 10"; do echo $i; done
for i in "seq 1 10"; do echo i; done
for i in "seq 1 10"; do echo $i; done
for i in (seq 1 10); do echo $i; done
for i in 'seq 1 10'; do echo $i; done
for i in $(seq 1 10); do echo $i; done
vim runner 
for i in $(seq 1 10); do echo $i; echo try; done
for i in $(seq 1 10); do echo $i echo try; done
for i in $(seq 1 10); do echo $i $i; done
./runner 
gcc -fopenmp omp_solver_static.c -o omp_solver_static
gcc -fopenmp omp_solver_dynamic.c -o omp_solver_dynamic
gcc -fopenmp omp_solver_guided.c -o omp_solver_guided
gcc -fopenmp omp_solver.c -o omp_solver
gcc -fopenmp solver.c -o solver
./runner 
gcc -fopenmp solver.c -o solver
ls
ls -a
vim runner
./runner
vim runner
./runner > 5000
vim 5000 
./runner > 5000_2
vim 5000_2 
./runner > 5000_3
vim runner
./runner > 500
vim 500
./runner > 5000_ave
./runner > 10000
top
ls
vim 10000 
