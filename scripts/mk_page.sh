NAME=$(echo $1 | sed 's/\.so//')
DATA="./gwion -p. -P $NAME"

mkpage() {
cat << EOF
# $NAME

compiler generated documentation

\`\`\`gwion
EOF

./gwion -p. -P $NAME

echo '```'
}

mkpage > docs/Reference/Plugins/$NAME.mdr
echo $NAME >> docs/Reference/Plugins/list

echo $NAME
cat docs/Reference/Plugins/list
