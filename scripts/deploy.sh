git branch -r | grep gh-pages && git push origin :gh-pages || true
git add -f book
git commit -m "add book"
git subtree push --prefix book origin gh-pages
git rm -r book
rm -rf book
git commit -m "remove book"
