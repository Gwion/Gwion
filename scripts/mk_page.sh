NAME=$(echo $1 | sed 's/\.so//')

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
echo $NAME.md >> docs/Reference/Plugins/list
