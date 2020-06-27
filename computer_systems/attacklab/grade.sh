#!/bin/bash
# Grade your solutions
# Do not change this file on your own!

CTARGET=ctarget
RTARGET=rtarget
HEX2RAW=hex2raw

if [ ! -f $CTARGET ]; then
  echo $CTARGET not found
  exit 1
fi

if [ ! -f $RTARGET ]; then
  echo $RTARGET not found
  exit 1
fi

if [ ! -f $HEX2RAW ]; then
  echo $HEX2RAW not found
  exit 1
fi

FILES=("c1.txt" "c2.txt" "c3.txt" "r2.txt" "r3.txt")
POINTS=(10 25 25 35 5)

GRADE=0

for i in  0 1 2; do
  FILE=${FILES[i]}
  if [ ! -f $FILE ]; then
    echo $FILE not found
  else
    PASS=$(cat $FILE | ./$HEX2RAW | ./$CTARGET -q | grep -oh -m 1 "\w*PASS\w*")
    TOUCH=$(cat $FILE | ./$HEX2RAW | ./$CTARGET -q | grep -oh -m 1 "\w*touch$(( i + 1 ))\w*")
    if [ "$PASS" = PASS ] && [ "$TOUCH" = touch$(( i + 1 )) ]; then
      echo PASS: $FILE
      GRADE=$(( $GRADE + ${POINTS[i]} ))
    else
      echo FAIL: $FILE
    fi
  fi
done

for i in 3 4; do
  FILE=${FILES[i]}
  if [ ! -f $FILE ]; then
    echo $FILE not found
  else
    PASS=$(cat $FILE | ./$HEX2RAW | ./$RTARGET -q | grep -oh -m 1 "\w*PASS\w*")
    TOUCH=$(cat $FILE | ./$HEX2RAW | ./$RTARGET -q | grep -oh -m 1 "\w*touch$(( i - 1 ))\w*")
    if [ "$PASS" = PASS ] && [ "$TOUCH" = touch$(( i - 1 )) ]; then
      echo PASS: $FILE
      GRADE=$(( $GRADE + ${POINTS[i]} ))
    else
      echo FAIL: $FILE
    fi
  fi
done

echo ""
echo Total: $GRADE/100
