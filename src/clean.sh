cd ./master_device
make clean
cd ../slave_device
make clean
cd ../ksocket
make clean
cd ../user_program
make clean
cd ..
sudo sh removeModules.sh
echo "Finished executing clean.sh" >&2

