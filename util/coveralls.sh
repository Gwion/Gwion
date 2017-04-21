#!/bin/bash
NORMAL=$(tput sgr0)
YELLOW=$(tput setaf 3)
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)

curl -s 'https://coveralls.io/github/fennecdjay/Gwion.json' | xargs | tr ',' '\n' | sed "s/{//" | sed "s/}//" |
	sed "s/ : +\(.*\)$/:$GREEN-\1$NORMAL/" |
	sed "s/ : -\(.*\)$/:$RED-\1$NORMAL/" |
	sed "s,^\(.*\):,$YELLOW\1:$NORMAL,"
