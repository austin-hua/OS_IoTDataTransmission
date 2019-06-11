if [ $1 = m ]
then
	method=m
else 
	method=f
fi
echo "method used: $method" >&2
for input_file in `find ./data -name *.in`
do
	sudo ./user_program/master $input_file $method &
		sudo ./user_program/slave $input_file.out $method `hostname -i`
done;