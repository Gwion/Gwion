#!/bin/sh
# deploy site on gh-pages

function post()
{
	if [ ls -d */ | grep "$1" ]
	then
		echo "first arg must be a directory, or 'run' or 'deploy'"
		return
	fi
	echo -e "---\nlayout:     page\ntitle:      $2" > $1/$2.html
	echo -e "categories: $1\n---\n" >> $1/$2.html
	nano $1/$2.html
}

function deploy()
{
	git add .
	git commit -am 'Yeah. Built from subdir'
	git push
	bundle exec jekyll b
	mv _site /tmp
	rm .jekyll-metadata
	git checkout gh-pages
	git rm -rf *
	rm -rf *
	mv /tmp/_site/* .
#	rmdir /tmp/_site
	git add .
	git commit -am 'Yeah. Built from subdir'
	git push
	git checkout site
}

on_int()
{
	sed -i '/base/s/^#//g' _config.yml #uncomment
	rm -rf _site
}
#run the size locally. access with localhost:4000
function run()
{
	trap on_int INT
	sed -i '/base/s/^/#/g' _config.yml #comment
	bundle exec jekyll s
}

# remove _site if interupted or ...
trap 'rm -rf _site; exit' INT
trap 'rm -rf _site; exit' QUIT
trap 'rm -rf _site; exit' TERM
trap 'rm -rf _site; exit' EXIT

function send_gist()
{
	echo $(gist -u $1 -f $2| sed "s/https:\/\/gist.github.com\///") >> known_gist
}

function examples()
{
	rm -rf examples Gwion-examples
	git checkout master -- examples
	mv examples Gwion-examples
	mkdir examples
	for ex in $(ls Gwion-examples/*.gw)
	do
		NAME=$(basename $ex .gw)
#		echo -e "---\nlayout: example\ntitle: example $NAME\ncategories: examples \n---\n<br>this page documents <b>$NAME.gw</b><br><p>" > examples/$NAME.html
		echo -e "---\nlayout: homepage\ntitle: example $NAME\ncategories: examples \n---\n<br>this page documents 
<b>$NAME.gw</b><br><p>" > examples/$NAME.html
		pygmentize -f html $ex >> examples/$NAME.html
	echo "</p>" >> examples/$NAME.html
	done
	echo '---
layout: homepage
title:	{{post.title}}
---
  <h1>Examples</h1>
  <ul class="posts">
{% for post in site.pages %}
	{% if post.categories == "examples" %}
		<li><span><a href="{{ site.baseurl }}{{ post.url }}">{{post.title}}</a></li>
	{% endif %}
{% endfor %}
  </ul>' > examples/index.html


	rm -r Gwion-examples
	echo "put example content, whith highligting"
	echo "you should be done."
}

function code()
{
	rm -f /tmp/file.gw
	nano /tmp/file.gw
	pygmentize -f html /tmp/file.gw | xclip -i
	echo "should be"
	pygmentize /tmp/file.gw
	rm -f /tmp/file.gw
}

# parse arguments
if [ "$#" -lt 1 ]
then
	echo "usage: $0 run/send_gist \$2 \$3/deploy/code/examples/post $1 $2/"
elif [ "${1}" = 'run' ]
then
	run
elif [ "${1}" = 'gist' ]
then
	send_gist $2 $3
elif [ "${1}" = 'deploy' ]
then
	deploy
elif [ "${1}" = 'code' ]
then
	code
elif [ "${1}" = 'examples' ]
then
	examples
else
	post $1 $2
fi
