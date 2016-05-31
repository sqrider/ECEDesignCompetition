#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Usage: klyt.sh imgfile"
else 
    ./final $1 temp.JPG
    tesseract temp.JPG temptext
    echo ""
    echo "*** RESULTS ***"
    cat temptext.txt
    rm temp.JPG
    rm temptext.txt
fi
