echo ""
echo ""
echo "--- INICIO ---"
mpicc combineArrays.c -o exe
time mpiexec -n 4 ./exe
echo "--- FIN ---"
rm ./exe