#!/usr/bin/bash

echo Building NasaParser...
if ! gcc main.c; then 
    echo -e "\e[31;1mBuild Failed. \e[33mExiting...\e[0m" 
    exit 1
fi
i=1
exit_code=0
function test_parser() {
    got=$(./a.out NASA_access_log_Jul95 $1 | grep queries | cut -d' ' -f8)
    if [[ $got -eq $2 ]]; then
	    echo -e "Test \e[33;1m$i \e[32mPASSED\e[0m"
        return 0
    else
	    echo -e "Test \e[33;1m$i \e[31mFAILED\e[0m [Got: $got, Expected: $2]"
        return 1
    fi
}

test_parser 1 38 || exit_code=1
i=$(($i + 1))

test_parser 10 97 || exit_code=1
i=$(($i + 1))

test_parser 2 45 || exit_code=1
i=$(($i + 1))

test_parser 100 630 || exit_code=1
i=$(($i + 1))

test_parser 1000 4740 || exit_code=1



exit $exit_code

