cd master_device
sudo make clean
cd ../slave_device
sudo make clean
cd ../ksocket
sudo make clean
cd ../user_program
sudo make clean
cd ..
echo "Finished executing clean.sh" >&2

