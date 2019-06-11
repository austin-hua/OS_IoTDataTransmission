if [[ $1 = f ]]
then
	func = f
else 
	func = m
fi

addr=127.0.0.1

for input_file in *.in; do
	sudo ./user_program/master $input_file $func &
		sudo ./user_program/slave $input_file.out $func addr
done;