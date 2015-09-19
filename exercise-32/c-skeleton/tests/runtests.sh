#!/usr/bin/env bash
# Bash3 Boilerplate. Copyright (c) 2014, kvz.io

set -o errexit
set -o pipefail
set -o nounset
# set -o xtrace

VALGRIND_PREFIX=${VALGRIND:-}

echo "Running unit tests:"

for i in tests/*_test
do
    if test -f $i
    then
        if ${VALGRIND_PREFIX} ./$i
        then
            echo "PASS $i"
        else
            echo "FAIL $i"
            exit 1
        fi
    fi
done

echo ""


