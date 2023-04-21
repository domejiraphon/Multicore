gcc -O3 -std=c99 -Wall -fopenmp -o genprime genprimes.c -lm
for i in 1 2 5 10 25 50 100
do
  echo "=====================genprime=1000=$i====================================="
  ./genprime 1000 $i
done
echo -e "\n\n"
for i in 1 2 5 10 25 50 100
do
  echo "=====================genprime=1000000=$i====================================="
  ./genprime 1000000 $i
done

echo -e "\n\n"
for i in 1 2 5 10 25 50 100
do
  echo "=====================genprime=100000000=$i====================================="
  ./genprime 100000000 $i
done