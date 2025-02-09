#!/bin/bash

cd tests

for file in $(ls); do
    if [[ -x "$file" && $file != "test.sh" ]]; then
        ./"$file"

        if [[ $? != 0 ]]; then
            kill $$
        fi
    fi
done
