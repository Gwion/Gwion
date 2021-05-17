cat << EOC
## Gwion config for GNU nano
##

syntax "gwion" "\.gw"

formatter gwion-nano-fmt
linter gwion -g check
EOC

scope=0
while read -r line
do
#  echo "$line"
  comment="$(sed 's/ *#!.*//' <<< "$line")"
  if [ "$comment" != "$line" ]
  then continue
  fi
  if [ "$(grep operator <<< "$line")" ]
  then continue
  fi
  primitive="$(sed 's/^ *primitive *\([a-zA-Z][a-zA-Z0-9_]*\).*/\1/' <<< "$line")"
  if [ "$primitive" != "$line" ]
  then
    echo "color bold,mauve \"\<${primitive}\>\""
    continue
  fi
  class="$(sed 's/^ *class *\([a-zA-Z][a-zA-Z0-9_]*\).*/\1/' <<< "$line")"
  if [ "$class" != "$line" ]
  then
    echo "color mauve \"\<${class}\>\""
    scope=$((scope+1))
    continue
  fi
  struct="$(sed 's/^ *struct *\([a-zA-Z][a-zA-Z0-9_]*\).*/\1/' <<< "$line")"
  if [ "$struct" != "$line" ]
  then
    echo "color mauve \"\<${struct}\>\""
    scope=$((scope+1))
    continue
  fi
  enum="$(sed 's/^ *enum *\([a-zA-Z][a-zA-Z0-9_]*\).*/\1/' <<< "$line")"
  if [ "$enum" != "$line" ]
  then
    echo "color italic,mauve \"\<${enum}\>\""
    scope=$((scope+1))
    continue
  fi
  fun="$(sed 's/^ *fun *[a-zA-Z][a-zA-Z0-9_]* *\([a-zA-Z][a-zA-Z0-9_]*\).*/\1/' <<< "$line")"
  if [ "$fun" != "$line" ]
  then
    if [ $scope -eq 0 ]
    then echo "color mint \"\<${fun}\>\""
    else echo "color mint \".\<${fun}\>\""
    fi
    if [ "${line: -1}" != ";" ]
    then scope=$((scope+1))
    fi
    continue
  fi
  funptr="$(sed 's/^ *funptr *[a-zA-Z][a-zA-Z0-9_]* *\([a-zA-Z][a-zA-Z0-9_]*\).*/\1/' <<< "$line")"
  if [ "$funptr" != "$line" ]
  then
    echo "color italic,mint \"\<${funptr}\>\""
    continue
  fi
  typedef="$(sed 's/^ *typedef *[a-zA-Z][a-zA-Z0-9_]* *\([a-zA-Z][a-zA-Z0-9_]*\).*/\1/' <<< "$line")"
  if [ "$typedef" != "$line" ]
  then
    echo "color italic,mauve \"\<${typedef}\>\""
    continue
  fi
  ignoreconstspid="$(sed 's/^ *specialid const *@\([a-zA-Z_][a-zA-Z0-9_]*\).*/\1/' <<< "$line")"
  if [ "$ignoreconstspid" != "$line" ]
  then
    continue
  fi
  ignorespid="$(sed 's/^ *specialid *@\([a-zA-Z_][a-zA-Z0-9_]*\).*/\1/' <<< "$line")"
  if [ "$ignorespid" != "$line" ]
  then
    continue
  fi
  constspid="$(sed 's/^ *specialid *const *\([a-zA-Z_][a-zA-Z0-9_]*\).*/\1/' <<< "$line")"
  if [ "$constspid" != "$line" ]
  then
    echo "color italic,lightyellow \"\<${constspid}\>\""
    continue
  fi
  spid="$(sed 's/^ *specialid *\([a-zA-Z_][a-zA-Z0-9_]*\).*/\1/' <<< "$line")"
  if [ "$spid" != "$line" ]
  then
    echo "color lightyellow \"\<${spid}\>\""
    continue
  fi
  lbrace="$(sed 's/^ *{.*$/@smth/' <<< "$line")"
  if [ "$lbrace" != "$line" ]
  then
    scope=$((scope+1))
    continue
  fi
  rbrace="$(sed 's/^ *}.*$/@smth/' <<< "$line")"
  if [ "$lbrace" != "$line" ]
  then scope=$((scope-1))
    continue
  fi
# misses items rn
done < $1

cat << EOC
comment "#!"

color italic,red start="#\<(pragma|include|define|ifdef|ifndef|undef|endif|require)\>" end="\$"

color brightred "\<(typeof|spork|fork)\>"
color lightred "\<(second|ms|day|samp|minute|hour)\>"
color brightcyan "\<(maybe)\>"
color cyan "\<(adc|blackhole)\>"

color brightgreen "\<(try|handle|perform|retry|class|struct|trait|union|enum|fun|operator|funptr|typedef|distinct)\>"
color italic,yellow "extends"
color italic,brightgreen "\<(final|abstract)\>"
color lightgreen "@[a-zA-Z]+[a-zA-Z0-9_]*|&[a-zA-Z]+[a-zA-Z0-9_]*"
color green "\<(static|global)\>"
color italic,green "\<(protected|private)\>"
color bold,green "\<(var|late|const)\>"
color bold,italic,green "\<(const)\>"

color bold,yellow "\<(for|until|if|while|do|else|match|case|when|where|defer)\>"
color magenta "\<(goto|continue|break|return)\>"

## Comment highlighting
color italic,lightblue start="#!" end="\$|!#"

color white "\?|:|$||\+|\-||\*|/|%|~|^|&|!||^|@|=|<|>|\+"

color pink "\<(\.[0-9]([0-9]*)|[0-9]+(\.)*)\>"

#String highlighting.  You will in general want your comments and
## strings to come last, because syntax highlighting rules will be
## applied in the order they are read in.
#color brightyellow "(\\.|[^"])*""
##
## This string is VERY resource intensive!
#color italic,brightyellow start=""(\\.|[^"])*\\[[:space:]]*$" end="^(\\.|[^"])*""
color italic,brightyellow start="\"" end="\""
color italic,lightyellow start="\`" end="\`"

## Trailing whitespace
color ,green "[[:space:]]+\$"
EOC

