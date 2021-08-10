make
./ospf 0 input1.txt outp1.txt 1 5 20 &
./ospf 1 input1.txt outp2.txt 1 5 20 &
./ospf 2 input1.txt outp3.txt 1 5 20 &
./ospf 3 input1.txt outp4.txt 1 5 20 &
./ospf 4 input1.txt outp5.txt 1 5 20 &
./ospf 5 input1.txt outp6.txt 1 5 20 &
./ospf 6 input1.txt outp7.txt 1 5 20 &
./ospf 7 input1.txt outp8.txt 1 5 20
sleep 5

make clean
