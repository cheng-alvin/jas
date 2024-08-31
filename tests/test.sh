#!/bin/bash

for file in $(ls); do
    if [[ -x "$file" && $file != "test.sh" ]]; then
        ./"$file"

        if [[ $? != 0 ]]; then
            kill $$
        fi
    fi
done

make clean
