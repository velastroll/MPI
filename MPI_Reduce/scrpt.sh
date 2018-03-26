echo ""
echo ""
echo "--- INICIO ---"
mpicc SumOfZero_mod.c -o exe
time mpiexec -n 5 ./exe 12000
echo "--- FIN ---"
rm ./exe