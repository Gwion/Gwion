title() {
  TMP=$(head -n 1 $1)
  echo ${TMP:2}
}

list_dir() {
  for a in $1/*
  do
   if [ -d "$a" ]
   then echo "$2- '$(basename $a | sed 's/[0-9][0-9]_//' | sed 's/_/ /g' | sed 's/-/ /g')':"; list_dir $a "$2  "
   else echo "$a" | grep "\.md$" >/dev/null  && echo "$2- $(title $a): ${a:5}"
		fi
  done
}

echo "site_name: Gwion"
echo "theme: windmill"
echo "repo_url: https://github.com/fennecdjay/Gwion"
echo "nav:"
echo "  - 'Welcome' : index.md"
mv docs/index.md .
list_dir docs "  "
mv index.md docs
