#!/bin/bash
echo "Creating changelog..." && git log >ChangeLog &&
echo "Running aclocal..." && aclocal -I ./m4 $ACLOCAL_FLAGS &&
echo "Running automake..." && automake &&
echo "Running autoconf..." && autoconf &&
echo "Running autoheader..." && autoheader &&
rm -rf autom4te.cache

