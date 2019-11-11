git checkout -b doc_tmp
git branch -r | grep gh-pages && git push origin :gh-pages || true
git add -f book
git commit -m "add book"
git subtree push --prefix book origin gh-pages
git checkout mdbook
git branch -D doc_tmp
