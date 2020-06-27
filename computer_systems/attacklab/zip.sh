#!/bin/bash
# Create solution.tar file
# Do not change this file on your own!

FILES=("c1.txt" "c2.txt" "c3.txt" "r2.txt" "r3.txt")

i=0
for FILE in  ${FILES[@]}; do
  if [ ! -f $FILE ]; then
    echo $FILE not found
    FILES[i]=""
  fi
  i=$(( i+1 ))
done

echo zip ${FILES[@]} files into solution.tar
tar -cvf solution.tar ${FILES[@]} 
