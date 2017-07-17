while read -r line
do
	grep import_class_begin <<< "$line"  > /dev/null && { echo "class $(echo $line |cut -d '"' -f2)"; continue; }
	grep dl_func_add_arg    <<< "$line"     > /dev/null && { echo arg; continue; }
done < "$1"
