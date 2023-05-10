#!/bin/bash

execute_testcase() {
    echo "$1"
    make -f Makefile framesize="$2" varmemsize="$3"

    cp mysh ../testcases/assignment3

    cd ../testcases/assignment3

    ./mysh < "$4" > output.txt

    diff -w output.txt "$5"

    if [ $? -ne 0 ]
    then
        echo "FAIL";
    else
        echo "PASS";
    fi

    rm output.txt

    rm mysh

    cd ../../src
    
    rm mysh

    echo ""
}

execute_testcase "TC1" 18 10 "T_tc1.txt" "T_tc1_result.txt"
execute_testcase "TC2" 18 10 "T_tc2.txt" "T_tc2_result.txt"
execute_testcase "TC3" 21 10 "T_tc3.txt" "T_tc3_result.txt"
execute_testcase "TC4" 18 10 "T_tc4.txt" "T_tc4_result.txt"
execute_testcase "TC5" 6 10 "T_tc5.txt" "T_tc5_result.txt"
execute_testcase "TC6" 18 10 "T_tc6.txt" "T_tc6_result.txt"
execute_testcase "TC7" 18 10 "T_tc7.txt" "T_tc7_result.txt"
execute_testcase "TC8" 21 10 "T_tc8.txt" "T_tc8_result.txt"
execute_testcase "TC9" 18 10 "T_tc9.txt" "T_tc9_result.txt"
execute_testcase "TC10" 6 10 "T_tc10.txt" "T_tc10_result.txt"