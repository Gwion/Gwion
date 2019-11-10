#!/bin/sh
toml() {
cat <<EOF > book.toml
[book]
authors = ["Jérémie Astor"]
title = "Gwion"
theme = "rust"
[output.html]
additional-css = ["custom.css"]

[output.html.fold]
enable = true
level = 0
EOF
}

css() {
cat <<EOF > custom.css
.mdr {
  color:var(--sidebar-active);
  background-color:var(--sidebar-bg);
  border: 5px solid var(--sidebar-fg);
  padding: 10px;
  margin-right: 20%;
  margin-left: 20%;
  moz-border-radius: 15px;
  -webkit-border-radius: 15px;
}

footer {
  text-align: center;
  font-size: small;
  font-style: italic;
}
EOF
}
[ -d src ] || sh scripts/mdr2mdbook.sh

[ -f src/SUMMARY.md ] || sh scripts/summary.sh > src/SUMMARY.md

[ -f book.toml ] || toml

[ -f custom.css ] || css
