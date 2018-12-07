#!/bin/sh

for pt in $( < $1);do
mv $pt.jets.list $pt.list
done
