make clean && clear; make test

echo "OK files"
./test_parser projSapotacheFormat.txt
./test_parser sapo1.ok.txt
./test_parser sapo2.ok.txt
./test_parser sapo3.ok.txt

echo "Corrupted files"
./test_parser sapo1.err.txt
./test_parser sapo2.err.txt
./test_parser sapo3.err.txt
./test_parser sapo4.err.txt
./test_parser sapo5.err.txt
./test_parser sapo6.err.txt
./test_parser sapo7.err.txt
./test_parser sapo8.err.txt
